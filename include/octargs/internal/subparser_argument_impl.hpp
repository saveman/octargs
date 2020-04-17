#ifndef OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_
#define OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_

#include <map>
#include <memory>

#include "argument_base_impl.hpp"

namespace oct
{
namespace args
{

// forward
template <typename char_T, typename values_storage_T>
class basic_parser;

namespace internal
{

template <typename char_T, typename values_storage_T = null_values_storage>
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

    explicit basic_subparser_argument_impl(const string_vector_type& names)
        : base_type(0, names)
    {
        this->set_min_count(1);
    }

    parser_type add_parser(const string_type& name)
    {
        if (has_parser(name))
        {
            // TODO: change exception?
            throw std::invalid_argument("name");
        }

        auto result = m_subparsers.emplace(name, parser_type());
        if (!result.second)
        {
            throw std::logic_error("Cannot emplace new parser");
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

    const std::map<string_type, parser_type>& get_parsers() const
    {
        return m_subparsers;
    }

private:
    std::map<string_type, parser_type> m_subparsers;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_
