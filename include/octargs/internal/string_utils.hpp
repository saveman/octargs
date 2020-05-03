#ifndef OCTARGS_STRING_UTILS_HPP_
#define OCTARGS_STRING_UTILS_HPP_

#include <algorithm>

#include "char_utils.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename string_T>
inline void trim_lead(string_T& s)
{
    using char_type = typename string_T::value_type;

    auto is_space_pred = [](char_type c) { return !is_space(c); };
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), is_space_pred));
}

template <typename string_T>
inline void trim_trail(string_T& s)
{
    using char_type = typename string_T::value_type;

    auto is_space_pred = [](char_type c) { return !is_space(c); };
    s.erase(std::find_if(s.rbegin(), s.rend(), is_space_pred).base(), s.end());
}

template <typename string_T>
inline void trim(string_T& s)
{
    trim_lead(s);
    trim_trail(s);
}

template <typename string_T>
inline string_T trim_lead_copy(string_T s)
{
    trim_lead(s);
    return s;
}

template <typename string_T>
inline string_T trim_trail_copy(string_T s)
{
    trim_trail(s);
    return s;
}

template <typename string_T>
inline string_T trim_copy(string_T s)
{
    trim(s);
    return s;
}

template <typename string_T, typename data_T>
inline data_T stox(const string_T& value_str, std::size_t* end_ptr, int base = 0)
{
    using data_type = data_T;
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

template <typename char_T>
class string_utils
{
};

template <>
class string_utils<char>
{
public:
    using string_type = std::basic_string<char>;

    template <typename data_T>
    static string_type to_string(const data_T& data)
    {
        return std::to_string(data);
    }
};

template <>
class string_utils<wchar_t>
{
public:
    using string_type = std::basic_string<wchar_t>;

    template <typename data_T>
    static string_type to_string(const data_T& data)
    {
        return std::to_wstring(data);
    }
};

template <typename char_T>
class string_less
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    string_less(bool case_sensitive)
        : m_case_sensitive(case_sensitive)
    {
        // noop
    }

    bool operator()(const string_type& str1, const string_type& str2) const
    {
        if (m_case_sensitive)
        {
            return str1 < str2;
        }
        else
        {
            std::size_t len1 = str1.size();
            std::size_t len2 = str2.size();
            for (std::size_t i = 0; (i < len1) && (i < len2); ++i)
            {
                if (to_lower(str1[i]) < to_lower(str2[i]))
                {
                    return true;
                }
                if (to_lower(str1[i]) > to_lower(str2[i]))
                {
                    return false;
                }
            }
            if (len1 < len2)
            {
                return true;
            }
            if (len1 > len2)
            {
                return false;
            }
            return false;
        }
    }

private:
    bool m_case_sensitive;
};

template <typename char_T>
class string_equal
{
public:
    using char_type = char_T;
    using string_type = std::basic_string<char_type>;

    string_equal(bool case_sensitive)
        : m_case_sensitive(case_sensitive)
    {
        // noop
    }

    bool operator()(const string_type& str1, const string_type& str2) const
    {
        if (m_case_sensitive)
        {
            return str1 == str2;
        }
        else
        {
            std::size_t len1 = str1.size();
            std::size_t len2 = str2.size();
            if (len1 != len2)
            {
                return false;
            }

            for (std::size_t i = 0; i < len1; ++i)
            {
                if (to_lower(str1[i]) != to_lower(str2[i]))
                {
                    return false;
                }
            }
            return true;
        }
    }

private:
    bool m_case_sensitive;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_STRING_UTILS_HPP_
