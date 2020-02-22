#ifndef OCTARGS_OCTARGS_HPP_
#define OCTARGS_OCTARGS_HPP_

#include "argument_table.hpp"
#include "parser.hpp"
#include "results.hpp"

namespace oct
{
namespace args
{

using argument_table = basic_argument_table<char>;
using wargument_table = basic_argument_table<wchar_t>;

using argument_table_iterator = basic_argument_table_iterator<char>;
using wargument_table_iterator = basic_argument_table_iterator<wchar_t>;

using results = basic_results<char>;
using wresults = basic_results<wchar_t>;

using parser = basic_parser<char>;
using wparser = basic_parser<wchar_t>;

template <typename VALUES_STORAGE>
using storing_parser = basic_parser<char, VALUES_STORAGE>;

template <typename VALUES_STORAGE>
using wstoring_parser = basic_parser<wchar_t, VALUES_STORAGE>;

} // namespace args
} // namespace oct

#endif // OCTARGS_OCTARGS_HPP_
