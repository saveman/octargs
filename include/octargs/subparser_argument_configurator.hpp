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

/// \brief Configurator for subparser argument
///
/// Subparser argument is a 'virtual' argument that allows registering
/// subparsers so it is possible to implement a command based user interface
/// (e.g. like the one provided by "git" command).
///
/// \tparam char_T              char type (as in std::basic_string)
/// \tparam values_storage_T    type of class uses as a storage for parsed values
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

    parser_type add_parser(const string_type& name)
    {
        return this->get_argument().add_parser(name);
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_SUBPARSER_ARGUMENT_CONFIGURATOR_HPP_
