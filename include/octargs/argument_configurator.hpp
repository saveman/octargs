#ifndef OCTARGS_ARGUMENT_CONFIGURATOR_HPP_
#define OCTARGS_ARGUMENT_CONFIGURATOR_HPP_

#include "internal/argument.hpp"

namespace oct
{
namespace args
{

template <typename derived_T, typename argument_T, typename char_T, typename values_storage_T, typename data_T>
class basic_argument_configurator
{
public:
    using derived_type = derived_T;
    using argument_type = argument_T;

    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_ptr_type = std::shared_ptr<argument_type>;

    using handler_type = internal::basic_argument_handler<char_type, values_storage_type>;
    using const_handler_ptr_type = std::shared_ptr<const handler_type>;

    // TODO: refactor
    template <typename handler_data_T>
    using type_handler_type = basic_argument_type_handler<handler_data_T, char_type, values_storage_type>;

    template <typename handler_data_T, typename converter_T = basic_converter<char_type, handler_data_T>>
    type_handler_type<handler_data_T>& set_type()
    {
        return m_argument->template set_type<handler_data_T, converter_T>();
    }

    template <typename handler_data_T>
    type_handler_type<handler_data_T>& set_type_and_storage(handler_data_T values_storage_type::*member_ptr)
    {
        return m_argument->template set_type_and_storage<handler_data_T>(member_ptr);
    }

    template <typename handler_data_T>
    type_handler_type<handler_data_T>& set_type_and_storage(
        std::vector<handler_data_T> values_storage_type::*member_ptr)
    {
        return m_argument->template set_type_and_storage<handler_data_T>(member_ptr);
    }

    derived_type& set_description(const string_type& text)
    {
        m_argument->set_description(text);
        return cast_this_to_derived();
    }

protected:
    explicit basic_argument_configurator(argument_ptr_type argument)
        : m_argument(argument)
    {
        if (!argument)
        {
            throw std::invalid_argument("argument");
        }
    }

    argument_type& get_argument()
    {
        return *m_argument;
    }

    derived_type& cast_this_to_derived()
    {
        return static_cast<derived_type&>(*this);
    }

private:
    argument_ptr_type m_argument;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_CONFIGURATOR_HPP_
