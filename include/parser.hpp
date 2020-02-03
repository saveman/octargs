#ifndef OCTARGS_PARSER_HPP_
#define OCTARGS_PARSER_HPP_

#include "basic_parser.hpp"
#include "traits.hpp"

namespace oct
{
namespace args
{

    using parser = basic_parser<char_traits>;
    using wparser = basic_parser<wchar_traits>;

} // namespace args
} // namespace oct

#endif /*OCTARGS_PARSER_HPP_*/
