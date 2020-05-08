#ifndef OCTARGS_PARSER_DATA_HPP_
#define OCTARGS_PARSER_DATA_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../argument_group.hpp"
#include "argument.hpp"
#include "argument_repository.hpp"
#include "memory.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_parser_data : public enable_shared_from_this<basic_parser_data<char_T, values_storage_T>>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;
    using string_less_type = string_less<char_type>;

    using argument_repository_type = basic_argument_repository<char_type, values_storage_type>;
    using argument_repository_ptr_type = std::shared_ptr<argument_repository_type>;

    using argument_group_impl_type = basic_argument_group_impl<char_type, values_storage_type>;
    using argument_group_impl_ptr_type = std::shared_ptr<argument_group_impl_type>;
    using const_argument_group_impl_ptr_type = std::shared_ptr<const argument_group_impl_type>;

    using argument_group_type = basic_argument_group<char_type, values_storage_type>;

    using dictionary_type = dictionary<char_type>;
    using const_dictionary_ptr_type = std::shared_ptr<const dictionary_type>;

    using exclusive_argument_ptr_type = typename argument_repository_type::exclusive_argument_ptr_type;
    using switch_argument_ptr_type = typename argument_repository_type::switch_argument_ptr_type;
    using valued_argument_ptr_type = typename argument_repository_type::valued_argument_ptr_type;
    using positional_argument_ptr_type = typename argument_repository_type::positional_argument_ptr_type;
    using subparser_argument_ptr_type = typename argument_repository_type::subparser_argument_ptr_type;

    using parser_data_ptr_type = std::shared_ptr<basic_parser_data>;

    using parsers_map_type = std::map<string_type, parser_data_ptr_type, string_less_type>;

    argument_group_type add_group(const std::string& name)
    {
        auto argument_group_impl = std::make_shared<argument_group_impl_type>(this->weak_from_this(), name);

        m_argument_groups.emplace_back(argument_group_impl);

        return argument_group_type(argument_group_impl);
    }

    exclusive_argument_ptr_type add_exclusive(const string_vector_type& names)
    {
        return m_argument_repository->add_exclusive(this->weak_from_this(), names);
    }

    switch_argument_ptr_type add_switch(const string_vector_type& names)
    {
        return m_argument_repository->add_switch(this->weak_from_this(), names);
    }

    valued_argument_ptr_type add_valued(const string_vector_type& names)
    {
        return m_argument_repository->add_valued(this->weak_from_this(), names);
    }

    positional_argument_ptr_type add_positional(const string_type& name)
    {
        return m_argument_repository->add_positional(this->weak_from_this(), name);
    }

    subparser_argument_ptr_type add_subparsers(const string_type& name)
    {
        return m_argument_repository->add_subparsers(this->weak_from_this(), name);
    }

    parser_data_ptr_type add_subparser(const string_type& name)
    {
        if (has_subparser(name))
        {
            throw invalid_parser_name_ex<char_type>("Duplicated parser name", name);
        }

        auto subparser_data = create(m_dictionary);

        auto result = m_subparsers.emplace(name, subparser_data);
        if (!result.second)
        {
            throw std::runtime_error("Cannot emplace new parser");
        }

        return result.first->second;
    }

    bool has_subparser(const string_type& name) const
    {
        return m_subparsers.find(name) != m_subparsers.end();
    }

    parser_data_ptr_type get_subparser(const string_type& name) const
    {
        auto iter = m_subparsers.find(name);
        if (iter == m_subparsers.end())
        {
            throw std::logic_error("No parser with given name exist");
        }
        return iter->second;
    }

    const parsers_map_type& get_subparsers() const
    {
        return m_subparsers;
    }

    argument_group_impl_type& get_default_argument_group()
    {
        return *m_default_argument_group_ptr;
    }

    const argument_group_impl_type& get_default_argument_group() const
    {
        return *m_default_argument_group_ptr;
    }

    const_dictionary_ptr_type m_dictionary;
    argument_repository_ptr_type m_argument_repository;

    std::vector<const_argument_group_impl_ptr_type> m_argument_groups;
    string_type m_usage_oneliner;
    string_type m_usage_header;
    string_type m_usage_footer;

    static std::shared_ptr<basic_parser_data> create(const const_dictionary_ptr_type& dictionary)
    {
        struct create_enabler : public basic_parser_data
        {
            create_enabler(const const_dictionary_ptr_type& dictionary)
                : basic_parser_data(dictionary)
            {
                // noop
            }
        };

        auto new_data = std::make_shared<create_enabler>(dictionary);
        new_data->init();
        return new_data;
    }

private:
    basic_parser_data(const const_dictionary_ptr_type& dictionary)
        : m_dictionary(check_dictionary(dictionary))
        , m_argument_repository(std::make_shared<argument_repository_type>(m_dictionary))
        , m_argument_groups()
        , m_usage_oneliner()
        , m_usage_header()
        , m_usage_footer()
        , m_default_argument_group_ptr()
        , m_subparsers(string_less<char_type>(dictionary->is_case_sensitive()))
    {
        // noop
    }

    static const const_dictionary_ptr_type& check_dictionary(const const_dictionary_ptr_type& dictionary)
    {
        if (!dictionary)
        {
            throw std::invalid_argument("dictionary");
        }
        return dictionary;
    }

    void init()
    {
        m_default_argument_group_ptr
            = std::make_shared<argument_group_impl_type>(this->weak_from_this(), string_type());
    }

    argument_group_impl_ptr_type m_default_argument_group_ptr;
    parsers_map_type m_subparsers;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_DATA_HPP_
