#ifndef OCTARGS_BASIC_RESULTS_DATA_HPP_
#define OCTARGS_BASIC_RESULTS_DATA_HPP_

#include "../exception.hpp"
#include "basic_parser_data.hpp"

namespace oct
{
namespace args
{

template <class TRAITS>
class basic_results;

namespace internal
{

template <class TRAITS>
struct basic_results_data
{
    using char_type = typename TRAITS::char_type;
    using string_type = typename TRAITS::string_type;
    using string_vector_type = typename TRAITS::string_vector_type;

    using argument_table_type = basic_argument_table<TRAITS>;

    using argument_type = basic_argument<TRAITS>;
    using argument_ptr_type = std::shared_ptr<argument_type>;

    using parser_data_type = basic_parser_data<TRAITS>;
    using parser_data_ptr_type = std::shared_ptr<parser_data_type>;

    basic_results_data(parser_data_ptr_type parser_data_ptr)
        : m_parser_data_ptr(parser_data_ptr)
        , m_input_table()
        , m_app_name()
        , m_argument_values()
    {
        // noop
    }

    void set_input(const argument_table_type& input_table)
    {
        this->m_input_table = input_table;
    }

    void set_app_name(const string_type& app_name)
    {
        this->m_app_name = app_name;
    }

    bool has_value(const argument_ptr_type& arg_ptr) const
    {
        return this->m_argument_values.find(arg_ptr) != this->m_argument_values.end();
    }

    std::size_t value_count(const argument_ptr_type& arg_ptr) const
    {
        auto arg_values_iter = this->m_argument_values.find(arg_ptr);
        if (arg_values_iter != this->m_argument_values.end())
        {
            return arg_values_iter->second.size();
        }
        else
        {
            return 0;
        }
    }

    void append_value(const argument_ptr_type& arg_ptr, const string_type& value)
    {
        auto arg_values_iter = this->m_argument_values.find(arg_ptr);
        if (arg_values_iter != this->m_argument_values.end())
        {
            arg_values_iter->second.push_back(value);
        }
        else
        {
            this->m_argument_values.insert(std::make_pair(arg_ptr, string_vector_type({ value })));
        }
    }

    argument_ptr_type find_argument(const string_type& arg_name) const
    {
        auto name_arg_iter = m_parser_data_ptr->m_names_repository.find(arg_name);
        if (name_arg_iter != m_parser_data_ptr->m_names_repository.end())
        {
            return name_arg_iter->second;
        }

        throw logic_error_exception("Argument not found");
    }

    const parser_data_ptr_type m_parser_data_ptr;

    argument_table_type m_input_table;
    string_type m_app_name;
    std::map<argument_ptr_type, string_vector_type> m_argument_values;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_RESULTS_DATA_HPP_*/
