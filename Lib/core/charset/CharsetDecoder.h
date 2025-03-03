//
// Created by brunshweeck on 2 sept. 2024.
//

#ifndef CORE24_CHARSETDECODER_H
#define CORE24_CHARSETDECODER_H

#include <core/charset/Charset.h>
#include <core/charset/CoderResult.h>

namespace core {
    namespace charset {
        /**
         * An engine that can transform a sequence of bytes in a specific charset into a sequence of
         * sixteen-bit Unicode characters.
         *
         * <a id="steps"></a>
         *
         * <p>
         * The input byte sequence is provided in a byte buffer or a series
         * of such buffers.  The output character sequence is written to a character buffer
         * or a series of such buffers.  A decoder should always be used by making
         * the following sequence of method invocations, hereinafter referred to as a
         * <i>decoding operation</i>: <br/>
         *
         *   - Reset the decoder via the @b reset method, unless it
         *   has not been used before; <br/>
         *
         *   - Invoke the @b decode method zero or more times, as
         *   long as additional input may be available, passing @c false for the
         *   @c endOfInput argument and filling the input buffer and flushing the
         *   output buffer between invocations; <br/>
         *
         *   - Invoke the @b decode method one final time, passing
         *   @c true for the @c endOfInput argument; and then <br/>
         *
         *   - Invoke the @b flush method so that the decoder can
         *   flush any internal state to the output buffer. <br/>
         *
         * Each invocation of the @b decode method will decode as many
         * bytes as possible from the input buffer, writing the resulting characters
         * to the output buffer.  The @b decode method returns when more
         * input is required, when there is not enough room in the output buffer, or
         * when a decoding error has occurred.  In each case a @b CoderResult
         * object is returned to describe the reason for termination.  An invoker can
         * examine this object and fill the input buffer, flush the output buffer, or
         * attempt to recover from a decoding error, as appropriate, and try again.
         * </p>
         * <a id="ce"></a>
         *
         * <p>
         * There are two general types of decoding errors.  If the input byte
         * sequence is not legal for this charset then the input is considered <i>malformed</i>.  If
         * the input byte sequence is legal but cannot be mapped to a valid
         * Unicode character then an <i>unmappable character</i> has been encountered.
         * </p>
         * <a id="cae"></a>
         *
         * <p>
         * How a decoding error is handled depends upon the action requested for
         * that type of error, which is described by an instance of the
         * @b CodingErrorAction class.  The possible error actions are to
         * @em ignore the erroneous input, @em report the error to the invoker via
         * the returned @b CoderResult object, or @em replace the erroneous input
         * with the current value of the replacement string.  The replacement
         * has the initial value <code>"\uFFFD"</code>;
         *
         * its value may be changed via the @b replaceWith method.
         * </p>
         * <p>
         * The default action for malformed-input and unmappable-character errors
         * is to @em report them.  The
         * malformed-input error action may be changed via the @b onMalformedInput method; the
         * unmappable-character action may be changed via the @b onUnmappableCharacter method.
         * </p>
         * <p>
         * This class is designed to handle many of the details of the decoding
         * process, including the implementation of error actions.  A decoder for a
         * specific charset, which is a concrete subclass of this class, need only
         * implement the abstract @b decode0 method, which
         * encapsulates the basic decoding loop.  A subclass that maintains internal
         * state should, additionally, override the @b flush0 and
         * @b reset0 methods.
         * </p>
         * <p>
         * Instances of this class are not safe for use by multiple concurrent
         * threads.
         * </p>
         *
         * @see ByteBuffer
         * @see CharBuffer
         * @see Charset
         * @see CharsetEncoder
         */
        class /* abstract */ CharsetDecoder : public virtual Object {
            CORE_ALIAS(UNSAFE, misc::Unsafe);

            /**
             * The charset using this decoder.
             */
            Charset const& cs;

            /**
             * The average number of chars per byte
             */
            gfloat averageCpB = 0;

            /**
             * The max number of chars per byte
             */
            gfloat maxCpB = 0;

            /**
             * The replacement string
             */
            String repl;

            /**
             * The action to execute after that malformed input has been detected
             */
            Charset::CodingErrorAction mia = Charset::CodingErrorAction::REPORT;

            /**
             * The action to execute after that unmappable character has been detected
             */
            Charset::CodingErrorAction uca = Charset::CodingErrorAction::REPORT;

            enum class State {
                ST_RESET,
                ST_CODING,
                ST_END,
                ST_FLUSHED,
            } state = State::ST_RESET;

        protected:
            /**
             * Initializes a new decoder.  The new decoder will have the given
             * chars-per-byte and replacement values.
             *
             * @param  cs
             *         The charset that created this decoder
             *
             * @param  averageCharsPerByte
             *         A positive float value indicating the expected number of
             *         characters that will be produced for each input byte
             *
             * @param  maxCharsPerByte
             *         A positive float value indicating the maximum number of
             *         characters that will be produced for each input byte
             *
             * @param  replacement
             *         The initial replacement; must have
             *         non-zero length, must not be longer than maxCharsPerByte,
             *         and must be @em legal
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameters do not hold
             */
            CORE_EXPLICIT CharsetDecoder(Charset const& cs,
                                         gfloat averageCharsPerByte,
                                         gfloat maxCharsPerByte,
                                         String replacement);

            /**
             * Initializes a new decoder.  The new decoder will have the given
             * chars-per-byte values and its replacement will be the
             * string <code>"\uFFFD"</code>.
             *
             * @param  cs
             *         The charset that created this decoder
             *
             * @param  averageCharsPerByte
             *         A positive float value indicating the expected number of
             *         characters that will be produced for each input byte
             *
             * @param  maxCharsPerByte
             *         A positive float value indicating the maximum number of
             *         characters that will be produced for each input byte
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameters do not hold
             */
            CORE_EXPLICIT CharsetDecoder(Charset const& cs,
                                         gfloat averageCharsPerByte,
                                         gfloat maxCharsPerByte);

        public:
            /**
             * Returns the charset that created this decoder.
             *
             * @return  This decoder's charset
             */
            Charset const& charset() const;

            /**
             * Returns this decoder's replacement value.
             *
             * @return  This decoder's current replacement,
             *          which is never empty
             */
            String replacement() const;

            /**
             * Changes this decoder's replacement value.
             *
             * <p>
             * This method invokes the @b replaceWith0 method, passing the new replacement,
             * after checking that the new replacement is acceptable.
             * </p>
             *
             * @param  newReplacement  The new replacement; must have non-zero length,
             *         and must not be longer than the value returned by the
             *         @b maxCharsPerByte method
             *
             * @return  This decoder
             *
             * @throws  IllegalArgumentException
             *          If the preconditions on the parameter do not hold
             */
            CharsetDecoder& replaceWith(String const& newReplacement);

            /**
             * Returns this decoder's current action for malformed-input errors.
             *
             * @return The current malformed-input action
             */
            Charset::CodingErrorAction malformedInputAction() const;

            /**
             * Changes this decoder's action for malformed-input errors.
             *
             * <p>
             * This method invokes the @b onMalformedInput0 method, passing the new action.
             * </p>
             *
             * @param  newAction  The new action
             *
             * @return  This decoder
             *
             * @throws IllegalArgumentException
             *         If the precondition on the parameter does not hold
             */
            CharsetDecoder& onMalformedInput(Charset::CodingErrorAction newAction);

            /**
             * Returns this decoder's current action for unmappable-character errors.
             *
             * @return The current unmappable-character action
             */
            Charset::CodingErrorAction unmappableCharacterAction() const;

            /**
             * Changes this decoder's action for unmappable-character errors.
             *
             * <p>
             * This method invokes the @b onUnmappableCharacter0 method, passing the new action.
             * </p>
             *
             * @param  newAction  The new action
             *
             * @return  This decoder
             *
             * @throws IllegalArgumentException
             *         If the precondition on the parameter does not hold
             */
            CharsetDecoder& onUnmappableCharacter(Charset::CodingErrorAction newAction);

            /**
             * Returns the average number of characters that will be produced for each
             * byte of input.  This heuristic value may be used to estimate the size
             * of the output buffer required for a given input sequence.
             *
             * @return  The average number of characters produced
             *          per byte of input
             */
            gfloat averageCharsPerByte() const;

            /**
             * Returns the maximum number of characters that will be produced for each
             * byte of input.  This value may be used to compute the worst-case size
             * of the output buffer required for a given input sequence. This value
             * accounts for any necessary content-independent prefix or suffix
             * characters.
             *
             * @return  The maximum number of characters that will be produced per
             *          byte of input
             */
            gfloat maxCharsPerByte() const;

            /**
             * Decodes as many bytes as possible from the given input buffer,
             * writing the results to the given output buffer.
             *
             * <p>
             * The buffers are read from, and written to, starting at their current
             * positions.  At most @b in.remaining() bytes
             * will be read and at most @b out.remaining()
             * characters will be written.  The buffers' positions will be advanced to
             * reflect the bytes read and the characters written, but their marks and
             * limits will not be modified.
             * </p>
             * <p>
             * In addition to reading bytes from the input buffer and writing
             * characters to the output buffer, this method returns a @b CoderResult
             * object to describe its reason for termination: <br/>
             *
             *   - @b CoderResult::UNDERFLOW indicates that as much of the
             *   input buffer as possible has been decoded.  If there is no further
             *   input then the invoker can proceed to the next step of the
             *   <a href="#steps">decoding operation</a>.  Otherwise, this method
             *   should be invoked again with further input. <br/>
             *
             *   - @b CoderResult::OVERFLOW indicates that there is
             *   insufficient space in the output buffer to decode any more bytes.
             *   This method should be invoked again with an output buffer that has
             *   more @em remaining characters. This is
             *   typically done by draining any decoded characters from the output
             *   buffer. <br/>
             *
             *   - A @em malformed-input result indicates that a malformed-input
             *   error has been detected.  The malformed bytes begin at the input
             *   buffer's (possibly incremented) position; the number of malformed
             *   bytes may be determined by invoking the result object's @b length method.
             *   This case applies only if the <em>malformed action</em> of this decoder
             *   is @b CodingErrorAction::REPORT; otherwise the malformed input
             *   will be ignored or replaced, as requested. <br/>
             *
             *   - An @em unmappable-character result indicates that an
             *   unmappable-character error has been detected.  The bytes that
             *   decode the unmappable character begin at the input buffer's (possibly
             *   incremented) position; the number of such bytes may be determined
             *   by invoking the result object's @b length
             *   method.  This case applies only if the <em>unmappable action</em>
             *   of this decoder is @b CodingErrorAction::REPORT; otherwise the unmappable character will be
             *   ignored or replaced, as requested. <br/>
             *
             * In any case, if this method is to be reinvoked in the same decoding
             * operation then care should be taken to preserve any bytes remaining
             * in the input buffer so that they are available to the next invocation.
             * </p>
             * <p>
             * The @c endOfInput parameter advises this method whether
             * the invoker can provide further input beyond that contained in the given
             * input buffer.  If there is a possibility of providing additional input
             * then the invoker should pass @c false for this parameter; if there
             * is no possibility of providing further input then the invoker should
             * pass @c true.  It is not erroneous, and in fact it is quite
             * common, to pass @c false in one invocation and later discover that
             * no further input was actually available.  It is critical, however, that
             * the final invocation of this method in a sequence of invocations always
             * pass @c true so that any remaining undecoded input will be treated
             * as being malformed.
             * </p>
             * <p>
             * This method works by invoking the @b decode0
             * method, interpreting its results, handling error conditions, and
             * reinvoking it as necessary.
             * </p>
             *
             *
             * @param  in
             *         The input byte buffer
             *
             * @param  out
             *         The output character buffer
             *
             * @param  endOfInput
             *         @c true if, and only if, the invoker can provide no
             *         additional input bytes beyond those in the given buffer
             *
             * @return  A coder-result object describing the reason for termination
             *
             * @throws  IllegalStateException
             *          If a decoding operation is already in progress and the previous
             *          step was an invocation neither of the @b reset
             *          method, nor of this method with a value of @c false for
             *          the @c endOfInput parameter, nor of this method with a
             *          value of @c true for the @c endOfInput parameter
             *          but a return value indicating an incomplete decoding operation
             *
             * @throws  CoderMalfunctionError
             *          If an invocation of the decode0 method threw
             *          an unexpected exception
             */
            CoderResult decode(io::ByteBuffer& in, io::CharBuffer& out, gbool endOfInput);

            /**
             * Flushes this decoder.
             *
             * <p>
             * Some decoders maintain internal state and may need to write some
             * final characters to the output buffer once the overall input sequence has
             * been read.
             * </p>
             * <p>
             * Any additional output is written to the output buffer beginning at
             * its current position.  At most @b out.remaining()
             * characters will be written.  The buffer's position will be advanced
             * appropriately, but its mark and limit will not be modified.
             * </p>
             * <p>
             * If this method completes successfully then it returns
             * @b CoderResult::UNDERFLOW.  If there is insufficient room in the output
             * buffer then it returns @b CoderResult::OVERFLOW.  If this happens
             * then this method must be invoked again, with an output buffer that has
             * more room, in order to complete the current <a href="#steps">decoding
             * operation</a>.
             * </p>
             * <p>
             * If this decoder has already been flushed then invoking this method
             * has no effect.
             * </p>
             * <p>
             * This method invokes the @b flush0 method to
             * perform the actual flushing operation.
             * </p>
             *
             * @param  out
             *         The output character buffer
             *
             * @return  A coder-result object, either @b CoderResult::UNDERFLOW or
             *          @b CoderResult::OVERFLOW
             *
             * @throws  IllegalStateException
             *          If the previous step of the current decoding operation was an
             *          invocation neither of the @b flush method nor of
             *          the three-argument @b decode method
             *          with a value of @c true for the @c endOfInput
             *          parameter
             */
            CoderResult flush(io::CharBuffer& out);

            /**
             * Resets this decoder, clearing any internal state.
             *
             * <p>
             * This method resets charset-independent state and also invokes the
             * @b reset0 method in order to perform any charset-specific reset actions.
             * </p>
             *
             * @return  This decoder
             */
            CharsetDecoder& reset();

            /**
             * Convenience method that decodes the remaining content of a single input
             * byte buffer into a newly-allocated character buffer.
             *
             * <p>
             * This method implements an entire <a href="#steps">decoding
             * operation</a>; that is, it resets this decoder, then it decodes the
             * bytes in the given byte buffer, and finally it flushes this
             * decoder.  This method should therefore not be invoked if a decoding
             * operation is already in progress.
             * </p>
             *
             * @param  in
             *         The input byte buffer
             *
             * @return A newly-allocated character buffer containing the result of the
             *         decoding operation.  The buffer's position will be zero and its
             *         limit will follow the last character written.
             *
             * @throws  IllegalStateException
             *          If a decoding operation is already in progress
             *
             * @throws  MalformedInputException
             *          If the byte sequence starting at the input buffer's current
             *          position is not legal for this charset and the current malformed-input action
             *          is @b CodingErrorAction::REPORT
             *
             * @throws  UnmappableCharacterException
             *          If the byte sequence starting at the input buffer's current
             *          position cannot be mapped to an equivalent character sequence and
             *          the current unmappable-character action is @b CodingErrorAction::REPORT
             *
             * @throws  CharacterCodingException
             *          @c MalformedInputException if the byte sequence starting at the
             *          input buffer's current position is not legal for this charset and the current
             *          malformed-input action is @c CodingErrorAction::REPORT;
             *          @c UnmappableCharacterException if the byte sequence starting at
             *          the input buffer's current position cannot be mapped to an
             *          equivalent character sequence and the current unmappable-character
             *          action is @c CodingErrorAction::REPORT
             *
             * @throws  OutOfMemoryError
             *          If the output character buffer for the requested size of the input
             *          byte buffer cannot be allocated
             */
            io::CharBuffer& decode(io::ByteBuffer& in);

            /**
             * Tells whether this decoder implements an auto-detecting charset.
             *
             * <p>
             * The default implementation of this method always returns
             * @c false; it should be overridden by auto-detecting decoders to
             * return @c true.
             * </p>
             *
             * @return  @c true if, and only if, this decoder implements an
             *          auto-detecting charset
             */
            virtual gbool isAutoDetecting();

            /**
             * Tells whether this decoder has yet detected a
             * charset  <i>(optional operation)</i>.
             *
             * <p>
             * If this decoder implements an auto-detecting charset then at a
             * single point during a decoding operation this method may start returning
             * @c true to indicate that a specific charset has been detected in
             * the input byte sequence.  Once this occurs, the @b detectedCharset method
             * may be invoked to retrieve the detected charset.
             * </p>
             * <p>
             * That this method returns @c false does not imply that no bytes
             * have yet been decoded.  Some auto-detecting decoders are capable of
             * decoding some, or even all, of an input byte sequence without fixing on
             * a particular charset.
             * </p>
             * <p>
             * The default implementation of this method always throws an
             * @b UnsupportedOperationException; it should be overridden by
             * auto-detecting decoders to return @c true once the input charset
             * has been determined.
             * </p>
             *
             * @return  @c true if, and only if, this decoder has detected a
             *          specific charset
             *
             * @throws  UnsupportedOperationException
             *          If this decoder does not implement an auto-detecting charset
             */
            virtual gbool isCharsetDetected() const;

            /**
             * Retrieves the charset that was detected by this
             * decoder  <i>(optional operation)</i>.
             *
             * <p>
             * If this decoder implements an auto-detecting charset then this
             * method returns the actual charset once it has been detected.  After that
             * point, this method returns the same value for the duration of the
             * current decoding operation.  If not enough input bytes have yet been
             * read to determine the actual charset then this method throws an
             * @b IllegalStateException.
             * </p>
             * <p>
             * The default implementation of this method always throws an
             * @b UnsupportedOperationException; it should be overridden by
             * auto-detecting decoders to return the appropriate value.
             * </p>
             * @return  The charset detected by this auto-detecting decoder,
             *          or @c null if the charset has not yet been determined
             *
             * @throws  IllegalStateException
             *          If insufficient bytes have been read to determine a charset
             *
             * @throws  UnsupportedOperationException
             *          If this decoder does not implement an auto-detecting charset
             */
            virtual Object const& detectCharset() const;

        protected:
            // ----------------------- Impl version of methods ------------------------------

            /**
             * Reports a change to this decoder's replacement value.
             *
             * <p>
             * The default implementation of this method does nothing.  This method
             * should be overridden by decoders that require notification of changes to
             * the replacement.
             * </p>
             *
             * @param  newReplacement    The replacement value
             */
            virtual void replaceWith0(String const& newReplacement);

            /**
             * Reports a change to this decoder's malformed-input action.
             *
             * <p>
             * The default implementation of this method does nothing.  This method
             * should be overridden by decoders that require notification of changes to
             * the malformed-input action.
             * </p>
             *
             * @param  newAction  The new action
             */
            virtual void onMalformedInput0(Charset::CodingErrorAction newAction);

            /**
             * Reports a change to this decoder's unmappable-character action.
             *
             * <p>
             * The default implementation of this method does nothing.  This method
             * should be overridden by decoders that require notification of changes to
             * the unmappable-character action.
             * </p>
             *
             * @param  newAction  The new action
             */
            virtual void onUnmappableCharacter0(Charset::CodingErrorAction newAction);

            /**
             * Flushes this decoder.
             *
             * <p>
             * The default implementation of this method does nothing, and always
             * returns @b CoderResult::UNDERFLOW.  This method should be overridden
             * by decoders that may need to write final characters to the output buffer
             * once the entire input sequence has been read.
             * </p>
             *
             * @param  out
             *         The output character buffer
             *
             * @return  A coder-result object, either @b CoderResult::UNDERFLOW or
             *          @b CoderResult::OVERFLOW
             */
            virtual CoderResult flush0(io::CharBuffer& out);

            /**
             * Resets this decoder, clearing any charset-specific internal state.
             *
             * <p>
             * The default implementation of this method does nothing.  This method
             * should be overridden by decoders that maintain internal state.
             * </p>
             */
            virtual void reset0();

            /**
             * Decodes one or more bytes into one or more characters.
             *
             * <p>
             * This method encapsulates the basic decoding loop, decoding as many
             * bytes as possible until it either runs out of input, runs out of room
             * in the output buffer, or encounters a decoding error.  This method is
             * invoked by the @b decode method, which handles result
             * interpretation and error recovery.
             * </p>
             * <p>
             * The buffers are read from, and written to, starting at their current
             * positions.  At most @b in.remaining() bytes
             * will be read, and at most @b out.remaining()
             * characters will be written.  The buffers' positions will be advanced to
             * reflect the bytes read and the characters written, but their marks and
             * limits will not be modified.
             * </p>
             * <p>
             * This method returns a @b CoderResult object to describe its
             * reason for termination, in the same manner as the @b decode
             * method.  Most implementations of this method will handle decoding errors
             * by returning an appropriate result object for interpretation by the
             * @b decode method.  An optimized implementation may instead
             * examine the relevant error action and implement that action itself.
             * </p>
             * <p>
             * An implementation of this method may perform arbitrary lookahead by
             * returning @b CoderResult::UNDERFLOW until it receives sufficient
             * input.
             * </p>
             *
             * @param  in
             *         The input byte buffer
             *
             * @param  out
             *         The output character buffer
             *
             * @return  A coder-result object describing the reason for termination
             */
            virtual CoderResult decode0(io::ByteBuffer& in, io::CharBuffer& out) = 0;
        };
    } // charset
} // core

#endif //CORE24_CHARSETDECODER_H
