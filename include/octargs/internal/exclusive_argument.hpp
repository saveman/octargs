#ifndef OCTARGS_EXCLUSIVE_ARGUMENT_HPP_
#define OCTARGS_EXCLUSIVE_ARGUMENT_HPP_

#include "argument_base.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T = null_values_storage>
class basic_exclusive_argument
    : public basic_argument_base<basic_exclusive_argument<char_T, values_storage_T>, char_T, values_storage_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using base_type
        = basic_argument_base<basic_exclusive_argument<char_type, values_storage_type>, char_type, values_storage_type>;

    using dictionary_type = parser_dictionary<char_type>;
    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    explicit basic_exclusive_argument(const string_vector_type& names)
        : base_type(base_type::FLAG_IS_EXCLUSIVE | base_type::FLAG_IS_ASSIGNABLE_BY_NAME, names)
    {
        // noop
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_EXCLUSIVE_ARGUMENT_HPP_