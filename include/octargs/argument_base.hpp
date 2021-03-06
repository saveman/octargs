#ifndef OCTARGS_ARGUMENT_BASE_HPP_
#define OCTARGS_ARGUMENT_BASE_HPP_

#include "internal/argument.hpp"
#include "internal/argument_type_handler.hpp"

#include <type_traits>

namespace oct
{
namespace args
{

/// \brief Argument base type handling
///
/// The class allows to set following "hooks" (functions):
/// - convert - used to convert the string representation to data type
/// - check   - used to check if the data after conversion is valid
/// - store   - used to store the data
///
/// \tparam derived_TT          derived argument template class
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of object to store parsed values
/// \tparam data_T              argument value data type
template <template <typename, typename, typename> class derived_TT, typename char_T, typename values_storage_T,
    typename data_T>
class basic_typed_argument_base
{
public:
    using derived_type = derived_TT<char_T, values_storage_T, data_T>;

    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using handler_type = internal::basic_argument_type_handler<data_type, char_type, values_storage_type>;
    using handler_ptr_type = std::shared_ptr<handler_type>;

    using convert_function_type = typename handler_type::convert_function_type;
    using check_function_type = typename handler_type::check_function_type;
    using store_function_type = typename handler_type::store_function_type;

    using storage_helper_wrapped_type = typename internal::storage_helper<values_storage_type>::type;

    template <typename function_T>
    derived_type& set_convert_function(const function_T& func)
    {
        if (!m_handler)
        {
            throw std::logic_error("Type (handler) not set");
        }
        m_handler->set_convert_function(func);
        return cast_this_to_derived();
    }

    template <typename function_T>
    derived_type& set_check_function(const function_T& func)
    {
        if (!m_handler)
        {
            throw std::logic_error("Type (handler) not set");
        }
        m_handler->set_check_function(func);
        return cast_this_to_derived();
    }

    template <typename function_T>
    derived_type& set_store_function(const function_T& func)
    {
        if (!m_handler)
        {
            throw std::logic_error("Type (handler) not set");
        }
        m_handler->set_store_function(func);
        return cast_this_to_derived();
    }

    // cppcheck-suppress functionStatic
    derived_type& set_storage(data_type storage_helper_wrapped_type::*member_ptr)
    {
        return set_store_function(
            [member_ptr](values_storage_type& storage, const data_type& value) { storage.*member_ptr = value; });
    }

    // cppcheck-suppress functionStatic
    derived_type& set_storage(std::vector<data_type> storage_helper_wrapped_type::*member_ptr)
    {
        return set_store_function([member_ptr](values_storage_type& storage, const data_type& value) {
            (storage.*member_ptr).emplace_back(value);
        });
    }

protected:
    explicit basic_typed_argument_base()
        : m_handler()
    {
        // noop
    }

    explicit basic_typed_argument_base(const handler_ptr_type& handler)
        : m_handler(handler)
    {
        if (!handler)
        {
            throw std::invalid_argument("handler");
        }
    }

    derived_type& cast_this_to_derived()
    {
        return static_cast<derived_type&>(*this);
    }

    // cppcheck-suppress functionStatic
    void reset_handler()
    {
        m_handler.reset();
    }

private:
    handler_ptr_type m_handler;
};

/// \brief Argument base type handling (limited, void type specialization)
///
/// \tparam derived_TT          derived argument template class
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of object to store parsed values
template <template <typename, typename, typename> class derived_TT, typename char_T, typename values_storage_T>
class basic_typed_argument_base<derived_TT, char_T, values_storage_T, void>
{
public:
    using derived_type = derived_TT<char_T, values_storage_T, void>;

    using handler_ptr_type = void;

    using storage_helper_wrapped_type = typename internal::storage_helper<values_storage_T>::type;

protected:
    // cppcheck-suppress functionStatic
    void reset_handler()
    {
        // noop
    }
};

/// \brief Argument base
///
/// \tparam derived_TT          derived argument template class
/// \tparam argument_TT         argument implementation template class
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
/// \tparam data_T              type of represented value
template <template <typename, typename, typename> class derived_TT, template <typename, typename> class argument_TT,
    typename char_T, typename values_storage_T, typename data_T>
class basic_argument_base : public basic_typed_argument_base<derived_TT, char_T, values_storage_T, data_T>
{
private:
    template <typename new_data_T>
    using casted_converter_type = basic_converter<char_T, new_data_T>;

    template <typename new_data_T>
    using casted_converter_data_type = typename casted_converter_type<new_data_T>::data_type;

    template <typename new_data_T>
    using has_no_converter = std::is_void<casted_converter_data_type<new_data_T>>;

    template <typename new_data_T>
    using enable_if_converter_exist = std::enable_if<!has_no_converter<new_data_T>::value, new_data_T>;

    template <typename new_data_T>
    using enable_if_converter_not_exist = std::enable_if<has_no_converter<new_data_T>::value, new_data_T>;

public:
    using argument_type = argument_TT<char_T, values_storage_T>;

    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_typed_argument_base<derived_TT, char_T, values_storage_T, data_T>;

    using derived_type = typename base_type::derived_type;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_ptr_type = std::shared_ptr<argument_type>;

    using handler_ptr_type = typename base_type::handler_ptr_type;

    template <typename new_data_T>
    using casted_derived_type = derived_TT<char_type, values_storage_type, new_data_T>;

    using storage_helper_wrapped_type = typename base_type::storage_helper_wrapped_type;

    derived_type& set_description(const string_type& text)
    {
        m_argument->set_description(text);
        return cast_this_to_derived();
    }

    template <typename new_data_T, typename enable_if_converter_exist<new_data_T>::type* = nullptr>
    casted_derived_type<new_data_T> set_type()
    {
        return set_type_internal<casted_derived_type<new_data_T>>().set_convert_function(
            casted_converter_type<new_data_T>());
    }

    template <typename new_data_T, typename enable_if_converter_not_exist<new_data_T>::type* = nullptr>
    casted_derived_type<new_data_T> set_type()
    {
        return this->template set_type_internal<casted_derived_type<new_data_T>>();
    }

    template <typename new_data_T>
    casted_derived_type<new_data_T> set_type_and_storage(new_data_T storage_helper_wrapped_type::*member_ptr)
    {
        return set_type<new_data_T>().set_storage(member_ptr);
    }

    template <typename new_data_T>
    casted_derived_type<new_data_T> set_type_and_storage(
        std::vector<new_data_T> storage_helper_wrapped_type::*member_ptr)
    {
        return set_type<new_data_T>().set_storage(member_ptr);
    }

protected:
    explicit basic_argument_base(const argument_ptr_type& argument)
        : base_type()
        , m_argument(argument)
    {
        if (!argument)
        {
            throw std::invalid_argument("argument");
        }
    }

    template <typename handler_ptr_T>
    explicit basic_argument_base(const argument_ptr_type& argument, const handler_ptr_T& handler)
        : base_type(handler)
        , m_argument(argument)
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

    template <typename new_derived_T>
    new_derived_T set_type_internal()
    {
        using new_derived_type = new_derived_T;
        using handler_type = typename new_derived_type::handler_type;

        auto handler = std::make_shared<handler_type>();

        m_argument->set_handler(handler);
        this->reset_handler();

        return new_derived_type(m_argument, handler);
    }

private:
    argument_ptr_type m_argument;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_BASE_HPP_
