#ifndef OCTARGS_PARSER_USAGE_HPP_
#define OCTARGS_PARSER_USAGE_HPP_

#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "argument_table.hpp"
#include "internal/argument.hpp"
#include "internal/char_utils.hpp"
#include "internal/misc.hpp"
#include "internal/parser_data.hpp"
#include "internal/results_data.hpp"
#include "results.hpp"
#include "usage_dictionary.hpp"

namespace oct
{
namespace args
{

/// \brief Parser usage output generator
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_parser_usage
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using ostream_type = std::basic_ostream<char_type>;

    using parser_data_type = internal::basic_parser_data<char_type, values_storage_type>;
    using const_parser_data_ptr_type = std::shared_ptr<const parser_data_type>;

    explicit basic_parser_usage(const const_parser_data_ptr_type& parser_data)
        : m_data_ptr(parser_data)
        , m_decorators_enabled(true)
        , m_description_indent(2)
        , m_multiline_indent(2)
        , m_arg_line_indent(1)
        , m_dictionary(std::make_shared<default_usage_dictionary<char_type>>())
    {
        // noop
    }

    friend ostream_type& operator<<(ostream_type& os, const basic_parser_usage& usage)
    {
        usage.print_usage_line(os);
        usage.print_header(os);
        usage.print_default_named_arguments(os);
        usage.print_default_positional_arguments(os);
        usage.print_groups(os);
        usage.print_subparsers(os);
        usage.print_footer(os);

        return os;
    }

private:
    using argument_type = internal::basic_argument<char_type, values_storage_type>;
    using const_argument_ptr_type = std::shared_ptr<const argument_type>;

    using string_vector_type = std::vector<string_type>;

    using string_utils_type = internal::string_utils<char_type>;

    using dictionary_type = usage_dictionary<char_type>;
    using dictionary_ptr_type = std::shared_ptr<dictionary_type>;

    using argument_group_type = basic_argument_group<char_type, values_storage_type>;
    using argument_group_ptr_type = std::shared_ptr<argument_group_type>;

    struct arg_info
    {
        arg_info()
            : m_is_named()
        {
            // noop
        }

        bool m_is_named;
        string_vector_type m_short_names;
        string_vector_type m_long_names;
        string_type m_description;
        string_type m_value_name;
        string_type m_text;
    };

    using arg_info_vector = std::vector<arg_info>;

    enum class print_arg_mode
    {
        ALL,
        NAMED_ONLY,
        POSITIONAL_ONLY,
    };

    void print_group_arguments(ostream_type& os, const argument_group_type& group, const string_type& title,
        const string_type& description, print_arg_mode mode) const
    {
        arg_info_vector infos;

        if (mode != print_arg_mode::POSITIONAL_ONLY)
        {
            /* exclusive */
            for (auto& argument : group.get_arguments())
            {
                if (!argument->is_exclusive())
                {
                    continue;
                }

                infos.push_back(prepare_info(argument, false, false));
            }

            /* standard named */
            for (auto& argument : group.get_arguments())
            {
                if (argument->is_exclusive() || !argument->is_assignable_by_name())
                {
                    continue;
                }

                infos.push_back(prepare_info(argument, false, false));
            }
        }

        if (mode != print_arg_mode::NAMED_ONLY)
        {
            /* positional */
            for (auto& argument : group.get_arguments())
            {
                if (argument->is_assignable_by_name())
                {
                    continue;
                }

                infos.push_back(prepare_info(argument, true, false));
            }
        }

        if (infos.empty())
        {
            return;
        }

        build_infos(infos);

        print_infos(os, infos, title, description);
    }

    void print_default_named_arguments(ostream_type& os) const
    {
        print_group_arguments(os, m_data_ptr->m_default_argument_group,
            m_dictionary->get_string(usage_dictionary_string_key::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME), string_type(),
            print_arg_mode::NAMED_ONLY);
    }

    void print_default_positional_arguments(ostream_type& os) const
    {
        print_group_arguments(os, m_data_ptr->m_default_argument_group,
            m_dictionary->get_string(usage_dictionary_string_key::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME),
            string_type(), print_arg_mode::POSITIONAL_ONLY);
    }

    void print_groups(ostream_type& os) const
    {
        for (auto& group : m_data_ptr->m_argument_groups)
        {
            print_group_arguments(os, *group, group->get_name(), group->get_description(), print_arg_mode::ALL);
        }
    }

    bool has_args() const
    {
        return !m_data_ptr->m_argument_repository.m_arguments.empty()
            || m_data_ptr->m_argument_repository.m_subparsers_argument;
    }

    void print_usage_line(ostream_type& os) const
    {
        os << m_dictionary->get_string(usage_dictionary_string_key::USAGE_LEAD) << ':';

        if (m_data_ptr->m_argument_repository.m_arguments.empty()
            && !m_data_ptr->m_argument_repository.m_subparsers_argument)
        {
            os << ' ' << m_dictionary->get_string(usage_dictionary_string_key::USAGE_NO_ARGUMENTS) << std::endl;
            return;
        }

        print_usage_line_named_args(os);
        print_usage_line_positional_args(os);
        print_usage_line_subparser_args(os);

        os << std::endl;
    }

    void print_usage_line_named_args(ostream_type& os) const
    {
        std::size_t argument_count = 0;
        std::size_t exclusive_count = 0;
        bool any_required = false;
        bool multiple_allowed = false;

        for (auto& argument : m_data_ptr->m_argument_repository.m_arguments)
        {
            if (!argument->is_assignable_by_name())
            {
                continue;
            }

            ++argument_count;

            if (argument->is_exclusive())
            {
                ++exclusive_count;
            }

            if (argument->get_max_count() > 1)
            {
                multiple_allowed = true;
            }

            if (argument->get_min_count() > 0)
            {
                any_required = true;
            }
        }

        if (argument_count == 0)
        {
            return;
        }

        if (argument_count - exclusive_count > 1)
        {
            multiple_allowed = true;
        }

        const char optreq_open_char = any_required ? '<' : '[';
        const char optreq_close_char = any_required ? '>' : ']';

        os << ' ' << optreq_open_char << "OPTIONS" << optreq_close_char;
        if (multiple_allowed)
        {
            os << "...";
        }
    }

    void print_usage_line_positional_args(ostream_type& os) const
    {
        for (auto& argument : m_data_ptr->m_argument_repository.m_arguments)
        {
            if (argument->is_assignable_by_name())
            {
                continue;
            }

            bool is_required = (argument->get_min_count() > 0);
            bool multiple_allowed = (argument->get_max_count() > 1);

            const char optreq_open_char = is_required ? '<' : '[';
            const char optreq_close_char = is_required ? '>' : ']';

            os << ' ' << optreq_open_char << argument->get_first_name() << optreq_close_char;
            if (multiple_allowed)
            {
                os << "...";
            }
        }
    }

    void print_usage_line_subparser_args(ostream_type& os) const
    {
        if (m_data_ptr->m_argument_repository.m_subparsers_argument)
        {
            os << ' ' << '<' << m_data_ptr->m_argument_repository.m_subparsers_argument->get_first_name() << '>';

            bool any_args = false;

            for (auto& iter : m_data_ptr->m_argument_repository.m_subparsers_argument->get_parsers())
            {
                auto subusage = iter.second.get_usage();

                if (subusage.has_args())
                {
                    any_args = true;
                    break;
                }
            }

            if (any_args)
            {
                os << ' ' << '[' << "ARGS" << ']';
            }
        }
    }

    static bool is_short_name(const string_type& name)
    {
        auto len = name.size();
        auto first_not_dash = name.find_first_not_of('-');
        if (first_not_dash != string_type::npos)
        {
            len -= first_not_dash;
        }

        return (len <= 1);
    }

    void print_subparsers(ostream_type& os) const
    {
        if (m_data_ptr->m_argument_repository.m_subparsers_argument)
        {
            os << std::endl;
            os << m_data_ptr->m_argument_repository.m_subparsers_argument->get_first_name() << ':' << std::endl;

            std::size_t longest_name_len = 0;
            for (auto& iter : m_data_ptr->m_argument_repository.m_subparsers_argument->get_parsers())
            {
                longest_name_len = std::max(longest_name_len, iter.first.size());
            }

            for (auto& iter : m_data_ptr->m_argument_repository.m_subparsers_argument->get_parsers())
            {
                auto subusage = iter.second.get_usage();

                auto this_name_len = iter.first.size();

                os << "  " << iter.first << string_type(longest_name_len - this_name_len, ' ') << "  "
                   << subusage.m_data_ptr->m_usage_oneliner << std::endl;
            }
        }
    }

    void print_header(ostream_type& os) const
    {
        if (!m_data_ptr->m_usage_oneliner.empty())
        {
            os << std::endl;
            os << m_data_ptr->m_usage_oneliner << std::endl;
        }
        if (!m_data_ptr->m_usage_header.empty())
        {
            os << std::endl;
            os << m_data_ptr->m_usage_header << std::endl;
        }
    }

    void print_footer(ostream_type& os) const
    {
        if (!m_data_ptr->m_usage_footer.empty())
        {
            os << std::endl;
            os << m_data_ptr->m_usage_footer << std::endl;
        }
    }

    string_type prepare_description(const const_argument_ptr_type& argument) const
    {
        auto description = argument->get_description();

        if (m_decorators_enabled)
        {
            bool is_required = (argument->get_min_count() > 0) && (argument->get_default_values().size() == 0);
            bool is_multivalued = (argument->get_max_count() > 1);
            if (is_required || is_multivalued)
            {
                if (!description.empty())
                {
                    description += '\n';
                }
                if (is_required)
                {
                    description += '[';
                    if (argument->get_min_count() == 1)
                    {
                        description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_REQUIRED);
                    }
                    else
                    {
                        description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_MIN_COUNT);
                        description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_VALUE_SEPARATOR);
                        description += string_utils_type::to_string(argument->get_min_count());
                    }
                    description += ']';
                }
                if (is_multivalued)
                {
                    if (is_required)
                    {
                        description += ' ';
                    }

                    description += '[';
                    if (argument->is_max_count_unlimited())
                    {
                        description
                            += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_MAX_COUNT_UNLIMITED);
                    }
                    else
                    {
                        description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_MAX_COUNT);
                        description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_VALUE_SEPARATOR);
                        description += string_utils_type::to_string(argument->get_max_count());
                    }
                    description += ']';
                }
            }

            auto& default_values = argument->get_default_values();
            if (!default_values.empty())
            {
                if (!description.empty())
                {
                    description += '\n';
                }
                bool first = true;
                description += '[';
                description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_DEFAULT);
                description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_VALUE_SEPARATOR);
                for (auto& value : default_values)
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        description += ',';
                        description += ' ';
                    }
                    description += value;
                }
                description += ']';
            }

            auto& allowed_values = argument->get_allowed_values();
            if (!allowed_values.empty())
            {
                if (!description.empty())
                {
                    description += '\n';
                }
                bool first = true;
                description += '[';
                description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_ALLOWED);
                description += m_dictionary->get_string(usage_dictionary_string_key::DECORATOR_VALUE_SEPARATOR);
                for (auto& value : allowed_values)
                {
                    if (first)
                    {
                        first = false;
                    }
                    else
                    {
                        description += ',';
                        description += ' ';
                    }
                    description += value;
                }
                description += ']';
            }
        }

        return description;
    }

    arg_info prepare_info(const const_argument_ptr_type& argument, bool all_short, bool all_long) const
    {
        arg_info info;

        info.m_is_named = argument->is_assignable_by_name();

        for (auto& name : argument->get_names())
        {
            if (all_short)
            {
                info.m_short_names.emplace_back(name);
            }
            else if (all_long)
            {
                info.m_long_names.emplace_back(name);
            }
            else
            {
                if (is_short_name(name))
                {
                    info.m_short_names.emplace_back(name);
                }
                else
                {
                    info.m_long_names.emplace_back(name);
                }
            }
        }

        info.m_description = prepare_description(argument);
        info.m_value_name = argument->get_value_name();

        return info;
    }

    static void add_indent(arg_info_vector& infos, std::size_t indent_size)
    {
        for (auto& info : infos)
        {
            info.m_text += string_type(indent_size, ' ');
        }
    }

    static void add_short_names(arg_info_vector& infos)
    {
        for (auto& info : infos)
        {
            for (std::size_t i = 0; i < info.m_short_names.size(); ++i)
            {
                info.m_text += ' ';
                info.m_text += info.m_short_names[i];
                if ((i + 1 < info.m_short_names.size()) || !info.m_long_names.empty())
                {
                    info.m_text += ',';
                }
            }
            if (info.m_long_names.empty() && !info.m_value_name.empty())
            {
                info.m_text += '=';
                info.m_text += info.m_value_name;
            }
        }
    }

    static void add_layout_chars(arg_info_vector& infos, bool only_named)
    {
        std::size_t max_len = 0;
        for (auto& info : infos)
        {
            if (only_named && !info.m_is_named)
            {
                continue;
            }

            max_len = std::max(max_len, info.m_text.size());
        }

        for (auto& info : infos)
        {
            if (only_named && !info.m_is_named)
            {
                continue;
            }

            auto len = info.m_text.size();
            if (len < max_len)
            {
                info.m_text += string_type(max_len - len, ' ');
            }
        }
    }

    static void add_long_names(arg_info_vector& infos)
    {
        for (auto& info : infos)
        {
            for (std::size_t i = 0; i < info.m_long_names.size(); ++i)
            {
                info.m_text += ' ';
                info.m_text += info.m_long_names[i];
                if (i + 1 < info.m_long_names.size())
                {
                    info.m_text += ',';
                }
            }
            if (!info.m_long_names.empty() && !info.m_value_name.empty())
            {
                info.m_text += '=';
                info.m_text += info.m_value_name;
            }
        }
    }

    void add_description(arg_info_vector& infos) const
    {
        for (auto& info : infos)
        {
            std::size_t prefix_len = info.m_text.size() + m_multiline_indent;

            for (std::size_t start_pos = 0;;)
            {
                if (start_pos > 0)
                {
                    info.m_text += string_type(prefix_len, ' ');
                }

                auto sep_pos = info.m_description.find('\n', start_pos);
                if (sep_pos != string_type::npos)
                {
                    ++sep_pos;
                    info.m_text += info.m_description.substr(start_pos, sep_pos - start_pos);
                    start_pos = sep_pos;
                }
                else
                {
                    info.m_text += info.m_description.substr(start_pos);
                    break;
                }
            }
        }
    }

    void print_infos(
        ostream_type& os, arg_info_vector& infos, const string_type& header, const string_type& description) const
    {
        os << std::endl;
        os << header << ':' << std::endl;
        if (!description.empty())
        {
            for (typename string_type::size_type start_pos = 0; start_pos < description.size();)
            {
                auto end_pos = description.find('\n', start_pos);
                if (end_pos == string_type::npos)
                {
                    os << string_type(m_multiline_indent, ' ') << description.substr(start_pos) << std::endl;
                    break;
                }
                else
                {
                    os << string_type(m_multiline_indent, ' ') << description.substr(start_pos, end_pos - start_pos)
                       << std::endl;
                    start_pos = end_pos + 1;
                }
            }
        }
        for (auto& info : infos)
        {
            os << info.m_text << std::endl;
        }
    }

    void build_infos(arg_info_vector& infos) const
    {
        add_indent(infos, m_arg_line_indent);
        add_short_names(infos);
        add_layout_chars(infos, true);
        add_long_names(infos);
        add_layout_chars(infos, false);
        add_indent(infos, m_description_indent);
        add_description(infos);
    }

    const const_parser_data_ptr_type m_data_ptr;
    bool m_decorators_enabled;
    std::size_t m_description_indent;
    std::size_t m_multiline_indent;
    std::size_t m_arg_line_indent;
    dictionary_ptr_type m_dictionary;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_USAGE_HPP_
