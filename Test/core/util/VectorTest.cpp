//
// This File has been Created by CMake For Testing in Project CORE-24
//
#include "VectorTest.h"

TEST_F(VectorTest, try_add_items) {
    ASSERT_NO_THROW(cobaye.add(1));
    ASSERT_NO_THROW(cobaye.add(12));
    ASSERT_NO_THROW(cobaye.add(123));
    ASSERT_NO_THROW(cobaye.add(1234));
    ASSERT_NO_THROW(cobaye.add(12345));
    ASSERT_NO_THROW(cobaye.add(123456));
    ASSERT_NO_THROW(cobaye.add(1234567));
    ASSERT_NO_THROW(cobaye.add(12345678));
    ASSERT_NO_THROW(cobaye.add(123456789));
}

TEST_F(VectorTest, try_get_item_at_index) {
    ASSERT_NO_THROW(cobaye.get(4));
}

TEST_F(VectorTest, try_get_size) {
    ASSERT_NO_THROW(cobaye.size());
}

TEST_F(VectorTest, try_remove_item_at_index) {
    ASSERT_NO_THROW(cobaye.remove(4));
}

TEST_F(VectorTest, try_remove_item) {
    ASSERT_NO_THROW(cobaye.remove((Integer)4));
}

TEST_F(VectorTest, try_iterate_all_items) {
    ASSERT_NO_THROW(iterateAndDoNothing(cobaye));
}

TEST_F(VectorTest, try_iterate_while_deleting_items) {
    ASSERT_THROW(iterateAndRemoveAll(cobaye), ConcurrentModificationException);
}

