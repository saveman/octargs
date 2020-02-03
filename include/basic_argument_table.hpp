#ifndef OCTARGS_BASIC_ARGUMENT_TABLE_HPP_
#define OCTARGS_BASIC_ARGUMENT_TABLE_HPP_

#include <string>
#include <vector>

namespace oct
{
namespace args
{

    template <class traits>
    class basic_argument_table
    {
    public:
        using char_type = typename traits::char_type;
        using string_type = typename traits::string_type;
        using string_vector_type = typename traits::string_vector_type;

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

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_ARGUMENT_TABLE_HPP_*/
