#ifndef OCTARGS_STRING_UTILS_HPP_
#define OCTARGS_STRING_UTILS_HPP_

#include <algorithm>

#include "char_utils.hpp"

namespace oct
{
namespace args
{
namespace internal
{
template <class TRAITS>
class string_utils
{
public:
    using char_type = typename TRAITS::char_type;
    using string_type = typename TRAITS::string_type;
    using char_utils_type = char_utils<char_type>;

    static inline void trim_lead(string_type& s)
    {
        auto is_space_pred = [](char_type c) { return !char_utils_type::is_space(c); };
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), is_space_pred));
    }

    static void trim_trail(string_type& s)
    {
        auto is_space_pred = [](char_type c) { return !char_utils_type::is_space(c); };
        s.erase(std::find_if(s.rbegin(), s.rend(), is_space_pred).base(), s.end());
    }

    static void trim(string_type& s)
    {
        trim_lead(s);
        trim_trail(s);
    }

    static string_type trim_lead_copy(string_type s)
    {
        trim_lead(s);
        return s;
    }

    static string_type trim_trail_copy(string_type s)
    {
        trim_trail(s);
        return s;
    }

    static string_type trim_copy(string_type s)
    {
        trim(s);
        return s;
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif /*OCTARGS_STRING_UTILS_HPP_*/
