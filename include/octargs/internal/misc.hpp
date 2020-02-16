#ifndef OCTARGS_MISC_HPP_
#define OCTARGS_MISC_HPP_

#include <exception>
#include <string>

#if defined(__GNUC__) || defined(__clang__)
#define DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED __declspec(deprecated)
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED
#endif

namespace oct
{
namespace args
{
namespace internal
{

struct null_values_storage
{
    // empty
};

class basic_argument_tag
{
};

} // namespace internal
} // namespace args
} // namespace oct

#endif /*OCTARGS_MISC_HPP_*/
