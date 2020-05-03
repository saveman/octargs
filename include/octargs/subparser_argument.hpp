#ifndef OCTARGS_SUBPARSER_ARGUMENT_HPP_
#define OCTARGS_SUBPARSER_ARGUMENT_HPP_

#include "argument_base.hpp"
#include "internal/subparser_argument_impl.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename values_storage_T>
class basic_parser;

/// \brief Subparser argument
///
/// Subparser argument is a 'virtual' argument that allows registering
/// subparsers so it is possible to implement a command based user interface
/// (e.g. like the one provided by "git" command).
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
/// \tparam data_T              argument value data type
template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_subparser_argument : public basic_argument_base<basic_subparser_argument,
                                     internal::basic_subparser_argument_impl, char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_argument_base<oct::args::basic_subparser_argument, internal::basic_subparser_argument_impl,
        char_T, values_storage_T, data_T>;

    using parser_type = basic_parser<char_type, values_storage_type>;

    using string_type = typename base_type::string_type;
    using argument_ptr_type = typename base_type::argument_ptr_type;

    explicit basic_subparser_argument(const argument_ptr_type& argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }

    template <typename handler_ptr_T>
    explicit basic_subparser_argument(const argument_ptr_type& argument_ptr, const handler_ptr_T& handler_ptr)
        : base_type(argument_ptr, handler_ptr)
    {
        // noop
    }

    parser_type add_parser(const string_type& name)
    {
        return this->get_argument().add_parser(name);
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_SUBPARSER_ARGUMENT_HPP_
