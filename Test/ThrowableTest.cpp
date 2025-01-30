//
// Created by admin on 14/01/25.
//

#include "Printer.h"
#include <gtest/gtest.h>
#include <core/ArithmeticException.h>
#include <core/ClassCastException.h>
#include <core/CloneNotSupportedException.h>
#include <core/IllegalArgumentException.h>
#include <core/IllegalStateException.h>
#include <core/IllegalThreadStateException.h>
#include <core/IndexOutOfBoundsException.h>
#include <core/InterruptedException.h>
#include <core/NumberFormatException.h>
#include <core/RuntimeException.h>
#include <core/SecurityException.h>
#include <core/UnsupportedOperationException.h>
#include <core/charset/CharacterCodingException.h>
#include <core/charset/IllegalCharsetNameException.h>
#include <core/charset/MalformedInputException.h>
#include <core/charset/UnmappableCharacterException.h>
#include <core/charset/UnsupportedCharsetException.h>
#include <core/io/AccessDeniedException.h>
#include <core/io/AtomicMoveNotSupportedException.h>
#include <core/io/BufferOverflowException.h>
#include <core/io/BufferUnderflowException.h>
#include <core/io/DirectoryNotEmptyException.h>
#include <core/io/EOFException.h>
#include <core/io/FileAlreadyExistsException.h>
#include <core/io/FileSystemAlreadyExistsException.h>
#include <core/io/FileSystemException.h>
#include <core/io/FileSystemLoopException.h>
#include <core/io/FileSystemNotFoundException.h>
#include <core/io/InvalidMarkException.h>
#include <core/io/IOException.h>
#include <core/io/NoSuchFileException.h>
#include <core/io/NotDirectoryException.h>
#include <core/io/NotLinkException.h>
#include <core/io/ReadOnlyBufferException.h>
#include <core/io/ReadOnlyFileSystemException.h>
#include <core/concurrent/RejectedExecutionException.h>
#include <core/net/URISyntaxException.h>
#include <core/regex/PatternSyntaxException.h>
#include <core/time/DateTimeException.h>
#include <core/time/TemporalException.h>
#include <core/time/ZoneRulesException.h>
#include <core/util/ConcurrentModificationException.h>
#include <core/util/IllegalFormatException.h> // ignored (abstract class)
#include <core/util/MissingKeyException.h>
#include <core/util/MissingResourceException.h>
#include <core/AssertionError.h>
#include <core/OutOfMemoryError.h>
#include <core/charset/CoderMalfunctionError.h>
#include <core/io/IOError.h>
#include <core/misc/Unsafe.h>

namespace core {
    class Warning final : public Throwable {
    protected:
        void selfThrow() const override {
            throw Warning(*this);
        }

    public:
        String message() const override {
            return "WARN: ";
        }

        Object &clone() const override {
            return UNSAFE::newInstance<Warning>(*this);
        }
    };

    class ThrowableTest : public Object, public testing::Test {
    protected:

        void SetUp() override {
            //
        }

        void TearDown() override {
            //
        }

    private:
        void TestBody() override = 0;
    };

    class ExceptionTest : public ThrowableTest {
    protected:

        void SetUp() override {
            //
        }

        void TearDown() override {
            //
        }

    private:
        void TestBody() override = 0;
    };

    TEST_F(ExceptionTest, constructorForExceptionClass) {
        Exception ex1 = Exception();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        Exception ex2 = Exception("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        Exception ex3 = Exception("I am backend developer."_Sl, Warning());
        ASSERT_FALSE(ex3.message().isEmpty());
        ASSERT_NE(ex3, ex3.cause());
        Exception ex4 = Exception(Warning());
        ASSERT_FALSE(ex4.message().isEmpty());
        ASSERT_NE(ex4, ex4.cause());
    }

    TEST_F(ExceptionTest, constructorForArithmeticExceptionClass) {
        ArithmeticException ex1 = ArithmeticException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        ArithmeticException ex2 = ArithmeticException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
    }

    TEST_F(ExceptionTest, constructorForClassCastExceptionClass) {
        ClassCastException ex1 = ClassCastException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        ClassCastException ex2 = ClassCastException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
    }

    TEST_F(ExceptionTest, constructorForCloneNotSupportedExceptionClass) {
        CloneNotSupportedException ex1 = CloneNotSupportedException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        CloneNotSupportedException ex2 = CloneNotSupportedException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
    }

    TEST_F(ExceptionTest, constructorForIllegalArgumentExceptionClass) {
        IllegalArgumentException ex1 = IllegalArgumentException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        IllegalArgumentException ex2 = IllegalArgumentException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        IllegalArgumentException ex3 = IllegalArgumentException("I am backend developer."_Sl, Warning());
        ASSERT_FALSE(ex3.message().isEmpty());
        ASSERT_NE(ex3, ex3.cause());
        IllegalArgumentException ex4 = IllegalArgumentException(Warning());
        ASSERT_FALSE(ex4.message().isEmpty());
        ASSERT_NE(ex4, ex4.cause());
    }

    TEST_F(ExceptionTest, constructorForIllegalStateExceptionClass) {
        IllegalStateException ex1 = IllegalStateException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        IllegalStateException ex2 = IllegalStateException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        IllegalStateException ex3 = IllegalStateException("I am backend developer."_Sl, Warning());
        ASSERT_FALSE(ex3.message().isEmpty());
        ASSERT_NE(ex3, ex3.cause());
        IllegalStateException ex4 = IllegalStateException(Warning());
        ASSERT_FALSE(ex4.message().isEmpty());
        ASSERT_NE(ex4, ex4.cause());
    }

    TEST_F(ExceptionTest, constructorForIllegalThreadStateExceptionClass) {
        IllegalThreadStateException ex1 = IllegalThreadStateException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        IllegalThreadStateException ex2 = IllegalThreadStateException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
    }

    TEST_F(ExceptionTest, constructorForIndexOutOfBoundsExceptionClass) {
        IndexOutOfBoundsException ex1 = IndexOutOfBoundsException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        IndexOutOfBoundsException ex2 = IndexOutOfBoundsException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        IndexOutOfBoundsException ex3 = IndexOutOfBoundsException(45);
        ASSERT_FALSE(ex3.message().isEmpty());
    }

    TEST_F(ExceptionTest, constructorForInterruptedExceptionClass) {
        InterruptedException ex1 = InterruptedException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        InterruptedException ex2 = InterruptedException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
    }

    TEST_F(ExceptionTest, constructorForNumberFormatExceptionClass) {
        NumberFormatException ex1 = NumberFormatException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        NumberFormatException ex2 = NumberFormatException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
    }

    TEST_F(ExceptionTest, constructorForRuntimeExceptionClass) {
        RuntimeException ex1 = RuntimeException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        RuntimeException ex2 = RuntimeException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        RuntimeException ex3 = RuntimeException("I am backend developer."_Sl, Warning());
        ASSERT_FALSE(ex3.message().isEmpty());
        ASSERT_NE(ex3, ex3.cause());
        RuntimeException ex4 = RuntimeException(Warning());
        ASSERT_FALSE(ex4.message().isEmpty());
        ASSERT_NE(ex4, ex4.cause());
    }

    TEST_F(ExceptionTest, constructorForSecurityExceptionClass) {
        SecurityException ex1 = SecurityException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        SecurityException ex2 = SecurityException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        SecurityException ex3 = SecurityException("I am backend developer."_Sl, Warning());
        ASSERT_FALSE(ex3.message().isEmpty());
        ASSERT_NE(ex3, ex3.cause());
        SecurityException ex4 = SecurityException(Warning());
        ASSERT_FALSE(ex4.message().isEmpty());
        ASSERT_NE(ex4, ex4.cause());
    }

    TEST_F(ExceptionTest, constructorForUnsupportedOperationExceptionClass) {
        UnsupportedOperationException ex1 = UnsupportedOperationException();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        UnsupportedOperationException ex2 = UnsupportedOperationException("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        UnsupportedOperationException ex3 = UnsupportedOperationException("I am backend developer."_Sl, Warning());
        ASSERT_FALSE(ex3.message().isEmpty());
        ASSERT_NE(ex3, ex3.cause());
        UnsupportedOperationException ex4 = UnsupportedOperationException(Warning());
        ASSERT_FALSE(ex4.message().isEmpty());
        ASSERT_NE(ex4, ex4.cause());
    }

    namespace charset {

        TEST_F(ExceptionTest, constructorForCharacterCodingExceptionClass) {
            CharacterCodingException ex1 = CharacterCodingException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
        }

        TEST_F(ExceptionTest, constructorForIllegalCharsetNameExceptionClass) {
            IllegalCharsetNameException ex2 = IllegalCharsetNameException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForMalformedInputExceptionClass) {
            MalformedInputException ex2 = MalformedInputException(45);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForUnmappableCharacterExceptionClass) {
            UnmappableCharacterException ex2 = UnmappableCharacterException(45);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForUnsupportedCharsetExceptionClass) {
            UnsupportedCharsetException ex2 = UnsupportedCharsetException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }
    }

    namespace io {

        TEST_F(ExceptionTest, constructorForAccessDeniedExceptionClass) {
            AccessDeniedException ex2 = AccessDeniedException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            AccessDeniedException ex3 = AccessDeniedException("I am developer."_Sl, "I am backend developer.",
                                                              "I love this.");
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_EQ(ex3, ex3.cause());
        }

        TEST_F(ExceptionTest, constructorForAtomicMoveNotSupportedExceptionClass) {
            AtomicMoveNotSupportedException ex2 = AtomicMoveNotSupportedException("I am developer."_Sl,
                                                                                  "I am backend developer.",
                                                                                  "I love this.");
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForBufferOverflowExceptionClass) {
            BufferOverflowException ex1 = BufferOverflowException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
        }

        TEST_F(ExceptionTest, constructorForBufferUnderflowExceptionClass) {
            BufferUnderflowException ex1 = BufferUnderflowException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
        }

        TEST_F(ExceptionTest, constructorForDirectoryNotEmptyExceptionClass) {
            DirectoryNotEmptyException ex2 = DirectoryNotEmptyException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForEOFExceptionClass) {
            EOFException ex1 = EOFException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
            EOFException ex2 = EOFException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForFileAlreadyExistsExceptionClass) {
            FileAlreadyExistsException ex2 = FileAlreadyExistsException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            FileAlreadyExistsException ex3 = FileAlreadyExistsException("I am developer."_Sl,
                                                                        "I am backend developer."_Sl,
                                                                        "I love this."_Sl);
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_EQ(ex3, ex3.cause());
        }

        TEST_F(ExceptionTest, constructorForFileSystemAlreadyExistsExceptionClass) {
            FileSystemAlreadyExistsException ex2 = FileSystemAlreadyExistsException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForFileSystemExceptionClass) {
            FileSystemException ex2 = FileSystemException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            FileSystemException ex3 = FileSystemException("I am developer."_Sl,
                                                          "I am backend developer."_Sl,
                                                          "I love this."_Sl);
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_EQ(ex3, ex3.cause());
        }

        TEST_F(ExceptionTest, constructorForFileSystemLoopExceptionClass) {
            FileSystemLoopException ex2 = FileSystemLoopException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForFileSystemNotFoundExceptionClass) {
            FileSystemNotFoundException ex2 = FileSystemNotFoundException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForInvalidMarkExceptionClass) {
            InvalidMarkException ex1 = InvalidMarkException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
        }

        TEST_F(ExceptionTest, constructorForIOExceptionClass) {
            IOException ex1 = IOException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
            IOException ex2 = IOException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            IOException ex3 = IOException("I am backend developer."_Sl, Warning());
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_NE(ex3, ex3.cause());
            IOException ex4 = IOException(Warning());
            ASSERT_FALSE(ex4.message().isEmpty());
            ASSERT_NE(ex4, ex4.cause());
        }

        TEST_F(ExceptionTest, constructorForNoSuchFileExceptionClass) {
            NoSuchFileException ex2 = NoSuchFileException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            NoSuchFileException ex3 = NoSuchFileException("I am developer."_Sl,
                                                          "I am backend developer."_Sl,
                                                          "I love this."_Sl);
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_EQ(ex3, ex3.cause());
        }

        TEST_F(ExceptionTest, constructorForNotDirectoryExceptionClass) {
            NotDirectoryException ex2 = NotDirectoryException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForNotLinkExceptionClass) {
            NotLinkException ex2 = NotLinkException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            NotLinkException ex3 = NotLinkException("I am developer."_Sl,
                                                    "I am backend developer."_Sl,
                                                    "I love this."_Sl);
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_EQ(ex3, ex3.cause());
        }

        TEST_F(ExceptionTest, constructorForReadOnlyBufferExceptionClass) {
            ReadOnlyBufferException ex1 = ReadOnlyBufferException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
        }

        TEST_F(ExceptionTest, constructorForReadOnlyFileSystemExceptionClass) {
            ReadOnlyFileSystemException ex1 = ReadOnlyFileSystemException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
        }
    }

    namespace concurrent {

        TEST_F(ExceptionTest, constructorForRejectedExecutionExceptionClass) {
            RejectedExecutionException ex1 = RejectedExecutionException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
            RejectedExecutionException ex2 = RejectedExecutionException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            RejectedExecutionException ex3 = RejectedExecutionException("I am backend developer."_Sl, Warning());
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_NE(ex3, ex3.cause());
            RejectedExecutionException ex4 = RejectedExecutionException(Warning());
            ASSERT_FALSE(ex4.message().isEmpty());
            ASSERT_NE(ex4, ex4.cause());
        }
    }

    namespace net {

        TEST_F(ExceptionTest, constructorForURISyntaxExceptionClass) {
            URISyntaxException ex2 = URISyntaxException("I am developer."_Sl, "I love this.");
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            URISyntaxException ex3 = URISyntaxException("I am developer."_Sl, "I love this.", 5);
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_EQ(ex3, ex3.cause());
        }
    }

    namespace regex {

        TEST_F(ExceptionTest, constructorForPatternSyntaxExceptionClass) {
            PatternSyntaxException ex2 = PatternSyntaxException("I am developer."_Sl, "I am backend developer.", 5);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }
    }

    namespace time {

        TEST_F(ExceptionTest, constructorForDateTimeExceptionClass) {
            DateTimeException ex2 = DateTimeException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            DateTimeException ex3 = DateTimeException("I am backend developer."_Sl, Warning());
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_NE(ex3, ex3.cause());
        }

        TEST_F(ExceptionTest, constructorForTemporalExceptionClass) {
            TemporalException ex2 = TemporalException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            TemporalException ex3 = TemporalException("I am backend developer."_Sl, Warning());
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_NE(ex3, ex3.cause());
        }

        TEST_F(ExceptionTest, constructorForZoneRulesExceptionClass) {
            ZoneRulesException ex2 = ZoneRulesException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            ZoneRulesException ex3 = ZoneRulesException("I am backend developer."_Sl, Warning());
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_NE(ex3, ex3.cause());
        }
    }

    namespace util {

        TEST_F(ExceptionTest, constructorForConcurrentModificationExceptionClass) {
            ConcurrentModificationException ex1 = ConcurrentModificationException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
            ConcurrentModificationException ex2 = ConcurrentModificationException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            ConcurrentModificationException ex3 = ConcurrentModificationException("I am backend developer."_Sl,
                                                                                  Warning());
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_NE(ex3, ex3.cause());
            ConcurrentModificationException ex4 = ConcurrentModificationException(Warning());
            ASSERT_FALSE(ex4.message().isEmpty());
            ASSERT_NE(ex4, ex4.cause());
        }

        TEST_F(ExceptionTest, constructorForMissingKeyExceptionClass) {
            MissingKeyException ex1 = MissingKeyException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
            MissingKeyException ex2 = MissingKeyException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            MissingKeyException ex3 = MissingKeyException("I am backend developer."_Sl, Warning());
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_NE(ex3, ex3.cause());
            MissingKeyException ex4 = MissingKeyException(null);
            ASSERT_FALSE(ex4.message().isEmpty());
            ASSERT_EQ(ex4, ex4.cause());
            MissingKeyException ex5 = MissingKeyException(null, Warning());
            ASSERT_FALSE(ex5.message().isEmpty());
            ASSERT_NE(ex5, ex5.cause());
        }

        TEST_F(ExceptionTest, constructorForMissingResourceExceptionClass) {
            MissingResourceException ex2 = MissingResourceException("I am developer."_Sl, "I am backend developer.",
                                                                    "lao");
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
        }

        TEST_F(ExceptionTest, constructorForNoSuchElementExceptionClass) {
            NoSuchElementException ex1 = NoSuchElementException();
            ASSERT_TRUE(ex1.message().isEmpty());
            ASSERT_EQ(ex1, ex1.cause());
            NoSuchElementException ex2 = NoSuchElementException("I am developer."_Sl);
            ASSERT_FALSE(ex2.message().isEmpty());
            ASSERT_EQ(ex2, ex2.cause());
            NoSuchElementException ex3 = NoSuchElementException("I am backend developer."_Sl, Warning());
            ASSERT_FALSE(ex3.message().isEmpty());
            ASSERT_NE(ex3, ex3.cause());
            NoSuchElementException ex4 = NoSuchElementException(Warning());
            ASSERT_FALSE(ex4.message().isEmpty());
            ASSERT_NE(ex4, ex4.cause());
        }
    }

    class ErrorTest : public ThrowableTest {
    protected:

        void SetUp() override {
            //
        }

        void TearDown() override {
            //
        }

    private:
        void TestBody() override = 0;
    };

    TEST_F(ErrorTest, constructorForErrorClass) {
        Error ex1 = Error();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        Error ex2 = Error("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        Error ex3 = Error("I am backend developer."_Sl, Warning());
        ASSERT_FALSE(ex3.message().isEmpty());
        ASSERT_NE(ex3, ex3.cause());
        Error ex4 = Error(Warning());
        ASSERT_FALSE(ex4.message().isEmpty());
        ASSERT_NE(ex4, ex4.cause());
    }

    TEST_F(ErrorTest, constructorForAssertionErrorClass) {
        AssertionError ex1 = AssertionError();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        AssertionError ex2 = AssertionError("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
        AssertionError ex3 = AssertionError("I am backend developer."_Sl, Warning());
        ASSERT_FALSE(ex3.message().isEmpty());
        ASSERT_NE(ex3, ex3.cause());
        AssertionError ex4 = AssertionError(Warning());
        ASSERT_FALSE(ex4.message().isEmpty());
        ASSERT_NE(ex4, ex4.cause());
    }

    TEST_F(ErrorTest, constructorForOutOfMemoryErrorClass) {
        OutOfMemoryError ex1 = OutOfMemoryError();
        ASSERT_TRUE(ex1.message().isEmpty());
        ASSERT_EQ(ex1, ex1.cause());
        OutOfMemoryError ex2 = OutOfMemoryError("I am developer."_Sl);
        ASSERT_FALSE(ex2.message().isEmpty());
        ASSERT_EQ(ex2, ex2.cause());
    }

    namespace charset {
        TEST_F(ErrorTest, constructorForCoderMalfunctionErrorClass) {
            CoderMalfunctionError ex4 = CoderMalfunctionError(Exception(Warning()));
            ASSERT_FALSE(ex4.message().isEmpty());
            ASSERT_NE(ex4, ex4.cause());
        }
    }

    namespace io {
        TEST_F(ErrorTest, constructorForIOErrorClass) {
            IOError ex4 = IOError(Warning());
            ASSERT_FALSE(ex4.message().isEmpty());
            ASSERT_NE(ex4, ex4.cause());
        }
    }
}
