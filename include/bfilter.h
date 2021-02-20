#include <functional>
#include <vector>
#include <algorithm>
#include<random>

namespace bloom
{
    template <class T>
    struct combined_hash
    {
        std::size_t seed;

        inline std::size_t operator()(const T &v)
        {
            return seed ^ std::hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        }
    };

    enum class result
    {
        definitely_not,
        possibly
    };

    template <typename Value>
    class bfilter
    {
        std::vector<bool> filter_;
        std::vector<std::size_t> seeds_;

    public:
        using value_type = Value;
        using size_type = std::vector<bool>::size_type;
        /*
         * Construct the filter with a number of hashes and size
         */
        explicit bfilter(int hashes, size_type size)
            : filter_(size, false),
              seeds_(hashes)
        {
            std::mt19937 urbg;
            std::uniform_int_distribution<std::size_t> distrib(std::numeric_limits<std::size_t>::min(),
                                                               std::numeric_limits<std::size_t>::max());

            for (int i = 0; i < hashes; i++)
            {
                seeds_.push_back(distrib(urbg));
            }
        }
        /*
         * Insert value into the filter
         */
        void insert(const value_type &value)
        {
            for (const auto seed : seeds_)
            {
                filter_[combined_hash<value_type>{seed}(value) % filter_.size()] = true;
            }
        }
        /*
         * Insert value into the filter
         */
        void insert(value_type &&value)
        {
            for (const auto seed : seeds_)
            {
                filter_[combined_hash<value_type>{seed}(value) % filter_.size()] = true;
            }
        }
        /*
         * Check whether the filter possibly contains value or whether
         * it definitely doesn't
         */
        result contains(const value_type &value) const
        {
            for (const auto seed : seeds_)
            {
                if (!filter_[combined_hash<value_type>{seed}(value) % filter_.size()])
                {
                    return result::definitely_not;
                }
            }
            return result::possibly;
        }
        /*
         * Clear the filter of all elements
         */
        void clear()
        {
            std::fill(std::begin(filter_), std::end(filter_), false);
        }
    };
} // namespace bloom
