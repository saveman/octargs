#ifndef OCTARGS_OCTARGS_HPP_
#define OCTARGS_OCTARGS_HPP_

#include "argument_table.hpp"
#include "parser.hpp"
#include "results.hpp"
#include "traits.hpp"

namespace oct
{
namespace args
{

using argument_table = basic_argument_table<traits<char>>;
using wargument_table = basic_argument_table<traits<wchar_t>>;

using argument_table_iterator = basic_argument_table_iterator<traits<char>>;
using wargument_table_iterator = basic_argument_table_iterator<traits<wchar_t>>;

using results = basic_results<traits<char>>;
using wresults = basic_results<traits<wchar_t>>;

template <typename VALUES_STORAGE = internal::null_values_storage>
using storing_parser = basic_parser<traits<char>, VALUES_STORAGE>;

using parser = storing_parser<>;

template <typename VALUES_STORAGE = internal::null_values_storage>
using storing_wparser = basic_parser<traits<wchar_t>, VALUES_STORAGE>;

using wparser = storing_parser<>;

} // namespace args
} // namespace oct

#endif /*OCTARGS_OCTARGS_HPP_*/
