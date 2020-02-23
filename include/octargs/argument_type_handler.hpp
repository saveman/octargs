#ifndef OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_
#define OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_

#include <functional>

#include "converter.hpp"
#include "internal/argument_handler.hpp"

namespace oct
{
namespace args
{

template <class char_T>
class missing_converter_ex : public missing_converter
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    missing_converter_ex(const string_type& value)
        : missing_converter()
        , m_value(value)
    {
        // noop
    }

    const string_type& value() const
    {
        return m_value;
    }

private:
    basic_shared_string<char_type> m_value;
};

template <typename data_T, typename char_T, typename values_storage_T>
class basic_argument_type_handler : public internal::basic_argument_handler<char_T, values_storage_T>
{
public:
    using data_type = data_T;
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_T>;

    using convert_function_type = std::function<data_type(const string_type&)>;
    using check_function_type = std::function<void(const data_type& data)>;
    using direct_store_function_type = std::function<void(const data_type& data)>;
    using store_function_type = std::function<void(values_storage_type& storage, const data_type& data)>;

    basic_argument_type_handler()
        : m_convert_function(basic_converter<char_type, data_type>())
        , m_store_function()
    {
        // noop
    }

    basic_argument_type_handler& set_convert_function(const convert_function_type& convert_func)
    {
        m_convert_function = convert_func;
        return *this;
    }

    basic_argument_type_handler& set_check_function(const check_function_type& check_func)
    {
        m_check_function = check_func;
        return *this;
    }

    basic_argument_type_handler& set_store_function(const store_function_type& store_func)
    {
        m_store_function = store_func;
        return *this;
    }

    basic_argument_type_handler& set_store_function(const direct_store_function_type& store_func)
    {
        m_store_function = [store_func](values_storage_type& /*storage*/, const data_type& data) { store_func(data); };
        return *this;
    }

    basic_argument_type_handler& set_storage(data_type values_storage_type::*member_ptr)
    {
        return set_store_function(
            [member_ptr](values_storage_type& storage, const data_type& value) { storage.*member_ptr = value; });
    }

    basic_argument_type_handler& set_storage(std::vector<data_type> values_storage_type::*member_ptr)
    {
        return set_store_function([member_ptr](values_storage_type& storage, const data_type& value) {
            (storage.*member_ptr).push_back(value);
        });
    }

    void parse(values_storage_type& storage, const string_type& value_str) const final
    {
        if (!m_convert_function)
        {
            throw missing_converter_ex<char_type>(value_str);
        }

        data_type value = m_convert_function(value_str);

        if (m_check_function)
        {
            m_check_function(value);
        }

        if (m_store_function)
        {
            m_store_function(storage, value);
        }
    }

private:
    convert_function_type m_convert_function;
    check_function_type m_check_function;
    store_function_type m_store_function;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_
