//
// Created by admin on 23/01/25.
//

#include <core/util/ArrayList.h>
#include <core/util/Optional.h>
#include <core/util/Vector.h>
#include <gtest/gtest.h>

namespace core {
    using misc::Unsafe;

    namespace util {

        template <class E>
        class CollectionsTest : public testing::Test {
        protected:
            void SetUp() override {
                // Initialize Collections
                colors = ArrayList<E>(5);
            }

            void TearDown() override {}

        public:
            ArrayList<E> colors;
        };

        class StringCollectionsTest : public CollectionsTest<String> {
        protected:
            void SetUp() override {
                CollectionsTest::SetUp();
                colors.add("Red"_Sl);
                colors.add("Green"_Sl);
                colors.add("Blue"_Sl);
                colors.add("Orange"_Sl);
                colors.add("Cyan"_Sl);
                colors.add("Purple"_Sl);
                colors.add("Yellow"_Sl);
                colors.add("White"_Sl);
                colors.add("Black"_Sl);
                colors.add("Gray"_Sl);
                colors.add("Grey"_Sl);
                colors.add("Violet"_Sl);
                colors.add("Brown"_Sl);
                colors.add("Orchid"_Sl);
                colors.add("Pink"_Sl);
                colors.add("Whitesmoke"_Sl);
                colors.add("DarkGray"_Sl);
                colors.add("LightGray"_Sl);
            }

            void TearDown() override {
                CollectionsTest::TearDown();
                for (gint i = colors.size() - 1; i >= 0; --i) {
                    auto& color = colors.get(i);
                    Unsafe::deleteRegInstance(color);
                }
                colors.clear();
            }
        };

        TEST_F(StringCollectionsTest, IterateWithJavaIterator) {
            gint index = 0;
            for (Iterator<String>& it = colors.iterator(); it.hasNext();) {
                auto& nextColor = it.next();
                auto& iColor = colors.get(index++);
                ASSERT_EQ(iColor, nextColor);
            }
        }

        TEST_F(StringCollectionsTest, IterateWithCIterator) {
            gint index = 0;
            for (auto& nextColor : colors) {
                auto& iColor = colors.get(index++);
                ASSERT_EQ(iColor, nextColor);
            }
        }

        TEST_F(StringCollectionsTest, StructuralModificationDuringJavaIteration) {
            for (Iterator<String>& it = colors.iterator(); it.hasNext();) {
                colors.add("Unknown Color"_Sl);
                ASSERT_THROW(it.next(), ConcurrentModificationException);
                break;
            }
        }

        TEST_F(StringCollectionsTest, StructuralModificationDuringCIteration) {
            ASSERT_THROW(
                { for (auto& nextColor : colors) colors.add("Unknown Color"_Sl); },
                ConcurrentModificationException);
        }
    }
}
