#ifndef OCTARGS_APPLICATION_INFO_HPP_
#define OCTARGS_APPLICATION_INFO_HPP_

#include <string>

namespace oct
{
namespace args
{
    class application_info
    {
    public:
        application_info()
            : m_name()
            , m_description()
            , m_version()
        {
            // noop
        }

        const std::string& get_name() const
        {
            return m_name;
        }

        void set_name(const std::string& name)
        {
            m_name = name;
        }

        const std::string& get_description() const
        {
            return m_description;
        }

        void set_description(const std::string& description)
        {
            m_description = description;
        }

        const std::string& get_version() const
        {
            return m_version;
        }

        void set_version(const std::string& version)
        {
            m_version = version;
        }

    private:
        std::string m_name;
        std::string m_description;
        std::string m_version;
    };

} // namespace args
} // namespace oct

#endif /*OCTARGS_APPLICATION_INFO_HPP_*/
