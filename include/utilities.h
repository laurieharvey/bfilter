#include <cmath>

namespace bloom
{
    template <typename CountType, typename SizeType>
    constexpr int optimum_hash_number(SizeType size, CountType expected_elements)
    {
        return std::round( ( size * std::log( 2 ) ) / expected_elements );
    }
} // namespace bloom