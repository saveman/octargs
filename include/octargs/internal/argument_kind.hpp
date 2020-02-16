#ifndef OCTARGS_ARGUMENT_KIND_HPP_
#define OCTARGS_ARGUMENT_KIND_HPP_

namespace oct
{
namespace args
{
namespace internal
{

enum class argument_kind
{
    SWITCH,
    VALUED,
    POSITIONAL
};

}
} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_KIND_HPP_
