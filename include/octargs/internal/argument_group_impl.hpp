#ifndef OCTARGS_ARGUMENT_GROUP_IMPL_HPP_
#define OCTARGS_ARGUMENT_GROUP_IMPL_HPP_

#include <exception>
#include <memory>
#include <string>
#include <vector>

#include "../exclusive_argument.hpp"
#include "../positional_argument.hpp"
#include "../subparser_argument.hpp"
#include "../switch_argument.hpp"
#include "../valued_argument.hpp"
#include "argument.hpp"
#include "argument_repository.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_parser_data;

template <typename char_T, typename values_storage_T>
class basic_argument_group_impl
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_type = basic_argument<char_type, values_storage_type>;
    using const_argument_ptr_type = std::shared_ptr<const argument_type>;

    using exclusive_argument_type = basic_exclusive_argument<char_type, values_storage_type>;
    using positional_argument_type = basic_positional_argument<char_type, values_storage_type>;
    using switch_argument_type = basic_switch_argument<char_type, values_storage_type>;
    using valued_argument_type = basic_valued_argument<char_type, values_storage_type>;

    using argument_repository_type = basic_argument_repository<char_type, values_storage_type>;
    using argument_repository_ptr_type = std::shared_ptr<argument_repository_type>;

    using parser_data_type = basic_parser_data<char_type, values_storage_type>;
    using parser_data_ptr_type = std::shared_ptr<parser_data_type>;
    using parser_data_weak_ptr_type = std::weak_ptr<parser_data_type>;

    basic_argument_group_impl(parser_data_weak_ptr_type parser_data_ptr, const string_type& name)
        : m_parser_data_ptr(parser_data_ptr)
        , m_name(name)
        , m_description()
        , m_arguments()
    {
        // noop
    }

    const string_type& get_name() const
    {
        return m_name;
    }

    const string_type& get_description() const
    {
        return m_description;
    }

    basic_argument_group_impl& set_description(const string_type& description)
    {
        m_description = description;
        return *this;
    }

    exclusive_argument_type add_exclusive(const string_vector_type& names)
    {
        auto argument_ptr = get_parser_data()->add_exclusive(names);
        m_arguments.emplace_back(argument_ptr);
        return exclusive_argument_type(argument_ptr);
    }

    switch_argument_type add_switch(const string_vector_type& names)
    {
        auto argument_ptr = get_parser_data()->add_switch(names);
        m_arguments.emplace_back(argument_ptr);
        return switch_argument_type(argument_ptr);
    }

    valued_argument_type add_valued(const string_vector_type& names)
    {
        auto argument_ptr = get_parser_data()->add_valued(names);
        m_arguments.emplace_back(argument_ptr);
        return valued_argument_type(argument_ptr);
    }

    positional_argument_type add_positional(const string_type& name)
    {
        auto argument_ptr = get_parser_data()->add_positional(name);
        m_arguments.emplace_back(argument_ptr);
        return positional_argument_type(argument_ptr);
    }

    const std::vector<const_argument_ptr_type>& get_arguments() const
    {
        return m_arguments;
    }

private:
    parser_data_ptr_type get_parser_data() const
    {
        auto ptr = m_parser_data_ptr.lock();
        if (!ptr)
        {
            throw std::logic_error("Argument group referencing already freed parser");
        }
        return ptr;
    }

    parser_data_weak_ptr_type m_parser_data_ptr;
    string_type m_name;
    string_type m_description;
    std::vector<const_argument_ptr_type> m_arguments;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_GROUP_IMPL_HPP_
