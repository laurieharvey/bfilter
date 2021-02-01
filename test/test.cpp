#include "gtest/gtest.h"

#include "bfilter.h"
#include "utilities.h"

TEST(bfilter, contains)
{
    bloom::bfilter<std::string> bloom_filter(bloom::optimum_hash_number(100, 5), 100);

    bloom_filter.insert("abc");
    bloom_filter.insert("def");
    bloom_filter.insert("ghi");
    bloom_filter.insert("jkl");

    ASSERT_TRUE(bloom_filter.contains("abc"));
    ASSERT_TRUE(bloom_filter.contains("def"));
    ASSERT_TRUE(bloom_filter.contains("ghi"));
    ASSERT_TRUE(bloom_filter.contains("jkl"));
}