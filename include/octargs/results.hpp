#ifndef OCTARGS_RESULTS_HPP_
#define OCTARGS_RESULTS_HPP_

#include <memory>

#include "argument_table.hpp"
#include "internal/argument.hpp"
#include "internal/function_helpers.hpp"
#include "internal/results_data.hpp"

namespace oct
{
namespace args
{

/// \brief Argument parsing results
///
/// \tparam char_T              char type (as in std::basic_string)
template <typename char_T>
class basic_results
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_table_type = basic_argument_table<char_type>;

    using results_data_type = internal::basic_results_data<char_type>;
    using const_results_data_ptr_type = std::shared_ptr<const results_data_type>;

    using dictionary_type = dictionary<char_type>;
    using const_dictionary_ptr_type = std::shared_ptr<const dictionary_type>;

    basic_results(const_dictionary_ptr_type dictionary_ptr, const_results_data_ptr_type results_data_ptr)
        : m_dictionary_ptr(dictionary_ptr)
        , m_results_data_ptr(results_data_ptr)
    {
        // noop
    }

    const string_type& get_app_name() const
    {
        return m_results_data_ptr->get_app_name();
    }

    bool has_value(const string_type& arg_name) const
    {
        return get_count(arg_name) > 0;
    }

    std::size_t get_count(const string_type& arg_name) const
    {
        return m_results_data_ptr->get_count(arg_name);
    }

    const string_type& get_first_value(const string_type& arg_name) const
    {
        auto& values = get_values(arg_name);
        if (values.size() == 0)
        {
            throw std::logic_error("values collection is empty");
        }
        return values[0];
    }

    const string_type& get_first_value(const string_type& arg_name, const string_type& default_value) const
    {
        auto& values = get_values(arg_name);
        if (values.size() == 0)
        {
            return default_value;
        }
        return values[0];
    }

    const string_vector_type& get_values(const string_type& arg_name) const
    {
        return m_results_data_ptr->get_values(arg_name);
    }

    template <typename data_T, typename converter_T = basic_converter<char_type, data_T>>
    data_T get_first_value_as(const string_type& arg_name) const
    {
        using data_type = data_T;
        using converter_type = converter_T;
        using helper = internal::convert_function_helper<char_type, data_type>;

        auto& value = get_first_value(arg_name);
        auto convert_func = helper::prepare(converter_type());
        return convert_func(*m_dictionary_ptr, value);
    }

    template <typename data_T, typename converter_T = basic_converter<char_type, data_T>>
    data_T get_first_value_as(const string_type& arg_name, const data_T& default_value) const
    {
        using data_type = data_T;
        using converter_type = converter_T;
        using helper = internal::convert_function_helper<char_type, data_type>;

        auto& values = get_values(arg_name);
        if (values.size() == 0)
        {
            return default_value;
        }
        else
        {
            auto& value = values[0];
            auto convert_func = helper::prepare(converter_type());
            return convert_func(*m_dictionary_ptr, value);
        }
    }

    template <typename data_T, typename converter_T = basic_converter<char_type, data_T>>
    std::vector<data_T> get_values_as(const string_type& arg_name) const
    {
        using data_type = data_T;
        using converter_type = converter_T;
        using helper = internal::convert_function_helper<char_type, data_type>;

        std::vector<data_type> data_vector;

        auto convert_func = helper::prepare(converter_type());

        for (const auto& value_str : get_values(arg_name))
        {
            data_vector.emplace_back(convert_func(*m_dictionary_ptr, value_str));
        }

        return data_vector;
    }

private:
    const_dictionary_ptr_type m_dictionary_ptr;
    const_results_data_ptr_type m_results_data_ptr;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_RESULTS_HPP_
