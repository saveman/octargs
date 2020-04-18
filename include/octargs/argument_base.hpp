#ifndef OCTARGS_ARGUMENT_BASE_HPP_
#define OCTARGS_ARGUMENT_BASE_HPP_

#include "internal/argument.hpp"

namespace oct
{
namespace args
{

template <typename derived_T, typename char_T, typename values_storage_T, typename data_T>
class basic_typed_argument_base
{
public:
    using derived_type = derived_T;

    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using handler_type = basic_argument_type_handler<data_type, char_type, values_storage_type>;
    using handler_ptr_type = std::shared_ptr<handler_type>;

    using convert_function_type = typename handler_type::convert_function_type;
    using check_function_type = typename handler_type::check_function_type;
    using store_function_type = typename handler_type::store_function_type;

    template <typename function_T>
    derived_type& set_convert_function(const function_T& func)
    {
        if (!m_handler)
        {
            // TODO: proper exception
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
            // TODO: proper exception
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
            // TODO: proper exception
            throw std::logic_error("Type (handler) not set");
        }
        m_handler->set_store_function(func);
        return cast_this_to_derived();
    }

    derived_type& set_storage(data_type values_storage_type::*member_ptr)
    {
        return set_store_function(
            [member_ptr](values_storage_type& storage, const data_type& value) { storage.*member_ptr = value; });
    }

    derived_type& set_storage(std::vector<data_type> values_storage_type::*member_ptr)
    {
        return set_store_function([member_ptr](values_storage_type& storage, const data_type& value) {
            (storage.*member_ptr).push_back(value);
        });
    }

protected:
    explicit basic_typed_argument_base()
        : m_handler()
    {
        // noop
    }

    explicit basic_typed_argument_base(handler_ptr_type handler)
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

    void reset_handler()
    {
        m_handler.reset();
    }

private:
    handler_ptr_type m_handler;
};

template <typename derived_T, typename char_T, typename values_storage_T>
class basic_typed_argument_base<derived_T, char_T, values_storage_T, void>
{
public:
    using handler_ptr_type = void;

protected:
    void reset_handler()
    {
        // noop
    }
};

/// \brief Argument base
///
/// \tparam derived_T   derived argument class
/// \tparam char_T      char type (as in std::basic_string)
template <typename derived_T, typename argument_T, typename char_T, typename values_storage_T, typename data_T>
class basic_argument_base : public basic_typed_argument_base<derived_T, char_T, values_storage_T, data_T>
{
public:
    using derived_type = derived_T;
    using argument_type = argument_T;

    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_typed_argument_base<derived_T, char_T, values_storage_T, data_T>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_ptr_type = std::shared_ptr<argument_type>;

    using handler_ptr_type = typename base_type::handler_ptr_type;

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
    template <typename new_data_T>
    using casted_converter_type = basic_converter<char_type, new_data_T>;

    template <typename new_data_T>
    using casted_converter_data_type = typename casted_converter_type<new_data_T>::data_type;

    template <typename new_data_T>
    using has_no_converter = std::is_void<casted_converter_data_type<new_data_T>>;

    template <typename new_data_T>
    using enable_if_converter_exist = std::enable_if<!has_no_converter<new_data_T>::value, new_data_T>;

    template <typename new_data_T>
    using enable_if_converter_not_exist = std::enable_if<has_no_converter<new_data_T>::value, new_data_T>;

    explicit basic_argument_base(argument_ptr_type argument)
        : base_type()
        , m_argument(argument)
    {
        if (!argument)
        {
            throw std::invalid_argument("argument");
        }
    }

    template <typename handler_ptr_T>
    explicit basic_argument_base(argument_ptr_type argument, handler_ptr_T handler)
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

    argument_ptr_type get_argument_ptr()
    {
        return m_argument;
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

    template <typename new_derived_T, typename converter_T>
    new_derived_T set_type_internal(const converter_T& converter)
    {
        using new_derived_type = new_derived_T;
        using handler_type = typename new_derived_type::handler_type;

        auto handler = std::make_shared<handler_type>();
        handler->set_convert_function(converter);

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
