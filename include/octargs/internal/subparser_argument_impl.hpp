#ifndef OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_
#define OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_

#include <map>
#include <memory>

#include "../exception.hpp"
#include "argument_base_impl.hpp"
#include "name.hpp"

namespace oct
{
namespace args
{

// forward
template <typename char_T, typename values_storage_T>
class basic_parser;

namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_subparser_argument_impl
    : public basic_argument_base_impl<basic_subparser_argument_impl<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type = basic_argument_base_impl<basic_subparser_argument_impl<char_type, values_storage_type>, char_type,
        values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using parser_type = basic_parser<char_type, values_storage_type>;
    using const_dictionary_ptr_type = typename parser_type::const_dictionary_ptr_type;

    using name_type = name<char_type>;
    using name_less_type = name_less<char_type>;

    using parsers_map_type = std::map<name_type, parser_type, name_less_type>;

    explicit basic_subparser_argument_impl(
        const const_dictionary_ptr_type& dictionary_ptr, const string_vector_type& names)
        : base_type(0, names)
        , m_dictionary_ptr(dictionary_ptr)
    {
        base_type::set_min_count(1);
    }

    parser_type add_parser(const string_type& name)
    {
        if (has_parser(name))
        {
            throw invalid_parser_name_ex<char_type>("Duplicated parser name", name);
        }

        auto result = m_subparsers.emplace(name, parser_type(m_dictionary_ptr));
        if (!result.second)
        {
            throw std::runtime_error("Cannot emplace new parser");
        }

        return result.first->second;
    }

    bool has_parser(const string_type& name) const
    {
        return m_subparsers.find(name) != m_subparsers.end();
    }

    parser_type get_parser(const string_type& name) const
    {
        auto iter = m_subparsers.find(name);
        if (iter == m_subparsers.end())
        {
            throw std::logic_error("No parser with given name exist");
        }
        return iter->second;
    }

    const parsers_map_type& get_parsers() const
    {
        return m_subparsers;
    }

private:
    const_dictionary_ptr_type m_dictionary_ptr;
    parsers_map_type m_subparsers;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_
