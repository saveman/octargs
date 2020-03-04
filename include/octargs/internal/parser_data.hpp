#ifndef OCTARGS_PARSER_DATA_HPP_
#define OCTARGS_PARSER_DATA_HPP_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../argument.hpp"
#include "../subparser_argument.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_parser_data
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_type = basic_argument<char_type, values_storage_type>;
    using argument_ptr_type = std::shared_ptr<argument_type>;

    using subparser_argument_type = basic_subparser_argument<char_type, values_storage_type>;
    using subparser_argument_ptr_type = std::shared_ptr<subparser_argument_type>;

    using argument_tag_type = basic_argument_tag;
    using argument_tag_ptr_type = std::shared_ptr<argument_tag_type>;

    using basic_parser_data_ptr = std::shared_ptr<basic_parser_data>;

    basic_parser_data()
        : m_exclusive_arguments()
        , m_named_arguments()
        , m_positional_arguments()
        , m_subparsers_argument()
        , m_names_repository()
        , m_usage_oneliner()
        , m_usage_header()
        , m_usage_footer()
    {
        // noop
    }

    std::vector<argument_ptr_type> m_exclusive_arguments;
    std::vector<argument_ptr_type> m_named_arguments;
    std::vector<argument_ptr_type> m_positional_arguments;
    subparser_argument_ptr_type m_subparsers_argument;
    std::map<string_type, argument_ptr_type> m_names_repository;
    string_type m_usage_oneliner;
    string_type m_usage_header;
    string_type m_usage_footer;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_DATA_HPP_
