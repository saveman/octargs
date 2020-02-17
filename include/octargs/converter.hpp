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

template <typename TRAITS, typename DATA>
class basic_converter
{
private:
    using string_type = typename TRAITS::string_type;

    void operator()(const string_type& /*value_str*/) const;
};

template <typename TRAITS>
class basic_converter<TRAITS, typename TRAITS::string_type>
{
public:
    using string_type = typename TRAITS::string_type;

    string_type operator()(const string_type& value_str) const
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

    bool operator()(const string_type& value_str) const
    {
        const auto& true_literals = traits_type::get_true_literals();
        if (std::find(true_literals.begin(), true_literals.end(), value_str) != true_literals.end())
        {
            return true;
        }

        const auto& false_literals = traits_type::get_false_literals();
        if (std::find(false_literals.begin(), false_literals.end(), value_str) != false_literals.end())
        {
            return false;
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

    data_type operator()(const string_type& value_str) const
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

    data_type operator()(const string_type& value_str) const
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
    : public basic_integer_converter<TRAITS, short, internal::stox<typename TRAITS::string_type, short>>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, unsigned short> : public basic_integer_converter<TRAITS, unsigned short,
                                                    internal::stox<typename TRAITS::string_type, unsigned short>>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, int> : public basic_integer_converter<TRAITS, int, std::stoi>
{
};

template <typename TRAITS>
class basic_converter<TRAITS, unsigned int>
    : public basic_integer_converter<TRAITS, unsigned int, internal::stox<typename TRAITS::string_type, unsigned int>>
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

#endif // OCTARGS_CONVERTER_HPP_
