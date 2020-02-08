#ifndef OCTARGS_BASIC_PARSER_DATA_HPP_
#define OCTARGS_BASIC_PARSER_DATA_HPP_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../basic_argument.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <class TRAITS>
struct basic_parser_data
{
    using char_type = typename TRAITS::char_type;
    using string_type = typename TRAITS::string_type;
    using string_vector_type = typename TRAITS::string_vector_type;

    using argument_type = basic_argument<TRAITS>;

    using argument_ptr_type = std::shared_ptr<argument_type>;

    basic_parser_data()
        : m_positional_arguments_enabled(false)
        , m_arguments()
        , m_names_repository()
    {
        // noop
    }

    bool m_positional_arguments_enabled;

    std::vector<argument_ptr_type> m_arguments;
    std::map<string_type, argument_ptr_type> m_names_repository;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_PARSER_DATA_HPP_*/
