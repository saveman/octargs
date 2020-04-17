#ifndef OCTARGS_VALUED_ARGUMENT_CONFIGURATOR_HPP_
#define OCTARGS_VALUED_ARGUMENT_CONFIGURATOR_HPP_

#include "argument_configurator.hpp"

#include "internal/valued_argument.hpp"

namespace oct
{
namespace args
{

template <typename char_T, typename values_storage_T, typename data_T = void>
class basic_valued_argument_configurator
    : public basic_argument_configurator<basic_valued_argument_configurator<char_T, values_storage_T, data_T>,
          internal::basic_valued_argument<char_T, values_storage_T>, char_T, values_storage_T, data_T>
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;
    using data_type = data_T;

    using base_type = basic_argument_configurator<basic_valued_argument_configurator<char_T, values_storage_T, data_T>,
        internal::basic_valued_argument<char_T, values_storage_T>, char_T, values_storage_T, data_T>;

    using string_type = typename base_type::string_type;
    using string_vector_type = typename base_type::string_vector_type;
    using argument_ptr_type = typename base_type::argument_ptr_type;

    explicit basic_valued_argument_configurator(argument_ptr_type argument_ptr)
        : base_type(argument_ptr)
    {
        // noop
    }

    basic_valued_argument_configurator& set_default_values(const string_vector_type& values)
    {
        this->get_argument().set_default_values(values);
        return this->cast_this_to_derived();
    }

    basic_valued_argument_configurator& set_default_value(const string_type& value)
    {
        this->get_argument().set_default_value(value);
        return this->cast_this_to_derived();
    }

    basic_valued_argument_configurator& set_allowed_values(const string_vector_type& values)
    {
        this->get_argument().set_allowed_values(values);
        return this->cast_this_to_derived();
    }

    basic_valued_argument_configurator& set_value_name(const string_type& name)
    {
        this->get_argument().set_value_name(name);
        return this->cast_this_to_derived();
    }

    basic_valued_argument_configurator& set_min_count(std::size_t count)
    {
        this->get_argument().set_min_count(count);
        return this->cast_this_to_derived();
    }

    basic_valued_argument_configurator& set_max_count(std::size_t count)
    {
        this->get_argument().set_max_count(count);
        return this->cast_this_to_derived();
    }

    basic_valued_argument_configurator& set_max_count_unlimited()
    {
        this->get_argument().set_max_count_unlimited();
        return this->cast_this_to_derived();
    }
};

} // namespace args
} // namespace oct

#endif // OCTARGS_VALUED_ARGUMENT_CONFIGURATOR_HPP_
