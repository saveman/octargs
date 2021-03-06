#ifndef OCTARGS_ARGUMENT_TABLE_HPP_
#define OCTARGS_ARGUMENT_TABLE_HPP_

#include <string>
#include <vector>

#include "exception.hpp"

namespace oct
{
namespace args
{

/// \brief Input arguments table
///
/// Simple input arguments wrapper class encapsulating arguments given to
/// to application (e.g. argc + argv passed to main function).
///
/// \tparam char_T      char type (as in std::basic_string)
template <typename char_T>
class basic_argument_table
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    explicit basic_argument_table()
        : m_app_name()
        , m_arguments()
    {
        // noop
    }

    explicit basic_argument_table(int argc, const char_type* argv[])
        : m_app_name(argv[0])
        , m_arguments(&argv[1], &argv[argc])
    {
        // noop
    }

    explicit basic_argument_table(int argc, char_type* argv[])
        : m_app_name(argv[0])
        , m_arguments(&argv[1], &argv[argc])
    {
        // noop
    }

    explicit basic_argument_table(const string_type& app_name, const string_vector_type& arguments)
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

/// \brief Iterator over input arguments table
///
/// \tparam char_T      char type (as in std::basic_string)
template <typename char_T>
class basic_argument_table_iterator
{
public:
    using char_type = char_T;

    using argument_table_type = basic_argument_table<char_type>;
    using string_type = std::basic_string<char_type>;

    explicit basic_argument_table_iterator(const argument_table_type& arg_table)
        : m_arg_table(arg_table)
        , m_arg_count(arg_table.get_argument_count())
        , m_arg_index(0)
    {
        // noop
    }

    std::size_t get_remaining_count() const
    {
        return m_arg_count - m_arg_index;
    }

    bool has_more() const
    {
        return m_arg_index < m_arg_count;
    }

    const string_type& peek_next() const
    {
        if (!has_more())
        {
            throw std::out_of_range("No more arguments available");
        }

        return m_arg_table.get_argument(m_arg_index);
    }

    const string_type& take_next()
    {
        if (!has_more())
        {
            throw std::out_of_range("No more arguments available");
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
