#ifndef OCTARGS_ARGUMENT_HANDLER_HPP_
#define OCTARGS_ARGUMENT_HANDLER_HPP_

#include "../parser_dictionary.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_argument_handler
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_T>;

    using dictionary_type = parser_dictionary<char_type>;

    virtual ~basic_argument_handler() = default;

    virtual void parse(
        values_storage_type& storage, const dictionary_type& dictionary, const string_type& value_str) const = 0;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_HANDLER_HPP_
