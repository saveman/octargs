#ifndef OCTARGS_ARGUMENT_HPP_
#define OCTARGS_ARGUMENT_HPP_

#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "argument_type_handler.hpp"
#include "exception.hpp"
#include "internal/argument_handler.hpp"
#include "internal/misc.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename values_storage_T>
class basic_argument : public internal::basic_argument_tag
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;
    using handler_type = internal::basic_argument_handler<char_type, values_storage_type>;
    using const_handler_ptr_type = std::shared_ptr<const handler_type>;

    virtual ~basic_argument() = default;

    virtual const string_vector_type& get_names() const = 0;

    virtual const string_vector_type& get_default_values() const = 0;

    virtual const string_vector_type& get_allowed_values() const = 0;

    virtual std::size_t get_min_count() const = 0;

    virtual std::size_t get_max_count() const = 0;

    virtual bool is_max_count_unlimited() const = 0;

    virtual const const_handler_ptr_type& get_handler() const = 0;

    virtual const string_type& get_description() const = 0;

    virtual const string_type& get_value_name() const = 0;

    virtual bool is_exclusive() const = 0;

    virtual bool is_assignable_by_name() const = 0;

    virtual bool is_accepting_immediate_value() const = 0;

    virtual bool is_accepting_separate_value() const = 0;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_HPP_
