#include <cmath>

namespace bloom
{
    /*
     * Return the false positive probability for a given filter of size m, containing n 
     * elements and using k independent hash functions
     */
    template <typename SizeType>
    constexpr double false_positive_probability(SizeType m, SizeType n, int k)
    {
        return std::pow(1 - std::pow(1 - 1.0 / m, k * n), k);
    }
    /*
     * Return the required filter size for a given false probability e, n expected elements
     * and k independent hash functions
     */
    template <typename SizeType>
    constexpr SizeType required_filter_size(SizeType n, int k, double e)
    {
        return std::round(1.0 / (1 - std::pow((1 - std::pow(e, 1.0 / k)), 1.0 / (k * n))));
    }
    /*
     * Return the required filter size for a given false probability e, n expected elements
     * and k equal to the optimum number of independent hash functions
     */
    template <typename SizeType>
    constexpr SizeType required_filter_size(SizeType n, double e)
    {
        return -(n * std::log(e) / std::pow(std::log(2), 2));
    }
    /*
     * Return the number of independent hash functions k which minimise the false
     * probability for expected element count n and filter size m
     */
    template <typename SizeType>
    constexpr int optimum_hash_number(SizeType m, SizeType n)
    {
        auto k = (m * std::log(2)) / n;

        return false_positive_probability(m, n, std::ceil(k)) < false_positive_probability(m, n, std::floor(k))
                   ? std::ceil(k)
                   : std::floor(k);
    }
} // namespace bloom