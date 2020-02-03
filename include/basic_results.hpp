#ifndef OCTARGS_BASIC_RESULTS_HPP_
#define OCTARGS_BASIC_RESULTS_HPP_

#include "basic_argument_table.hpp"
#include "exception.hpp"

namespace oct
{
namespace args
{
    template <class traits>
    class basic_results
    {
    public:
        using char_type = typename traits::char_type;
        using string_type = typename traits::string_type;
        using string_vector_type = typename traits::string_vector_type;

        using argument_table_type = basic_argument_table<traits>;

        basic_results()
            : m_input_table()
        {
            // noop
        }

        const argument_table_type& get_input() const
        {
            return m_input_table;
        }

        const string_type& get_app_name() const
        {
            return m_app_name;
        }

        const string_vector_type& get_positional_arguments() const
        {
            return m_positional_arguments;
        }

    protected:
        argument_table_type m_input_table;
        string_type m_app_name;
        string_vector_type m_positional_arguments;
    };

    // TODO: move to a separate file?

    template <class traits>
    class basic_solid_results : public basic_results<traits>
    {
    public:
        using char_type = typename traits::char_type;
        using string_type = typename traits::string_type;
        using string_vector_type = typename traits::string_vector_type;

        using argument_table_type = basic_argument_table<traits>;

        basic_solid_results()
            : basic_results<traits>()
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

        void append_positional_argument(const string_type& arg_value)
        {
            this->m_positional_arguments.push_back(arg_value);
        }
    };

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_RESULTS_HPP_*/
