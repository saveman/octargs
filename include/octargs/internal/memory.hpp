#ifndef OCTARGS_MEMORY_HPP_
#define OCTARGS_MEMORY_HPP_

#include <memory>

namespace oct
{
namespace args
{
namespace internal
{

template <typename derived_T>
class enable_shared_from_this : public std::enable_shared_from_this<derived_T>
{
public:
    std::weak_ptr<derived_T> weak_from_this()
    {
        return std::weak_ptr<derived_T>( this->shared_from_this() );
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_MEMORY_HPP_
