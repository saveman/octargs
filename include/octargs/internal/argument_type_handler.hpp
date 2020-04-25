#ifndef OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_
#define OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_

#include <functional>

#include "../converter.hpp"
#include "argument_handler.hpp"
#include "function_helpers.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename storage_T>
class storage_helper
{
public:
    using type = storage_T;
};

template <>
class storage_helper<void>
{
private:
    struct internal
    {
        int none;
    };

public:
    using type = internal;
};

template <typename data_T, typename char_T, typename values_storage_T>
class basic_argument_type_handler_base : public basic_argument_handler<char_T, values_storage_T>
{
public:
    using data_type = data_T;
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using convert_helper = convert_function_helper<char_type, data_type>;
    using check_helper = check_function_helper<char_type, data_type>;
    using store_helper = store_function_helper<char_type, data_type, values_storage_type>;

    using convert_function_type = typename convert_helper::function_type;
    using check_function_type = typename check_helper::function_type;
    using store_function_type = typename store_helper::function_type;

    using storage_helper_wrapped_type = typename storage_helper<values_storage_type>::type;

    basic_argument_type_handler_base()
        : m_convert_function()
        , m_check_function()
        , m_store_function()
    {
        // noop
    }

    template <typename function_T>
    void set_convert_function(const function_T& func)
    {
        m_convert_function = convert_helper::prepare(func);
    }

    template <typename function_T>
    void set_check_function(const function_T& func)
    {
        m_check_function = check_helper::prepare(func);
    }

    template <typename function_T>
    void set_store_function(const function_T& func)
    {
        m_store_function = store_helper::prepare(func);
    }

    // cppcheck-suppress functionStatic
    void set_storage(data_type storage_helper_wrapped_type::*member_ptr)
    {
        set_store_function(
            [member_ptr](values_storage_type& storage, const data_type& value) { storage.*member_ptr = value; });
    }

    // cppcheck-suppress functionStatic
    void set_storage(std::vector<data_type> storage_helper_wrapped_type::*member_ptr)
    {
        set_store_function([member_ptr](values_storage_type& storage, const data_type& value) {
            (storage.*member_ptr).emplace_back(value);
        });
    }

protected:
    convert_function_type m_convert_function;
    check_function_type m_check_function;
    store_function_type m_store_function;
};

template <typename data_T, typename char_T, typename values_storage_T>
class basic_argument_type_handler : public basic_argument_type_handler_base<data_T, char_T, values_storage_T>
{
public:
    using data_type = data_T;
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using dictionary_type = dictionary<char_type>;

    void parse(
        values_storage_type& storage, const dictionary_type& dictionary, const string_type& value_str) const final
    {
        if (!this->m_convert_function)
        {
            throw missing_converter_ex<char_type>(value_str);
        }

        data_type value = this->m_convert_function(dictionary, value_str);

        if (this->m_check_function)
        {
            this->m_check_function(value);
        }

        if (this->m_store_function)
        {
            this->m_store_function(storage, value);
        }
    }
};

template <typename data_T, typename char_T>
class basic_argument_type_handler<data_T, char_T, void> : public basic_argument_type_handler_base<data_T, char_T, void>
{
public:
    using data_type = data_T;
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using dictionary_type = dictionary<char_type>;

    void parse(const dictionary_type& dictionary, const string_type& value_str) const final
    {
        if (!this->m_convert_function)
        {
            throw missing_converter_ex<char_type>(value_str);
        }

        data_type value = this->m_convert_function(dictionary, value_str);

        if (this->m_check_function)
        {
            this->m_check_function(value);
        }

        if (this->m_store_function)
        {
            this->m_store_function(value);
        }
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_
