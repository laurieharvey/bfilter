#include <cmath>

namespace bloom
{
    /*
     * Return the false positive probability for a given filter of size m, containing n 
     * elements and using k independent hash functions
     */
    template <typename SizeType, typename HashNumberType>
    constexpr double false_positive_probability(SizeType m, SizeType n, HashNumberType k)
    {
        return std::pow(1 - std::pow(1 - 1 / m, k * n), k);
    }
    /*
     * Return the number of independent hash functions k which minimise the false
     * probability for expected element count n and filter size m
     */
    template <typename SizeType, typename HashNumberType>
    constexpr HashNumberType optimum_hash_number(SizeType m, SizeType n)
    {
        auto k = (m * std::log(2)) / n;

        return false_positive_probability(m, n, std::ceil(k)) < false_positive_probability(m, n, std::floor(k))
                   ? std::ceil(k)
                   : std::floor(k);
    }
    /*
     * Return the required filter size m for expected element count n and desired false
     * probability e
     */
    template <typename SizeType>
    constexpr SizeType required_filter_size(SizeType n, double e)
    {
        return -(n * std::log(e) / std::pow(std::log(2), 2));
    }
} // namespace bloom