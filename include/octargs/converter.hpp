#ifndef OCTARGS_CONVERTER_HPP_
#define OCTARGS_CONVERTER_HPP_

#include <limits>
#include <string>

#include "exception.hpp"
#include "internal/string_utils.hpp"
#include "parser_dictionary.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename data_T>
class basic_converter
{
public:
    basic_converter() = delete;
};

template <typename char_T>
class basic_converter<char_T, std::basic_string<char_T>>
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;

    string_type operator()(const string_type& value_str) const
    {
        return value_str;
    }
};

template <typename char_T>
class basic_converter<char_T, bool>
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using dictionary_type = parser_dictionary<char_type>;

    bool operator()(const string_type& value_str) const
    {
        const auto& true_literals = dictionary_type::get_true_literals();
        if (std::find(true_literals.begin(), true_literals.end(), value_str) != true_literals.end())
        {
            return true;
        }

        const auto& false_literals = dictionary_type::get_false_literals();
        if (std::find(false_literals.begin(), false_literals.end(), value_str) != false_literals.end())
        {
            return false;
        }

        throw conversion_error_ex<char_type>(value_str);
    }
};

template <typename char_T, typename data_T, data_T (*convert_F)(const std::basic_string<char_T>&, std::size_t*, int)>
class basic_integer_converter
{
public:
    using char_type = char_T;
    using data_type = data_T;

    using string_type = std::basic_string<char_type>;

    data_type operator()(const string_type& value_str) const
    {
        try
        {
            std::size_t end_index;

            const auto trimmed_str = internal::trim_copy(value_str);
            auto result = convert_F(trimmed_str, &end_index, 0);
            if (end_index == trimmed_str.size())
            {
                return result;
            }
        }
        catch (const std::exception&)
        {
            std::throw_with_nested(conversion_error_ex<char_type>(value_str));
        }
        throw conversion_error_ex<char_type>(value_str);
    }
};

template <typename char_T, typename data_T, data_T (*convert_F)(const std::basic_string<char_T>&, std::size_t*)>
class basic_floating_point_converter
{
public:
    using char_type = char_T;
    using data_type = data_T;

    using string_type = std::basic_string<char_type>;

    data_type operator()(const string_type& value_str) const
    {
        try
        {
            std::size_t end_index;

            const auto trimmed_str = internal::trim_copy(value_str);
            auto result = convert_F(trimmed_str, &end_index);
            if (end_index == trimmed_str.size())
            {
                return result;
            }
        }
        catch (const std::exception&)
        {
            std::throw_with_nested(conversion_error_ex<char_type>(value_str));
        }
        throw conversion_error_ex<char_type>(value_str);
    }
};

template <typename char_T>
class basic_converter<char_T, float> : public basic_floating_point_converter<char_T, float, std::stof>
{
};

template <typename char_T>
class basic_converter<char_T, double> : public basic_floating_point_converter<char_T, double, std::stod>
{
};

template <typename char_T>
class basic_converter<char_T, long double> : public basic_floating_point_converter<char_T, long double, std::stold>
{
};

template <typename char_T>
class basic_converter<char_T, short>
    : public basic_integer_converter<char_T, short, internal::stox<std::basic_string<char_T>, short>>
{
};

template <typename char_T>
class basic_converter<char_T, unsigned short>
    : public basic_integer_converter<char_T, unsigned short, internal::stox<std::basic_string<char_T>, unsigned short>>
{
};

template <typename char_T>
class basic_converter<char_T, int> : public basic_integer_converter<char_T, int, std::stoi>
{
};

template <typename char_T>
class basic_converter<char_T, unsigned int>
    : public basic_integer_converter<char_T, unsigned int, internal::stox<std::basic_string<char_T>, unsigned int>>
{
};

template <typename char_T>
class basic_converter<char_T, long> : public basic_integer_converter<char_T, long, std::stol>
{
};

template <typename char_T>
class basic_converter<char_T, unsigned long> : public basic_integer_converter<char_T, unsigned long, std::stoul>
{
};

template <typename char_T>
class basic_converter<char_T, long long> : public basic_integer_converter<char_T, long long, std::stoll>
{
};

template <typename char_T>
class basic_converter<char_T, unsigned long long>
    : public basic_integer_converter<char_T, unsigned long long, std::stoull>
{
};

} // namespace args
} // namespace oct

#endif // OCTARGS_CONVERTER_HPP_
