#ifndef GETFILE_SOCKETBSD_HPP_
#define GETFILE_SOCKETBSD_HPP_

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdint>
#include <iostream>
#include <string>

namespace oct_args_examples
{

class socket
{
public:
    socket()
        : m_handle(-1)
    {
        // noop
    }

    ~socket()
    {
        close();
    }

    bool open()
    {
        m_handle = ::socket(AF_INET, SOCK_STREAM, 0);
        if (m_handle == -1)
        {
            std::cerr << "Failed to open socket" << std::endl;
            return false;
        }
        return true;
    }

    void close()
    {
        if (m_handle != -1)
        {
            ::close(m_handle);
            m_handle = -1;
        }
    }

    bool connect(const std::string& host, std::uint16_t port)
    {
        sockaddr_in host_address;
        std::memset(&host_address, 0, sizeof(host_address));

        host_address.sin_family = AF_INET;

        if (::inet_pton(AF_INET, host.c_str(), &host_address.sin_addr.s_addr) != 1)
        {
            std::cerr << "Failed to convert host address: " << host << std::endl;
            return false;
        }

        host_address.sin_port = htons(port);

        if (::connect(m_handle, reinterpret_cast<const sockaddr*>(&host_address),
                static_cast<socklen_t>(sizeof(host_address)))
            != 0)
        {
            std::cerr << "Failed to connect to host: " << host << std::endl;
            return false;
        }

        return true;
    }

    bool send_request(const std::string& request)
    {
        auto request_str = request.c_str();
        auto request_len = static_cast<int>(request.size());

        if (::send(m_handle, request_str, request_len, 0) != request_len)
        {
            std::cerr << "Failed to send request" << std::endl;
            return false;
        }

        return true;
    }

    bool receive_response(std::string& response)
    {
        bool success = false;

        response.clear();

        for (;;)
        {
            char buffer[1024];

            int bytes_read = ::recv(m_handle, buffer, sizeof(buffer), 0);
            if (bytes_read > 0)
            {
                response.append(&buffer[0], &buffer[bytes_read]);
            }
            else if (bytes_read == 0)
            {
                success = true;
                break;
            }
            else
            {
                std::cerr << "Failed to read response" << std::endl;
                break;
            }
        }

        return success;
    }

private:
    int m_handle;
};

} // namespace oct_args_examples

#endif // GETFILE_SOCKETBSD_HPP_
