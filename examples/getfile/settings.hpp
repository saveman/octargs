#ifndef GETFILE_SETTINGS_HPP_
#define GETFILE_SETTINGS_HPP_

#include <cstdint>
#include <string>

namespace oct_args_examples
{

struct common_settings
{
    bool m_verbose;

    common_settings()
        : m_verbose(false)
    {
        // noop
    }
};

struct file_settings
{
    bool m_help_requested;
    std::string m_path;

    file_settings()
        : m_help_requested(false)
        , m_path()
    {
        // noop
    }
};

struct http_settings
{
    bool m_help_requested;
    std::string m_host;
    std::uint16_t m_port;
    std::string m_path;

    http_settings()
        : m_help_requested(false)
        , m_host()
        , m_port(0)
        , m_path()
    {
        // noop
    }
};

struct app_settings
{
    bool m_help_requested;
    std::string m_protocol;

    common_settings m_common;
    file_settings m_file;
    http_settings m_http;

    app_settings()
        : m_help_requested(false)
        , m_protocol()
        , m_common()
        , m_file()
        , m_http()
    {
        // noop
    }
};

} // namespace oct_args_examples

#endif // GETFILE_SETTINGS_HPP_
