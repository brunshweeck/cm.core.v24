//
// Created by brunshweeck on 20/11/24.
//

#ifndef CORE24_PATTERN_NODE_H
#define CORE24_PATTERN_NODE_H

#include <core/Array.h>
#include <meta/regex/Pattern.Predicates.h>

namespace core {
    using namespace util;
    using namespace function;
    using namespace text;

    namespace regex {
        CORE_ALIAS(UNSAFE, misc::Unsafe);

        enum class Qtype { GREEDY, LAZY, POSSESSIVE, INDEPENDENT };

        class Pattern::TreeInfo final : public Object {
        public:
            gint minLength = { };
            gint maxLength = { };
            gbool maxValid = { };
            gbool deterministic = { };

            TreeInfo();

            void reset();
        };

        class Pattern::Analyzer : public Object {
        public:
            Analyzer() = default;

            virtual gbool match(Matcher& matcher, gint i, CharSequence const& seq) = 0;

            virtual gbool study(TreeInfo& info) = 0;

            Object& clone() const override;
        };

        class Pattern::Node : public Analyzer {
        public:
            Optional<Analyzer> next;

            Node();

            gbool match(Matcher& matcher, gint i, CharSequence const& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::LastNode final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Start : public Node {
        public:
            gint minLength = { };

            CORE_EXPLICIT Start(const Optional<Node>& node);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::StartS final : public Start {
        public:
            CORE_EXPLICIT StartS(const Optional<Node>& node);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Begin final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::End final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Caret final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::UnixCaret final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::LastMatch final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Dollar final : public Node {
        public:
            gbool multiline;

            CORE_EXPLICIT Dollar(gbool multiline);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::UnixDollar final : public Node {
            gbool multiline;

        public:
            CORE_EXPLICIT UnixDollar(gbool multiline);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::LineEnding final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::CharProperty : public Node {
        public:
            Optional<CharPredicate> predicate;

            CORE_EXPLICIT CharProperty(const Optional<CharPredicate>& predicate);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::BmpCharProperty final : public CharProperty {
        public:
            CORE_EXPLICIT BmpCharProperty(const Optional<CharPredicate>& predicate);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::NFCCharProperty final : public Node {
        public:
            Optional<CharPredicate> predicate;

            CORE_EXPLICIT NFCCharProperty(const Optional<CharPredicate>& predicate);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::XGrapheme final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::GraphemeBound final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::SliceNode : public Node {
        public:
            IntArray buffer;

            CORE_EXPLICIT SliceNode(IntArray buffer);

            gbool study(TreeInfo& info) override;
        };

        class Pattern::Slice : public SliceNode {
        public:
            CORE_EXPLICIT Slice(const IntArray& buffer);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::SliceI final : public SliceNode {
        public:
            CORE_EXPLICIT SliceI(const IntArray& buffer);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::SliceU final : public SliceNode {
        public:
            CORE_EXPLICIT SliceU(const IntArray& buffer);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::SliceS final : public Slice {
        public:
            CORE_EXPLICIT SliceS(const IntArray& buffer);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::SliceIS : public SliceNode {
        public:
            CORE_EXPLICIT SliceIS(const IntArray& buffer);

            virtual gint toLower(gint c);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::SliceUS final : public SliceIS {
        public:
            CORE_EXPLICIT SliceUS(const IntArray& buffer);

            gint toLower(gint c) override;
        };

        class Pattern::Ques final : public Node {
        public:
            Optional<Node> atom;
            Qtype type;

            Ques(const Optional<Node>& atom, Qtype type);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::CharPropertyGreedy : public Node {
        public:
            Optional<CharPredicate> predicate;
            gint cmin = 0;

            CharPropertyGreedy(const Optional<CharProperty>& cp, gint cmin);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::BmpCharPropertyGreedy final : public CharPropertyGreedy {
        public:
            BmpCharPropertyGreedy(const Optional<BmpCharProperty>& predicate, gint cmin);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Curly final : public Node {
        public:
            Optional<Node> atom;
            Qtype type;
            gint cmin = 0;
            gint cmax = 0;

            Curly(const Optional<Node>& atom, gint cmin, gint cmax, Qtype type);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            // Greedy match.
            // 'i' is the index to start matching at
            // 'j' is the number of atoms that have matched
            gbool match0(Matcher& matcher, gint i, gint j, const CharSequence& seq);

            // Reluctant match. At this point, the minimum has been satisfied.
            // 'i' is the index to start matching at
            // 'j' is the number of atoms that have matched
            gbool match1(Matcher& matcher, gint i, gint j, const CharSequence& seq);

            gbool match2(Matcher& matcher, gint i, gint j, const CharSequence& seq);

            gbool study(TreeInfo& info) override;
        };

        class Pattern::GroupCurly final : public Node {
        public:
            Optional<Node> atom;
            Qtype type;
            gint cmin = 0;
            gint cmax = 0;
            gint localIndex = 0;
            gint groupIndex = 0;
            gbool capture = false;

            GroupCurly(const Optional<Node>& atom, gint cmin, gint cmax, Qtype type,
                       gint localIndex, gint groupIndex, gbool capture);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            // Aggressive group match
            gbool match0(Matcher& matcher, gint i, gint j, const CharSequence& seq);

            // Reluctant matching
            gbool match1(Matcher& matcher, gint i, gint j, const CharSequence& seq);

            // Possessive matching
            gbool match2(Matcher& matcher, gint i, gint j, const CharSequence& seq);

            gbool study(TreeInfo& info) override;
        };

        class Pattern::BranchConn final : public Node {
        public:
            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::Branch final : public Node {
        public:
            Array<Node> atoms;
            gint size = 2;
            Optional<Node> conn;

            Branch(Optional<Node> first, Optional<Node> second, const Optional<Node>& branchConn);

            void addNode(Optional<Node> node);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::GroupTail final : public Node {
        public:
            gint localIndex;
            gint groupIndex;

            GroupTail(gint localCount, gint groupCount);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::GroupHead final : public Node {
        public:
            gint localIndex;
            Optional<GroupTail> tail;

            CORE_EXPLICIT GroupHead(gint localCount);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Loop : public Node {
        public:
            Optional<Node> body;
            gint countIndex; // local count index in matcher locals
            gint beginIndex; // group beginning index
            gint cmin = 0, cmax = 0;
            gint posIndex;

            Loop(gint countIndex, gint beginIndex);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            virtual gbool matchInit(Matcher& matcher, gint i, const CharSequence& seq);

            gbool study(TreeInfo& info) override;
        };

        class Pattern::Prolog final : public Node {
        public:
            Optional<Loop> loop;

            CORE_EXPLICIT Prolog(const Optional<Loop>& loop);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::LazyLoop final : public Loop {
        public:
            LazyLoop(gint countIndex, gint beginIndex);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool matchInit(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::BackRef final : public Node {
        public:
            gint groupIndex;

            CORE_EXPLICIT BackRef(gint groupCount);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::CIBackRef final : public Node {
        public:
            gint groupIndex;
            gbool doUnicodeCase;

            CIBackRef(gint groupCount, gint doUnicodeCase);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::First final : public Node {
        public:
            Optional<Node> atom;

            CORE_EXPLICIT First(const Optional<Node>& atom);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::Pos final : public Node {
        public:
            Optional<Node> cond;

            CORE_EXPLICIT Pos(const Optional<Node>& cond);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Neg final : public Node {
        public:
            Optional<Node> cond;

            CORE_EXPLICIT Neg(const Optional<Node>& cond);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::LookBehindEndNode final : public Node {
        public:
            static LookBehindEndNode& INSTANCE;

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Behind : public Node {
        public:
            Optional<Node> cond;
            gint rmax, rmin;

            Behind(const Optional<Node>& cond, gint rmax, gint rmin);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::BehindS final : public Behind {
        public:
            BehindS(const Optional<Node>& cond, gint rmax, gint rmin);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::NotBehind : public Node {
        public:
            Optional<Node> cond;
            gint rmax, rmin;

            NotBehind(const Optional<Node>& cond, gint rmax, gint rmin);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::NotBehindS final : public NotBehind {
        public:
            NotBehindS(const Optional<Node>& cond, gint rmax, gint rmin);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::Bound final : public Node {
        public:
            enum {
                LEFT = 1, RIGHT = 2, BOTH = 3, NONE = 4
            };

            gint type;
            gbool useUWORD;

            Bound(gint type, gbool useUWORD);

            gbool isWord(gint ch) const;

            static gbool hasBaseCharacter(Matcher& matcher, gint i, const CharSequence& seq);

            gint check(Matcher& matcher, gint i, const CharSequence& seq) const;

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };

        class Pattern::BnM : public Node {
        public:
            IntArray buffer;
            IntArray lastOcc;
            IntArray optoSft;

            /**
             * Pre-calculates arrays needed to generate the bad character
             * shift and the good suffix shift. Only the last seven bits
             * are used to see if chars match; This keeps the tables small
             * and covers the heavily used ASCII range, but occasionally
             * results in an aliased match for the bad character shift.
             */
            static Optional<Node> optimize(Optional<Node> node);

            BnM(IntArray buffer, IntArray lastOcc, IntArray optoSft, Optional<Node> next);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;

            gbool study(TreeInfo& info) override;
        };

        class Pattern::BnMS final : public BnM {
            gint lengthInChars;

        public:
            BnMS(const IntArray& buffer, const IntArray& lastOcc, const IntArray& optoSft, const Optional<Node>& next);

            gbool match(Matcher& matcher, gint i, const CharSequence& seq) override;
        };
    } // regex
} // core

#endif //CORE24_PATTERN_NODE_H
