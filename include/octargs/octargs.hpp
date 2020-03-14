#ifndef OCTARGS_OCTARGS_HPP_
#define OCTARGS_OCTARGS_HPP_

#include "argument_table.hpp"
#include "parser.hpp"
#include "results.hpp"

namespace oct
{
namespace args
{

/// \brief Argument table (for char/string)
using argument_table = basic_argument_table<char>;

/// \brief Argument table (for wchar_t/wstring)
using wargument_table = basic_argument_table<wchar_t>;

/// \brief Argument table iterator (for char/string)
using argument_table_iterator = basic_argument_table_iterator<char>;

/// \brief Argument table iterator (for wchar_t/wstring)
using wargument_table_iterator = basic_argument_table_iterator<wchar_t>;

/// \brief Argument parsing results (for char/string)
using results = basic_results<char>;

/// \brief Argument parsing results (for wchar_t/wstring)
using wresults = basic_results<wchar_t>;

/// \brief Parser (for char/string)
using parser = basic_parser<char>;

/// \brief Parser (for wchar_t/wstring)
using wparser = basic_parser<wchar_t>;

/// \brief Parser with values storage (for char/string)
template <typename values_storage_T>
using storing_parser = basic_parser<char, values_storage_T>;

/// \brief Parser with values storage (for wchar_t/wstring)
template <typename values_storage_T>
using wstoring_parser = basic_parser<wchar_t, values_storage_T>;

} // namespace args
} // namespace oct

#endif // OCTARGS_OCTARGS_HPP_
