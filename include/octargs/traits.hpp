#ifndef OCTARGS_TRAITS_HPP_
#define OCTARGS_TRAITS_HPP_

#include <string>
#include <vector>

namespace oct
{
namespace args
{
template <class CHAR_TYPE>
class traits
{
    // nothing
};

template <>
class traits<char>
{
public:
    using char_type = char;
    using string_type = std::string;
    using string_vector_type = std::vector<string_type>;
};

template <>
class traits<wchar_t>
{
public:
    using char_type = wchar_t;
    using string_type = std::wstring;
    using string_vector_type = std::vector<string_type>;
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_TRAITS_HPP_*/
