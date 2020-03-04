#ifndef GETFILE_PROTOHTTP_HPP_
#define GETFILE_PROTOHTTP_HPP_

#include <cstdlib>
#include <cstring>
#include <iostream>

#include "settings.hpp"

#ifdef __linux__
#include "socketbsd.hpp"
#else
#include "socketwin.hpp"
#endif

namespace oct_args_examples
{

class http_getter
{
public:
    http_getter(const common_settings& common_settings, const http_settings& http_settings)
        : m_common_settings(common_settings)
        , m_http_settings(http_settings)
    {
        // noop
    }

    int execute()
    {
        if (m_common_settings.m_verbose)
        {
            std::cerr << "HTTP: host= " << m_http_settings.m_host << " port=" << m_http_settings.m_port
                      << " path=" << m_http_settings.m_path << std::endl;
        }

        int return_code = EXIT_FAILURE;

        socket http_socket;

        if (http_socket.open())
        {
            if (m_common_settings.m_verbose)
            {
                std::cerr << "Socket opened" << std::endl;
            }

            if (http_socket.connect(m_http_settings.m_host, m_http_settings.m_port))
            {
                if (m_common_settings.m_verbose)
                {
                    std::cerr << "Socket connected to: " << m_http_settings.m_host << std::endl;
                }

                std::string request;

                request += "GET ";
                request += m_http_settings.m_path;
                request += " HTTP/1.0";
                request += "\r\n";
                request += "Host: ";
                request += m_http_settings.m_host;
                request += "\r\n";
                request += "User-agent: getfile/1.0";
                request += "\r\n";
                request += "\r\n";

                if (http_socket.send_request(request))
                {
                    if (m_common_settings.m_verbose)
                    {
                        std::cerr << "Request sent to: " << m_http_settings.m_host << std::endl;
                    }

                    std::string response;

                    if (http_socket.receive_response(response))
                    {
                        if (m_common_settings.m_verbose)
                        {
                            std::cerr << "Read response from: " << m_http_settings.m_host << std::endl;
                        }

                        const std::string success_pattern("HTTP/1.0 200 OK\r\n");
                        if (response.substr(0, success_pattern.size()) == success_pattern)
                        {
                            auto header_end_pos = response.find("\r\n\r\n");
                            if (header_end_pos != std::string::npos)
                            {
                                header_end_pos += 4;

                                std::cout << response.substr(header_end_pos);
                            }
                            else
                            {
                                std::cerr << "Cannot find body in received response" << std::endl;
                            }
                        }
                        else
                        {
                            std::cerr << "Received response is not 200 OK" << std::endl;
                        }
                    }
                }
            }
        }

        return return_code;
    }

private:
    const common_settings m_common_settings;
    const http_settings m_http_settings;
};

} // namespace oct_args_examples

#endif // GETFILE_PROTOHTTP_HPP_
