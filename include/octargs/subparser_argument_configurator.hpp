#ifndef OCTARGS_SUBPARSER_ARGUMENT_CONFIGURATOR_HPP_
#define OCTARGS_SUBPARSER_ARGUMENT_CONFIGURATOR_HPP_

#include "argument_configurator.hpp"

#include "internal/subparser_argument.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename values_storage_T>
class basic_parser;

template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_subparser_argument_configurator
    : public basic_argument_configurator<basic_subparser_argument_configurator<char_T, values_storage_T, data_T>,
          internal::basic_subparser_argument<char_T, values_storage_T>, char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type
        = basic_argument_configurator<basic_subparser_argument_configurator<char_T, values_storage_T, data_T>,
            internal::basic_subparser_argument<char_T, values_storage_T>, char_T, values_storage_T, data_T>;

    using parser_type = basic_parser<char_type, values_storage_type>;

    using string_type = typename base_type::string_type;
    using argument_ptr_type = typename base_type::argument_ptr_type;

    explicit basic_subparser_argument_configurator(argument_ptr_type argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }

    parser_type& add_parser(const string_type& name)
    {
        return this->get_argument().add_parser(name);
    }
};

#if 0
/// \brief Subparser argument
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
// TODO: remove internal namespace from base (after argument moved also to internal)
template <typename char_T, typename values_storage_T = internal::null_values_storage>
class basic_subparser_argument
    : public internal::basic_argument_base<basic_subparser_argument<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    // TODO: as above
    using base_type = internal::basic_argument_base<basic_subparser_argument<char_type, values_storage_type>, char_type,
        values_storage_type>;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using parser_type = basic_parser<char_type, values_storage_type>;
    using const_parser_ptr_type = std::shared_ptr<const parser_type>;

    explicit basic_subparser_argument(const string_vector_type& names)
        : base_type(0, names)
    {
        this->set_min_count(1);
    }

    parser_type& add_parser(const string_type& name)
    {
        auto new_parser = std::make_shared<parser_type>();

        m_subparsers.emplace(name, new_parser);

        return *new_parser;
    }

    // TODO: hide the getters if possible?

    const_parser_ptr_type get_parser(const string_type& name) const
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

    const std::map<string_type, const_parser_ptr_type>& get_parsers() const
    {
        return m_subparsers;
    }

private:
    std::map<string_type, const_parser_ptr_type> m_subparsers;
};
#endif

} // namespace args
} // namespace oct

#endif // OCTARGS_SUBPARSER_ARGUMENT_CONFIGURATOR_HPP_
