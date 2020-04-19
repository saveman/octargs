#ifndef OCTARGS_FUNCTION_HELPERS_HPP_
#define OCTARGS_FUNCTION_HELPERS_HPP_

#include <functional>
#include <string>

#include "../parser_dictionary.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename data_T>
class convert_function_helper
{
public:
    using char_type = char_T;
    using data_type = data_T;

    using string_type = std::basic_string<char_type>;
    using dictionary_type = parser_dictionary<char_type>;

    using function_type = std::function<data_type(const dictionary_type& dictionary, const string_type& value_str)>;

    using simple_function_type = std::function<data_type(const string_type& value_str)>;

    convert_function_helper() = delete;

    static const function_type& prepare(const function_type& func)
    {
        return func;
    }

    static function_type prepare(const simple_function_type& func)
    {
        if (!func)
        {
            return function_type();
        }

        return [func](const dictionary_type& /*dictionary*/, const string_type& value_str) { return func(value_str); };
    }
};

template <typename char_T, typename data_T>
class check_function_helper
{
public:
    using char_type = char_T;
    using data_type = data_T;

    using function_type = std::function<void(const data_type& data)>;

    check_function_helper() = delete;

    static const function_type& prepare(const function_type& func)
    {
        return func;
    }
};

template <typename char_T, typename data_T, typename values_storage_T>
class store_function_helper
{
public:
    using char_type = char_T;
    using data_type = data_T;
    using values_storage_type = values_storage_T;

    using function_type = std::function<void(values_storage_type& storage, const data_type& data)>;

    using simple_function_type = std::function<void(const data_type& data)>;

    store_function_helper() = delete;

    static const function_type& prepare(const function_type& func)
    {
        return func;
    }

    static function_type prepare(const simple_function_type& func)
    {
        if (!func)
        {
            return function_type();
        }

        return [func](values_storage_type& /*storage*/, const data_type& data) { return func(data); };
    }
};

template <typename char_T, typename data_T>
class store_function_helper<char_T, data_T, void>
{
public:
    using char_type = char_T;
    using data_type = data_T;
    using function_type = std::function<void(const data_type& data)>;

    store_function_helper() = delete;

    static const function_type& prepare(const function_type& func)
    {
        return func;
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_FUNCTION_HELPERS_HPP_
