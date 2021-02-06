#include "gtest/gtest.h"

#include "bfilter.h"
#include "utilities.h"

TEST(bfilter, contains)
{
    bloom::bfilter<std::string> bloom_filter(bloom::optimum_hash_number(100, 5), 100);

    ASSERT_EQ(bloom_filter.contains("xyz"), bloom::result::definitely_not);

    bloom_filter.insert("abc");
    bloom_filter.insert("def");
    bloom_filter.insert("ghi");
    bloom_filter.insert("jkl");

    ASSERT_EQ(bloom_filter.contains("abc"), bloom::result::possibly);
    ASSERT_EQ(bloom_filter.contains("def"), bloom::result::possibly);
    ASSERT_EQ(bloom_filter.contains("ghi"), bloom::result::possibly);
    ASSERT_EQ(bloom_filter.contains("jkl"), bloom::result::possibly);
}