#ifndef OCTARGS_SUBPARSER_ARGUMENT_HPP_
#define OCTARGS_SUBPARSER_ARGUMENT_HPP_

#include <map>
#include <memory>

#include "argument_base.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename values_storage_T>
class basic_parser;

template <typename char_T, typename values_storage_T>
class basic_parser_usage;

template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_subparser_argument
    : public basic_argument_base<basic_subparser_argument<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type
        = basic_argument_base<basic_subparser_argument<char_type, values_storage_type>, char_type, values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using parser_type = basic_parser<char_type, values_storage_type>;
    using parser_ptr_type = std::shared_ptr<parser_type>;

    using usage_type = basic_parser_usage<char_type, values_storage_type>;

    friend parser_type;
    friend usage_type;

    explicit basic_subparser_argument(const string_vector_type& names)
        : base_type(0, names)
    {
        this->set_min_count(1);
    }

    parser_type& add_parser(const string_type& name)
    {
        parser_ptr_type new_parser = std::make_shared<parser_type>();

        m_subparsers.emplace(name, new_parser);

        return *new_parser;
    }

private:
    parser_ptr_type get_parser(const string_type& name) const
    {
        auto iter = m_subparsers.find(name);
        if (iter != m_subparsers.end())
        {
            return iter->second;
        }
        else
        {
            return nullptr;
        }
    }

    const std::map<string_type, parser_ptr_type>& get_parsers() const
    {
        return m_subparsers;
    }

    std::map<string_type, parser_ptr_type> m_subparsers;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_SUBPARSER_ARGUMENT_HPP_
