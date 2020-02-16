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

template <typename STRING>
inline void trim_lead(STRING& s)
{
    using char_type = typename STRING::value_type;

    auto is_space_pred = [](char_type c) { return !is_space(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), is_space_pred));
}

template <typename STRING>
inline void trim_trail(STRING& s)
{
    using char_type = typename STRING::value_type;

    auto is_space_pred = [](char_type c) { return !is_space(c); };
    s.erase(std::find_if(s.rbegin(), s.rend(), is_space_pred).base(), s.end());
}

template <typename STRING>
inline void trim(STRING& s)
{
    trim_lead(s);
    trim_trail(s);
}

template <typename STRING>
inline STRING trim_lead_copy(STRING s)
{
    trim_lead(s);
    return s;
}

template <typename STRING>
inline STRING trim_trail_copy(STRING s)
{
    trim_trail(s);
    return s;
}

template <typename STRING>
inline STRING trim_copy(STRING s)
{
    trim(s);
    return s;
}

} // namespace internal
} // namespace args
} // namespace oct

#endif /*OCTARGS_STRING_UTILS_HPP_*/
