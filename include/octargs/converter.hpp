#ifndef OCTARGS_CONVERTER_HPP_
#define OCTARGS_CONVERTER_HPP_

#include <exception>
#include <limits>
#include <string>

#include "exception.hpp"
#include "internal/string_utils.hpp"

namespace oct
{
namespace args
{

template <typename STRING, typename DATA>
inline DATA stox(const STRING& value_str, std::size_t* end_ptr, int base = 0)
{
    using data_type = DATA;
    using limits_type = std::numeric_limits<data_type>;

    static_assert(sizeof(data_type) < sizeof(long long), "Unsupported data type");

    auto result = std::stoll(value_str, end_ptr, base);

    if (result < limits_type::min())
    {
        throw std::out_of_range("Value lower than allowed minimum");
    }
    if (result > limits_type::max())
    {
        throw std::out_of_range("Value higher than allowed maximum");
    }
    return static_cast<data_type>(result);
}

template <typename TRAITS, typename DATA>
class basic_converter
{
private:
    using string_type = typename TRAITS::string_type;

    void operator()(const string_type& /*value_str*/);
};

template <typename TRAITS>
class basic_converter<TRAITS, typename TRAITS::string_type>
{
public:
    using string_type = typename TRAITS::string_type;

    string_type operator()(const string_type& value_str)
    {
        return value_str;
    }
};

template <typename TRAITS>
class basic_converter<TRAITS, bool>
{
public:
    using traits_type = TRAITS;
    using string_type = typename traits_type::string_type;

    bool operator()(const string_type& value_str)
    {
        for (const auto& true_value : traits_type::get_true_literals())
        {
            if (true_value == value_str)
            {
                return true;
            }
        }
        for (const auto& false_value : traits_type::get_false_literals())
        {
            if (false_value == value_str)
            {
                return false;
            }
        }
        throw parse_exception("Invalid argument value");
    }
};

template <typename TRAITS, typename DATA_TYPE,
    DATA_TYPE (*CONVERT_FUNC)(const typename TRAITS::string_type&, std::size_t*, int)>
class basic_integer_converter
{
public:
    using data_type = DATA_TYPE;
    using string_type = typename TRAITS::string_type;

    data_type operator()(const string_type& value_str)
    {
        try
        {
            std::size_t end_index;

            const auto trimmed_str = internal::trim_copy(value_str);
            auto result = CONVERT_FUNC(trimmed_str, &end_index, 0);
            if (end_index == trimmed_str.size())
            {
                return result;
            }
        }
        catch (const std::exception&)
        {
            // ignore
        }
        throw parse_exception("Invalid argument value");
    }
};

template <typename TRAITS, typename DATA_TYPE,
    DATA_TYPE (*CONVERT_FUNC)(const typename TRAITS::string_type&, std::size_t*)>
class basic_floating_point_converter
{
public:
    using data_type = DATA_TYPE;
    using string_type = typename TRAITS::string_type;

    data_type operator()(const string_type& value_str)
    {
        try
        {
            std::size_t end_index;

            const auto trimmed_str = internal::trim_copy(value_str);
            auto result = CONVERT_FUNC(trimmed_str, &end_index);
            if (end_index == trimmed_str.size())
            {
                return result;
            }
        }
        catch (const std::exception&)
        {
            // ignore
        }
        throw parse_exception("Invalid argument value");
    }
};

template <typename TRAITS>
class basic_converter<TRAITS, float> : public basic_floating_point_converter<TRAITS, float, std::stof>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, double> : public basic_floating_point_converter<TRAITS, double, std::stod>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, long double> : public basic_floating_point_converter<TRAITS, long double, std::stold>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, short>
    : public basic_integer_converter<TRAITS, short, stox<typename TRAITS::string_type, short>>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, unsigned short>
    : public basic_integer_converter<TRAITS, unsigned short, stox<typename TRAITS::string_type, unsigned short>>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, int> : public basic_integer_converter<TRAITS, int, std::stoi>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, unsigned int>
    : public basic_integer_converter<TRAITS, unsigned int, stox<typename TRAITS::string_type, unsigned int>>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, long> : public basic_integer_converter<TRAITS, long, std::stol>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, unsigned long> : public basic_integer_converter<TRAITS, unsigned long, std::stoul>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, long long> : public basic_integer_converter<TRAITS, long long, std::stoll>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, unsigned long long>
    : public basic_integer_converter<TRAITS, unsigned long long, std::stoull>
{
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_CONVERTER_HPP_*/
