#ifndef OCTARGS_BASIC_RESULTS_HPP_
#define OCTARGS_BASIC_RESULTS_HPP_

#include <memory>

#include "basic_argument.hpp"
#include "basic_argument_table.hpp"
#include "internal/basic_results_data.hpp"
#include "traits.hpp"

namespace oct
{
namespace args
{
template <class TRAITS>
class basic_results
{
public:
    using char_type = typename TRAITS::char_type;
    using string_type = typename TRAITS::string_type;
    using string_vector_type = typename TRAITS::string_vector_type;

    using argument_table_type = basic_argument_table<TRAITS>;

    using results_data_type = internal::basic_results_data<TRAITS>;
    using results_data_ptr_type = std::shared_ptr<const results_data_type>;

    basic_results(results_data_ptr_type results_data_ptr)
        : m_results_data_ptr(results_data_ptr)
    {
        // noop
    }

    const argument_table_type& get_input() const
    {
        return m_results_data_ptr->m_input_table;
    }

    const string_type& get_app_name() const
    {
        return m_results_data_ptr->m_app_name;
    }

    bool has_value(const string_type& arg_name) const
    {
        return count(arg_name) > 0;
    }

    std::size_t count(const string_type& arg_name) const
    {
        auto name_arg_iter = m_results_data_ptr->find_argument(arg_name);
        auto values_iter = m_results_data_ptr->m_argument_values.find(name_arg_iter);
        if (values_iter != m_results_data_ptr->m_argument_values.end())
        {
            return values_iter->second.size();
        }
        else
        {
            return 0;
        }
    }

    const string_vector_type& values(const string_type& arg_name) const
    {
        auto name_arg_iter = m_results_data_ptr->find_argument(arg_name);
        auto values_iter = m_results_data_ptr->m_argument_values.find(name_arg_iter);
        if (values_iter != m_results_data_ptr->m_argument_values.end())
        {
            return values_iter->second;
        }
        else
        {
            return get_empty_values();
        }
    }

private:
    const string_vector_type& get_empty_values() const
    {
        static const string_vector_type EMPTY_VALUES;
        return EMPTY_VALUES;
    }

    results_data_ptr_type m_results_data_ptr;
};

using results = basic_results<traits<char>>;
using wresults = basic_results<traits<wchar_t>>;

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_RESULTS_HPP_*/
