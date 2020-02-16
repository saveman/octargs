#ifndef OCTARGS_ARGUMENT_TABLE_HPP_
#define OCTARGS_ARGUMENT_TABLE_HPP_

#include <string>
#include <vector>

#include "exception.hpp"

namespace oct
{
namespace args
{

template <typename TRAITS>
class basic_argument_table
{
public:
    using char_type = typename TRAITS::char_type;
    using string_type = typename TRAITS::string_type;
    using string_vector_type = typename TRAITS::string_vector_type;

    basic_argument_table()
        : m_app_name()
        , m_arguments()
    {
        // noop
    }

    basic_argument_table(int argc, const char_type* argv[])
        : m_app_name(argv[0])
        , m_arguments(&argv[1], &argv[argc])
    {
        // noop
    }

    basic_argument_table(int argc, char_type* argv[])
        : m_app_name(argv[0])
        , m_arguments(&argv[1], &argv[argc])
    {
        // noop
    }

    basic_argument_table(const string_type& app_name, const string_vector_type& arguments)
        : m_app_name(app_name)
        , m_arguments(arguments)
    {
        // noop
    }

    const string_type& get_app_name() const
    {
        return m_app_name;
    }

    std::size_t get_argument_count() const
    {
        return m_arguments.size();
    }

    const string_type& get_argument(std::size_t index) const
    {
        return m_arguments[index];
    }

private:
    string_type m_app_name;
    string_vector_type m_arguments;
};

template <typename TRAITS>
class basic_argument_table_iterator
{
public:
    using argument_table_type = basic_argument_table<TRAITS>;
    using string_type = typename TRAITS::string_type;

    basic_argument_table_iterator(const argument_table_type& arg_table)
        : m_arg_table(arg_table)
        , m_arg_count(arg_table.get_argument_count())
        , m_arg_index(0)
    {
        // noop
    }

    bool has_more() const
    {
        return m_arg_index < m_arg_count;
    }

    const string_type& peek_next() const
    {
        if (!has_more())
        {
            throw logic_error_exception();
        }

        return m_arg_table.get_argument(m_arg_index);
    }

    const string_type& take_next()
    {
        if (!has_more())
        {
            throw logic_error_exception();
        }

        return m_arg_table.get_argument(m_arg_index++);
    }

private:
    const argument_table_type& m_arg_table;
    const std::size_t m_arg_count;
    std::size_t m_arg_index;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_TABLE_HPP_
