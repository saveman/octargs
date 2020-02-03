#ifndef OCTARGS_ARGUMENT_TABLE_HPP_
#define OCTARGS_ARGUMENT_TABLE_HPP_

#include "basic_argument_table.hpp"
#include "traits.hpp"

namespace oct
{
namespace args
{

    using argument_table = basic_argument_table<char_traits>;
    using wargument_table = basic_argument_table<wchar_traits>;

} // namespace args
} // namespace oct

#endif /*OCTARGS_ARGUMENT_TABLE_HPP_*/
