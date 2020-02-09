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
template <class CHAR_TYPE>
class char_utils
{
};

template <>
class char_utils<char>
{
public:
    using char_type = char;

    static bool is_space(char_type c)
    {
        return std::isspace(cast(c));
    }

private:
    static int cast(char_type c)
    {
        return static_cast<unsigned char>(c);
    }
};

template <>
class char_utils<wchar_t>
{
public:
    using char_type = wchar_t;

    static bool is_space(char_type c)
    {
        return std::iswspace(cast(c));
    }

private:
    static wint_t cast(char_type c)
    {
        return static_cast<wint_t>(c);
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif /*OCTARGS_CHAR_UTILS_HPP_*/
