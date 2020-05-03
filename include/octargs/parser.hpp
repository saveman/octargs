#ifndef OCTARGS_PARSER_HPP_
#define OCTARGS_PARSER_HPP_

#include <memory>
#include <string>
#include <vector>

#include "argument_group.hpp"
#include "argument_table.hpp"
#include "dictionary.hpp"
#include "exception.hpp"
#include "parser_error.hpp"
#include "results.hpp"
#include "usage.hpp"

#include "internal/argument.hpp"
#include "internal/parser_engine.hpp"

namespace oct
{
namespace args
{

/// \brief Arguments parser
///
/// \tparam derived_T           derived (parser) type
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
template <typename derived_T, typename char_T, typename values_storage_T>
class basic_parser_base
{
public:
    using derived_type = derived_T;

    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using dictionary_type = dictionary<char_type>;
    using const_dictionary_ptr_type = std::shared_ptr<const dictionary_type>;

    using argument_group_type = basic_argument_group<char_type, values_storage_type>;

    using exclusive_argument_type = basic_exclusive_argument<char_type, values_storage_type>;
    using positional_argument_type = basic_positional_argument<char_type, values_storage_type>;
    using subparser_argument_type = basic_subparser_argument<char_type, values_storage_type>;
    using switch_argument_type = basic_switch_argument<char_type, values_storage_type>;
    using valued_argument_type = basic_valued_argument<char_type, values_storage_type>;

    using argument_table_type = basic_argument_table<char_type>;
    using results_type = basic_results<char_type>;

    using parser_usage_type = basic_parser_usage<char_type, values_storage_type>;

    parser_usage_type get_usage() const
    {
        return parser_usage_type(m_data_ptr);
    }

    derived_type& set_usage_oneliner(const string_type& text)
    {
        m_data_ptr->m_usage_oneliner = text;
        return cast_this_to_derived();
    }

    derived_type& set_usage_header(const string_type& text)
    {
        m_data_ptr->m_usage_header = text;
        return cast_this_to_derived();
    }

    derived_type& set_usage_footer(const string_type& text)
    {
        m_data_ptr->m_usage_footer = text;
        return cast_this_to_derived();
    }

    argument_group_type add_group(const std::string& name)
    {
        return m_data_ptr->add_group(name);
    }

    exclusive_argument_type add_exclusive(const string_vector_type& names)
    {
        return m_data_ptr->get_default_argument_group().add_exclusive(names);
    }

    switch_argument_type add_switch(const string_vector_type& names)
    {
        return m_data_ptr->get_default_argument_group().add_switch(names);
    }

    valued_argument_type add_valued(const string_vector_type& names)
    {
        return m_data_ptr->get_default_argument_group().add_valued(names);
    }

    positional_argument_type add_positional(const string_type& name)
    {
        return m_data_ptr->get_default_argument_group().add_positional(name);
    }

    subparser_argument_type add_subparsers(const string_type& name)
    {
        return subparser_argument_type(m_data_ptr->add_subparsers(name));
    }

protected:
    using parser_data_type = internal::basic_parser_data<char_type, values_storage_type>;
    using parser_data_ptr_type = std::shared_ptr<parser_data_type>;
    using const_parser_data_ptr_type = std::shared_ptr<const parser_data_type>;

    using storage_helper_type = internal::storage_handler_helper<char_type, values_storage_type>;

    using engine_type = internal::basic_parser_engine<char_type, values_storage_type>;

    explicit basic_parser_base(parser_data_ptr_type data_ptr)
        : m_data_ptr(data_ptr)
    {
        // noop
    }

    derived_type& cast_this_to_derived()
    {
        return static_cast<derived_T&>(*this);
    }

    results_type parse_internal(const argument_table_type& arg_table, storage_helper_type& storage_helper) const
    {
        engine_type engine(arg_table, storage_helper, m_data_ptr);
        return engine.parse();
    }

private:
    parser_data_ptr_type m_data_ptr;
};

/// \brief Arguments parser
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
template <typename char_T, typename values_storage_T>
class basic_parser : public basic_parser_base<basic_parser<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type = basic_parser_base<basic_parser<char_T, values_storage_T>, char_T, values_storage_T>;

    using dictionary_type = dictionary<char_type>;
    using const_dictionary_ptr_type = std::shared_ptr<const dictionary_type>;

    using argument_table_type = basic_argument_table<char_type>;

    using results_type = basic_results<char_type>;

    using parser_data_type = typename base_type::parser_data_type;
    using parser_data_ptr_type = typename base_type::parser_data_ptr_type;

    explicit basic_parser()
        : basic_parser(std::make_shared<default_dictionary<char_type>>())
    {
        // noop
    }

    explicit basic_parser(const const_dictionary_ptr_type& dictionary)
        : basic_parser(parser_data_type::create(dictionary))
    {
        // noop
    }

    explicit basic_parser(parser_data_ptr_type data_ptr)
        : base_type(data_ptr)
    {
        // noop
    }

    results_type parse(int argc, char_type* argv[], values_storage_type& values_storage) const
    {
        return this->parse_internal(argument_table_type(argc, argv), values_storage);
    }

    results_type parse(int argc, const char_type* argv[], values_storage_type& values_storage) const
    {
        return this->parse_internal(argument_table_type(argc, argv), values_storage);
    }

    results_type parse(const argument_table_type& arg_table, values_storage_type& values_storage) const
    {
        return this->parse_internal(arg_table, values_storage);
    }

private:
    results_type parse_internal(const argument_table_type& arg_table, values_storage_type& values_storage) const
    {
        typename base_type::storage_helper_type helper(values_storage);
        return base_type::parse_internal(arg_table, helper);
    }
};

/// \brief Arguments parser
///
/// \tparam char_T              char type (as in std::basic_string)
template <typename char_T>
class basic_parser<char_T, void> : public basic_parser_base<basic_parser<char_T, void>, char_T, void>
{
public:
    using char_type = char_T;
    using values_storage_type = void;

    using base_type = basic_parser_base<basic_parser<char_T, void>, char_T, void>;

    using dictionary_type = dictionary<char_type>;
    using const_dictionary_ptr_type = std::shared_ptr<const dictionary_type>;

    using argument_table_type = basic_argument_table<char_type>;

    using results_type = basic_results<char_type>;

    using parser_data_type = typename base_type::parser_data_type;
    using parser_data_ptr_type = typename base_type::parser_data_ptr_type;

    explicit basic_parser()
        : basic_parser(std::make_shared<default_dictionary<char_type>>())
    {
        // noop
    }

    explicit basic_parser(const const_dictionary_ptr_type& dictionary)
        : basic_parser(parser_data_type::create(dictionary))
    {
        // noop
    }

    explicit basic_parser(const parser_data_ptr_type& data_ptr)
        : base_type(data_ptr)
    {
        // noop
    }

    results_type parse(int argc, char_type* argv[]) const
    {
        return this->parse_internal(argument_table_type(argc, argv));
    }

    results_type parse(int argc, const char_type* argv[]) const
    {
        return this->parse_internal(argument_table_type(argc, argv));
    }

    results_type parse(const argument_table_type& arg_table) const
    {
        return this->parse_internal(arg_table);
    }

private:
    results_type parse_internal(const argument_table_type& arg_table) const
    {
        typename base_type::storage_helper_type helper;
        return base_type::parse_internal(arg_table, helper);
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_HPP_
