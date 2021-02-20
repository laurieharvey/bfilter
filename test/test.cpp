#include <set>
#include <random>

#include "gtest/gtest.h"

#include "bfilter.h"
#include "utilities.h"

TEST(utilities, false_positive_probability)
{
    ASSERT_NEAR(bloom::false_positive_probability(10000, 1000, 7), 820e-5, 1e-5);
    ASSERT_NEAR(bloom::false_positive_probability(1000,  100 , 7), 821e-5, 1e-5);
    ASSERT_NEAR(bloom::false_positive_probability(100,   10,   7), 839e-5, 1e-5);

    ASSERT_NEAR(bloom::false_positive_probability(10000, 100,  7), 6e-9,   1e-9);
    ASSERT_NEAR(bloom::false_positive_probability(1000,  10,   7), 6e-9,   1e-9);
    ASSERT_NEAR(bloom::false_positive_probability(100,   1,    7), 6e-9,   1e-9);
}

TEST(utilities, required_filter_size)
{
    ASSERT_EQ(bloom::required_filter_size(1000, 7, 820e-5), 9999 );
    ASSERT_EQ(bloom::required_filter_size(100,  7, 821e-5), 1000 );
    ASSERT_EQ(bloom::required_filter_size(10,   7, 839e-5), 100  );

    ASSERT_EQ(bloom::required_filter_size(100,  7, 6e-9  ), 10109);
    ASSERT_EQ(bloom::required_filter_size(10,   7, 6e-9  ), 1011 );
    ASSERT_EQ(bloom::required_filter_size(1,    7, 6e-9  ), 102  );
}

TEST(utilities, optimum_hash_functions)
{
    ASSERT_EQ(bloom::optimum_hash_number(10000, 1000), 7);
    ASSERT_EQ(bloom::optimum_hash_number(1000,  100),  7);
    ASSERT_EQ(bloom::optimum_hash_number(100,   10),   7);

    ASSERT_EQ(bloom::optimum_hash_number(10000, 100), 69);
    ASSERT_EQ(bloom::optimum_hash_number(1000,  10),  69);
    ASSERT_EQ(bloom::optimum_hash_number(100,   1),   69);
}

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

TEST(bfilter, clear)
{
    bloom::bfilter<std::string> bloom_filter(bloom::optimum_hash_number(100, 5), 100);

    ASSERT_EQ(bloom_filter.contains("abc"), bloom::result::definitely_not);

    bloom_filter.insert("abc");

    ASSERT_EQ(bloom_filter.contains("abc"), bloom::result::possibly);

    bloom_filter.clear();

    ASSERT_EQ(bloom_filter.contains("abc"), bloom::result::definitely_not);
}

TEST(bfilter, false_probability)
{
    std::set<int> set;

    const int range = 1000000, k = 4, m = 100000, n = 20000;

    std::mt19937 urbg;
    std::uniform_int_distribution distribution(0, range);

    bloom::bfilter<int> bloom_filter(k, m);

    for (int i = 0; i < n;)
    {
        const int element = distribution(urbg);
        
        if (set.find(element) == std::cend(set))
        {
            ++i;
            set.insert(element);
            bloom_filter.insert(element);
        }
    }

    int positives = 0;

    for (int i = 0; i < range; ++i)
    {
        if (bloom_filter.contains(i) == bloom::result::possibly)
        {
            ++positives;
        }
    }

    const double false_probability = double(positives - n) / range;

    ASSERT_NEAR(false_probability, bloom::false_positive_probability(m, n, k), 0.01);
}