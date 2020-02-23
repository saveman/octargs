#ifndef OCTARGS_PARSER_USAGE_HPP_
#define OCTARGS_PARSER_USAGE_HPP_

#include <iterator>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "argument.hpp"
#include "argument_table.hpp"
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

template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_parser_usage
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using argument_type = basic_argument<char_type, values_storage_type>;
    using argument_ptr_type = std::shared_ptr<const argument_type>;

    using string_type = std::basic_string<char_type>;
    using ostream_type = std::basic_ostream<char_type>;

    using string_vector_type = std::vector<string_type>;

    using parser_data_type = internal::basic_parser_data<char_type, values_storage_type>;
    using parser_data_ptr_type = std::shared_ptr<const parser_data_type>;

    using string_utils_type = internal::string_utils<char_type>;

    using dictionary_type = usage_dictionary<char_type>;
    using dictionary_ptr_type = std::shared_ptr<dictionary_type>;

    explicit basic_parser_usage(const parser_data_ptr_type& parser_data)
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
        usage.print_named_arguments(os);
        usage.print_positional_arguments(os);

        usage.print_footer(os);

        return os;
    }

private:
    struct arg_info
    {
        string_vector_type m_short_names;
        string_vector_type m_long_names;
        string_type m_description;
        string_type m_value_name;
        string_type m_text;
    };

    using arg_info_vector = std::vector<arg_info>;

    void print_usage_line(ostream_type& os) const
    {
        os << "Usage:";

        if (m_data_ptr->m_named_arguments.empty() && m_data_ptr->m_positional_arguments.empty()
            && m_data_ptr->m_exclusive_arguments.empty())
        {
            os << " --" << std::endl;
            return;
        }

        if (!m_data_ptr->m_named_arguments.empty())
        {
            bool any_required = false;
            bool multiple_allowed = false;

            if (m_data_ptr->m_named_arguments.size() > 1)
            {
                multiple_allowed = true;
            }
            else if (m_data_ptr->m_named_arguments[0]->get_max_count() > 1)
            {
                multiple_allowed = true;
            }

            for (auto& argument : m_data_ptr->m_positional_arguments)
            {
                if (argument->get_min_count() > 0)
                {
                    any_required = true;
                }
            }

            const char optreq_open_char = any_required ? '<' : '[';
            const char optreq_close_char = any_required ? '>' : ']';

            os << ' ' << optreq_open_char << "OPTIONS" << optreq_close_char;
            if (multiple_allowed)
            {
                os << "...";
            }
        }

        for (auto& argument : m_data_ptr->m_positional_arguments)
        {
            bool is_required = (argument->get_min_count() > 0);
            bool multiple_allowed = (argument->get_max_count() > 1);

            const char optreq_open_char = is_required ? '<' : '[';
            const char optreq_close_char = is_required ? '>' : ']';

            os << ' ' << optreq_open_char << argument->get_names()[0] << optreq_close_char;
            if (multiple_allowed)
            {
                os << "...";
            }
        }
        os << std::endl;
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

    void print_named_arguments(ostream_type& os) const
    {
        if (!m_data_ptr->m_named_arguments.empty())
        {
            arg_info_vector infos;

            for (auto& argument : m_data_ptr->m_exclusive_arguments)
            {
                // cppcheck-suppress useStlAlgorithm
                infos.push_back(prepare_info(argument, false));
            }
            for (auto& argument : m_data_ptr->m_named_arguments)
            {
                // cppcheck-suppress useStlAlgorithm
                infos.push_back(prepare_info(argument, false));
            }

            build_infos(infos);

            output_infos(os, infos,
                m_dictionary->get_string(usage_dictionary_string_key::USAGE_DEFAULT_NAMED_ARGUMENTS_GROUP_NAME));
        }
    }

    void print_positional_arguments(ostream_type& os) const
    {
        if (!m_data_ptr->m_positional_arguments.empty())
        {
            arg_info_vector infos;

            for (auto& argument : m_data_ptr->m_positional_arguments)
            {
                // cppcheck-suppress useStlAlgorithm
                infos.push_back(prepare_info(argument, true));
            }

            build_infos(infos);

            output_infos(os, infos,
                m_dictionary->get_string(usage_dictionary_string_key::USAGE_DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME));
        }
    }

    void print_header(ostream_type& os) const
    {
        if (!m_data_ptr->m_info_header.empty())
        {
            os << std::endl;
            os << m_data_ptr->m_info_header << std::endl;
        }
    }

    void print_footer(ostream_type& os) const
    {
        if (!m_data_ptr->m_info_footer.empty())
        {
            os << std::endl;
            os << m_data_ptr->m_info_footer << std::endl;
        }
    }

    string_type prepare_description(const argument_ptr_type& argument) const
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
                        description += m_dictionary->get_string(usage_dictionary_string_key::USAGE_DECORATOR_REQUIRED);
                    }
                    else
                    {
                        description += m_dictionary->get_string(usage_dictionary_string_key::USAGE_DECORATOR_MIN_COUNT);
                        description
                            += m_dictionary->get_string(usage_dictionary_string_key::USAGE_DECORATOR_VALUE_SEPARATOR);
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
                        description += m_dictionary->get_string(
                            usage_dictionary_string_key::USAGE_DECORATOR_MAX_COUNT_UNLIMITED);
                    }
                    else
                    {
                        description += m_dictionary->get_string(usage_dictionary_string_key::USAGE_DECORATOR_MAX_COUNT);
                        description
                            += m_dictionary->get_string(usage_dictionary_string_key::USAGE_DECORATOR_VALUE_SEPARATOR);
                        description += string_utils_type::to_string(argument->get_max_count());
                    }
                    description += ']';
                }
            }
        }

        return description;
    }

    arg_info prepare_info(const argument_ptr_type& argument, bool all_long) const
    {
        arg_info info;

        for (auto& name : argument->get_names())
        {
            if (!all_long && is_short_name(name))
            {
                info.m_short_names.emplace_back(name);
            }
            else
            {
                info.m_long_names.emplace_back(name);
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

    static void add_layout_chars(arg_info_vector& infos)
    {
        std::size_t max_len = 0;
        for (auto& info : infos)
        {
            max_len = std::max(max_len, info.m_text.size());
        }

        for (auto& info : infos)
        {
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

    static void output_infos(ostream_type& os, arg_info_vector& infos, const string_type& header)
    {
        os << std::endl;
        os << header << ':' << std::endl;
        for (auto& info : infos)
        {
            os << info.m_text << std::endl;
        }
    }

    void build_infos(arg_info_vector& infos) const
    {
        add_indent(infos, m_arg_line_indent);
        add_short_names(infos);
        add_layout_chars(infos);
        add_long_names(infos);
        add_layout_chars(infos);
        add_indent(infos, m_description_indent);
        add_description(infos);
    }

    const parser_data_ptr_type m_data_ptr;
    bool m_decorators_enabled;
    std::size_t m_description_indent;
    std::size_t m_multiline_indent;
    std::size_t m_arg_line_indent;
    dictionary_ptr_type m_dictionary;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_USAGE_HPP_
