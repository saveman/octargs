#ifndef OCTARGS_ARGUMENT_GROUP_HPP_
#define OCTARGS_ARGUMENT_GROUP_HPP_

#include <memory>
#include <string>
#include <vector>

#include "exclusive_argument_configurator.hpp"
#include "positional_argument_configurator.hpp"
#include "subparser_argument_configurator.hpp"
#include "switch_argument_configurator.hpp"
#include "valued_argument_configurator.hpp"

#include "internal/argument.hpp"
#include "internal/argument_repository.hpp"

namespace oct
{
namespace args
{

/// \brief Arguments group
///
/// This class allows grouping arguments. It is used currently only for documentation
/// purpose (usage automatic documentation).
///
/// \tparam derived_T   derived type
/// \tparam char_T      char type (as in std::basic_string)

// TODO: introduce configurator wrapper, to hide e.g. the get_arguments()
template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_argument_group
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_type = internal::basic_argument<char_type, values_storage_type>;
    using const_argument_ptr_type = std::shared_ptr<const argument_type>;

    using exclusive_argument_configurator_type = basic_exclusive_argument_configurator<char_type, values_storage_type>;
    using positional_argument_configurator_type
        = basic_positional_argument_configurator<char_type, values_storage_type>;
    using switch_argument_configurator_type = basic_switch_argument_configurator<char_type, values_storage_type>;
    using valued_argument_configurator_type = basic_valued_argument_configurator<char_type, values_storage_type>;

    using argument_repository_type = internal::basic_argument_repository<char_type, values_storage_type>;

    basic_argument_group(argument_repository_type& argument_repository, const string_type& name)
        : m_argument_repository(argument_repository)
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

    basic_argument_group& set_description(const string_type& description)
    {
        m_description = description;
        return *this;
    }

    exclusive_argument_configurator_type add_exclusive(const string_vector_type& names)
    {
        auto argument_ptr = m_argument_repository.add_exclusive(names);
        m_arguments.push_back(argument_ptr);
        return exclusive_argument_configurator_type(argument_ptr);
    }

    switch_argument_configurator_type add_switch(const string_vector_type& names)
    {
        auto argument_ptr = m_argument_repository.add_switch(names);
        m_arguments.push_back(argument_ptr);
        return switch_argument_configurator_type(argument_ptr);
    }

    valued_argument_configurator_type add_valued(const string_vector_type& names)
    {
        auto argument_ptr = m_argument_repository.add_valued(names);
        m_arguments.push_back(argument_ptr);
        return valued_argument_configurator_type(argument_ptr);
    }

    positional_argument_configurator_type add_positional(const string_type& name)
    {
        auto argument_ptr = m_argument_repository.add_positional(name);
        m_arguments.push_back(argument_ptr);
        return positional_argument_configurator_type(argument_ptr);
    }

    const std::vector<const_argument_ptr_type>& get_arguments() const
    {
        return m_arguments;
    }

private:
    argument_repository_type& m_argument_repository;
    string_type m_name;
    string_type m_description;
    std::vector<const_argument_ptr_type> m_arguments;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_GROUP_HPP_
