#ifndef OCTARGS_ARGUMENT_TABLE_HPP_
#define OCTARGS_ARGUMENT_TABLE_HPP_

#include "basic_argument_table.hpp"
#include "basic_parser.hpp"
#include "basic_results.hpp"
#include "traits.hpp"

namespace oct
{
namespace args
{

using argument_table = basic_argument_table<traits<char>>;
using wargument_table = basic_argument_table<traits<wchar_t>>;

using results = basic_results<traits<char>>;
using wresults = basic_results<traits<wchar_t>>;

using parser = basic_parser<traits<char>>;
using wparser = basic_parser<traits<wchar_t>>;

} // namespace args
} // namespace oct

#endif /*OCTARGS_ARGUMENT_TABLE_HPP_*/
