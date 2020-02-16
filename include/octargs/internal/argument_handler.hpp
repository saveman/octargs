#ifndef OCTARGS_ARGUMENT_HANDLER_HPP_
#define OCTARGS_ARGUMENT_HANDLER_HPP_

namespace oct
{
namespace args
{
namespace internal
{

template <typename TRAITS, typename VALUES_STORAGE>
class basic_argument_handler
{
public:
    using string_type = typename TRAITS::string_type;
    using values_storage_type = VALUES_STORAGE;

    virtual ~basic_argument_handler() = default;
    virtual void parse(values_storage_type& storage, const string_type& value_str) const = 0;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_HANDLER_HPP_
