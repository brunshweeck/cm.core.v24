//
// Created by brunshweeck on 20/11/24.
//

#ifndef CORE24_PATTERN_SELF_H
#define CORE24_PATTERN_SELF_H

#include <core/atomic/AtomicInteger.h>
#include <core/regex/PatternSyntaxException.h>
#include <core/util/List.h>
#include <core/util/Map.h>
#include <meta/regex/Pattern.Node.h>
#include <meta/regex/Pattern.Predicates.h>

namespace core {
    namespace regex {
        class Pattern::Self final : public Object {
        public:
            gint flags = 0;
            gbool compiled = false;
            String pattern = {};
            Optional<Node> root = {};
            Optional<Node> matchRoot = {};
            IntArray buffer = {};
            Optional<CharPredicate> predicate = {};
            Optional<Map<String, Integer> > namedGroups = {};
            Array<GroupHead> groupNodes = {};
            Optional<List<Node> > topClosureNodes = {};
            gint localTCNCount = 0;
            gbool hasGroupRef = false;
            mutable IntArray temp = {};
            gint capturingGroupCount = 0;
            gint localCount = 0;
            gint position = 0;
            gint patternLength = 0;
            gbool hasSupplementary = false;

            static String normalize(String const &pattern);

            static void normalizeSlice(String const &src, gint off, gint limit, XString &dst);

            static void normalizeClazz(String const &src, gint off, gint limit, XString &dst);

            static void produceEquivalentAlternation(String const &input, Set<String> &dst);

            static StringArray producePermutations(String const &input);

            static gint getClass(gint c);

            static String composeOneStep(String const &input);

            void RemoveQEQuoting();

            void compile(String const &regex);

            Optional<Map<String, Integer> > namedGroupsMap();

            gbool has(gint f) const;

            void accept(gint ch, String const &s);

            void mark(gint c);

            gint peek();

            gint read();

            gint readEscaped();

            gint next();

            gint nextEscaped();

            gint peekPastWhitespace(gint ch);

            gint parsePastWhitespace(gint ch);

            gint parsePastLine();

            gint peekPastLine();

            gbool isLineSeparator(gint ch) const;

            gint skip();

            void unread();

            PatternSyntaxException error(String const &s) const;

            gbool findSupplementary(gint start, gint end);

            static gbool isSupplementary(gint ch);

            Optional<Node> expr(const Optional<Node> &end);

            Optional<Node> sequence(const Optional<Node> &end);

            Optional<Node> atom();

            void append(gint ch, gint index);

            Optional<Node> ref(gint refNum);

            gint escape(gbool inclass, gbool create, gbool isrange);

            Optional<CharPredicate> clazz(gbool consume);

            Optional<CharPredicate> bitsOrSingle(Optional<BitClass> bits, gint ch) const;

            Optional<CharPredicate> single(gint ch) const;

            Optional<CharPredicate> range(const Optional<BitClass>& bits);

            Optional<CharPredicate> family(gbool singleLetter, gbool isComplement);

            Optional<CharProperty> newCharProperty(const Optional<CharPredicate>& p);

            String groupname(gint ch);

            Optional<Node> group0();

            Optional<Node> createGroup(gbool anonymous);

            void addFlag();

            void subFlag();

            Qtype qtype();

            Optional<Node> curly(const Optional<Node>& prev, gint cmin);

            Optional<Node> closure(Optional<Node> prev);

            gint c();

            gint o();

            gint x();

            gint cursor() const;

            void setcursor(gint pos);

            gint uxxxx();

            gint u();

            gint N();

            static gint countChars(CharSequence const &seq, gint index, gint lengthInCodePoints);

            static gint countCodePoints(CharSequence const &seq);

            Optional<Node> newSlice(IntArray buff, gint count, gbool hasSupplemenatry) const;

            void clear();

            static Optional<CharPredicate> VertWS();

            static Optional<CharPredicate> HorizWS();

            static Optional<CharPredicate> ALL();

            static Optional<CharPredicate> DOT();

            static Optional<CharPredicate> UNIXDOT();

            static Optional<CharPredicate> SingleS(gint c);

            static Optional<CharPredicate> Single(gint c);

            static Optional<CharPredicate> SingleI(gint lower, gint upper);

            static Optional<CharPredicate> SingleU(gint lower);

            static gbool inRange(gint lower, gint ch, gint upper);

            static Optional<CharPredicate> Range(gint lower, gint upper);

            static Optional<CharPredicate> CIRange(gint lower, gint upper);

            static Optional<CharPredicate> CIRangeU(gint lower, gint upper);

            ~Self() override;

            atomic::AtomicInteger state;
        };
    }
}

#endif //CORE24_PATTERN_SELF_H
