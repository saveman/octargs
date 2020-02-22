#ifndef OCTARGS_PARSER_DATA_HPP_
#define OCTARGS_PARSER_DATA_HPP_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../argument.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T>
class basic_parser_data_iface
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using argument_tag_type = basic_argument_tag;
    using argument_tag_ptr_type = std::shared_ptr<argument_tag_type>;

    virtual ~basic_parser_data_iface() = default;
    virtual argument_tag_ptr_type find_argument(const string_type& arg_name) const = 0;
};

template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_parser_data : public basic_parser_data_iface<char_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_type = basic_argument<char_type, values_storage_type>;
    using argument_ptr_type = std::shared_ptr<argument_type>;

    using argument_tag_type = basic_argument_tag;
    using argument_tag_ptr_type = std::shared_ptr<argument_tag_type>;

    basic_parser_data()
        : m_named_arguments()
        , m_positional_arguments()
        , m_names_repository()
        , m_info_header()
        , m_info_footer()
    {
        // noop
    }

    argument_tag_ptr_type find_argument(const string_type& arg_name) const override
    {
        auto name_arg_iter = m_names_repository.find(arg_name);
        if (name_arg_iter != m_names_repository.end())
        {
            return name_arg_iter->second;
        }
        return nullptr;
    }

    std::vector<argument_ptr_type> m_named_arguments;
    std::vector<argument_ptr_type> m_positional_arguments;
    std::vector<argument_ptr_type> m_exclusive_arguments;
    std::map<string_type, argument_ptr_type> m_names_repository;
    string_type m_info_header;
    string_type m_info_footer;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_DATA_HPP_
