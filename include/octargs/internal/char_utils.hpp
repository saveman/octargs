#ifndef OCTARGS_CHAR_UTILS_HPP_
#define OCTARGS_CHAR_UTILS_HPP_

#include <cctype>
#include <cwctype>

namespace oct
{
namespace args
{
namespace internal
{

inline bool is_space(char c)
{
    return std::isspace(static_cast<unsigned char>(c));
}

inline bool is_space(wchar_t c)
{
    return std::iswspace(static_cast<wint_t>(c));
}

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_CHAR_UTILS_HPP_
