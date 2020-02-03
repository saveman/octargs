#ifndef OCTARGS_BASIC_PARSER_HPP_
#define OCTARGS_BASIC_PARSER_HPP_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "basic_argument.hpp"
#include "basic_argument_table.hpp"
#include "basic_results.hpp"
#include "exception.hpp"

namespace oct
{
namespace args
{
    template <class traits>
    class basic_parser
    {
    public:
        using char_type = typename traits::char_type;
        using string_type = typename traits::string_type;
        using string_vector_type = typename traits::string_vector_type;

        using argument_type = basic_argument<traits>;
        using argument_table_type = basic_argument_table<traits>;
        using results_type = basic_results<traits>;

        basic_parser()
#if 0
            : m_arguments()
            , m_names_repository()
            , m_positional_arguments_enabled(false)
#else
            : m_positional_arguments_enabled(false)
#endif
        {
            // noop
        }

        void set_positional_arguments_enabled(bool enabled)
        {
            m_positional_arguments_enabled = enabled;
        }

        const results_type& parse(int argc, char_type* argv[])
        {
            return parse(argument_table_type(argc, argv));
        }

        const results_type& parse(int argc, const char_type* argv[])
        {
            return parse(argument_table_type(argc, argv));
        }

        const results_type& parse(const argument_table_type& arg_table)
        {
            m_solid_results = {};
            m_solid_results.set_input(arg_table);
            m_solid_results.set_app_name(arg_table.get_app_name());
            for (std::size_t i = 0; i < arg_table.get_argument_count();)
            {
                // placeholder: parse named arguments

                if (!m_positional_arguments_enabled)
                {
                    throw exception("Positional arguments not enabled");
                }

                m_solid_results.append_positional_argument(arg_table.get_argument(i));
                ++i;
            }

            // parse positional arguments

            return m_solid_results;
        }

#if 0

        argument& add_argument(const string_vector& names = { "-h", "--help" })
        {
            auto new_argument = std::make_shared<argument>(names);

            m_arguments.emplace_back(new_argument);

            add_to_names_repository(new_argument);

            // TODO: check other constraints

            return *new_argument;
        }

        //        void add_help_option(const string_vector& names = { "-h", "--help" });
        //        void add_version_option(const string_vector& names = { "--version" });
#endif

    private:
#if 0
        void add_to_names_repository(const std::shared_ptr<argument>& argument)
        {
            if (argument->get_names().size() == 0)
            {
                throw exception("Argument must have at least one name");
            }

            for (const auto& name : argument->get_names())
            {
                ensure_valid_name(name);

                m_names_repository.emplace(name, argument);
            }
        }

        void ensure_valid_name(const string& name)
        {
            if (name.empty())
            {
                throw exception("Argument name must not be empty");
            }

            // TODO: check name characters

            if ((name.find(' ') != string::npos) || (name.find('\t') != string::npos))
            {
                throw exception("Argument name must not contain spaces or tabs");
            }
        }

        std::vector<std::shared_ptr<argument>> m_arguments;
        std::map<std::string, std::shared_ptr<argument>> m_names_repository;
#endif
        using solid_results_type = basic_solid_results<traits>;

        bool m_positional_arguments_enabled;
        solid_results_type m_solid_results;
    };

} // namespace args
} // namespace oct

#endif /*OCTARGS_BASIC_PARSER_HPP_*/
