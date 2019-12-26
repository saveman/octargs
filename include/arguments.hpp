#ifndef OCTARGS_ARGUMENTS_HPP_
#define OCTARGS_ARGUMENTS_HPP_

#include <string>
#include <vector>

namespace oct
{
namespace args
{

class arguments
{
public:
    arguments(int argc, char* argv[]) :
            m_app_name(argv[0]),
            m_arguments(&argv[1], &argv[argc])
    {
        // noop
    }

    arguments(const std::string& app_name, const std::vector<std::string>& arguments) :
            m_app_name(app_name),
            m_arguments(arguments)
    {
        // noop
    }

    const std::string& get_app_name() const
    {
        return m_app_name;
    }

    std::size_t get_argument_count() const
    {
        return m_arguments.size();
    }

    const std::string& get_argument(std::size_t index) const
    {
        return m_arguments[index];
    }

private:
    using argument_collection = std::vector<std::string>;

    std::string m_app_name;
    argument_collection m_arguments;

};

} // namespace args
} // namespace oct

#endif /*OCTARGS_ARGUMENTS_HPP_*/
