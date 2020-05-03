#ifndef OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_
#define OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_

#include <memory>

#include "../exception.hpp"
#include "argument_base_impl.hpp"
#include "string_utils.hpp"

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

    using parser_data_weak_ptr_type = typename base_type::parser_data_weak_ptr_type;

    explicit basic_subparser_argument_impl(parser_data_weak_ptr_type parser_data_ptr, const string_vector_type& names)
        : base_type(parser_data_ptr, base_type::ZERO_FLAGS, names)
    {
        base_type::set_min_count(1);
    }

    parser_type add_parser(const string_type& name)
    {
        auto parser_data = this->get_parser_data()->add_subparser(name);
        return parser_type(parser_data);
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_SUBPARSER_ARGUMENT_IMPL_HPP_
