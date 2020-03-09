#ifndef OCTARGS_PARSER_DATA_HPP_
#define OCTARGS_PARSER_DATA_HPP_

#include <memory>
#include <string>
#include <vector>

#include "../argument.hpp"
#include "../argument_group.hpp"
#include "../exclusive_argument.hpp"
#include "../positional_argument.hpp"
#include "../subparser_argument.hpp"
#include "../valued_argument.hpp"
#include "argument_repository.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_parser_data
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;

    using argument_repository_type = basic_argument_repository<char_type, values_storage_type>;
    using argument_group_type = basic_argument_group<char_type, values_storage_type>;
    using argument_group_ptr_type = std::shared_ptr<argument_group_type>;
    using const_argument_group_ptr_type = std::shared_ptr<const argument_group_type>;

    basic_parser_data()
        : m_argument_repository()
        , m_default_argument_group(m_argument_repository, string_type()) // TODO: default name
        , m_argument_groups()
        , m_usage_oneliner()
        , m_usage_header()
        , m_usage_footer()
    {
        // noop
    }

    argument_group_type& add_group(const std::string& name)
    {
        auto argument_group = std::make_shared<argument_group_type>(m_argument_repository, name);

        m_argument_groups.push_back(argument_group);

        return *argument_group;
    }

    argument_repository_type m_argument_repository;
    argument_group_type m_default_argument_group;
    std::vector<const_argument_group_ptr_type> m_argument_groups;
    string_type m_usage_oneliner;
    string_type m_usage_header;
    string_type m_usage_footer;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_DATA_HPP_
