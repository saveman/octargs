#ifndef OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_
#define OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_

#include <functional>

#include "converter.hpp"
#include "internal/argument_handler.hpp"
#include "internal/function_helpers.hpp"

namespace oct
{
namespace args
{

/// \brief Handler for processing argument values in a type-specific way
///
/// The handler has following "hooks" (functions):
/// - convert - used to convert the string representation to data type
/// - check   - used to check if the data after conversion is valid
/// - store   - used to store the data
///
/// \tparam data_T              argument value data type
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of object to store parsed values
template <typename data_T, typename char_T, typename values_storage_T>
class basic_argument_type_handler : public internal::basic_argument_handler<char_T, values_storage_T>
{
public:
    using data_type = data_T;
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;

    using dictionary_type = parser_dictionary<char_type>;

    using convert_function_type = std::function<data_type(const dictionary_type& dictionary, const string_type&)>;
    using check_function_type = std::function<void(const data_type& data)>;
    using store_function_type = std::function<void(values_storage_type& storage, const data_type& data)>;

    basic_argument_type_handler()
        : m_convert_function()
        , m_check_function()
        , m_store_function()
    {
        // noop
    }

    template <typename function_T>
    basic_argument_type_handler& set_convert_function(const function_T& func)
    {
        using helper = internal::convert_function_helper<char_type, data_type>;
        m_convert_function = helper::prepare(func);
        return *this;
    }

    template <typename function_T>
    basic_argument_type_handler& set_check_function(const function_T& func)
    {
        using helper = internal::check_function_helper<char_type, data_type>;
        m_check_function = helper::prepare(func);
        return *this;
    }

    template <typename function_T>
    basic_argument_type_handler& set_store_function(const function_T& func)
    {
        using helper = internal::store_function_helper<char_type, data_type, values_storage_type>;
        m_store_function = helper::prepare(func);
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

    void parse(
        values_storage_type& storage, const dictionary_type& dictionary, const string_type& value_str) const final
    {
        if (!m_convert_function)
        {
            throw missing_converter_ex<char_type>(value_str);
        }

        data_type value = m_convert_function(dictionary, value_str);

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
