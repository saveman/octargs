#ifndef OCTARGS_ARGUMENT_GROUP_HPP_
#define OCTARGS_ARGUMENT_GROUP_HPP_

#include "internal/argument_group_impl.hpp"
#include "names.hpp"

namespace oct
{
namespace args
{

/// \brief Arguments group
///
/// This class allows grouping arguments. It is used currently only for documentation
/// purpose (usage automatic documentation).
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of object to store parsed values
template <typename char_T, typename values_storage_T>
class basic_argument_group
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_group_type = internal::basic_argument_group_impl<char_type, values_storage_type>;
    using argument_group_ptr_type = std::shared_ptr<argument_group_type>;

    using exclusive_argument_type = basic_exclusive_argument<char_type, values_storage_type>;
    using positional_argument_type = basic_positional_argument<char_type, values_storage_type>;
    using switch_argument_type = basic_switch_argument<char_type, values_storage_type>;
    using valued_argument_type = basic_valued_argument<char_type, values_storage_type>;

    basic_argument_group(argument_group_ptr_type argument_group)
        : m_argument_group(argument_group)
    {
        if (!argument_group)
        {
            throw std::invalid_argument("argument_group");
        }
    }

    basic_argument_group& set_description(const string_type& description)
    {
        m_argument_group->set_description(description);
        return *this;
    }

    exclusive_argument_type add_exclusive(const multi_name_wrapper<char_type>& names)
    {
        return m_argument_group->add_exclusive(names);
    }

    switch_argument_type add_switch(const multi_name_wrapper<char_type>& names)
    {
        return m_argument_group->add_switch(names);
    }

    valued_argument_type add_valued(const multi_name_wrapper<char_type>& names)
    {
        return m_argument_group->add_valued(names);
    }

    positional_argument_type add_positional(const single_name_wrapper<char_type>& name)
    {
        return m_argument_group->add_positional(name);
    }

private:
    argument_group_ptr_type m_argument_group;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_GROUP_HPP_
