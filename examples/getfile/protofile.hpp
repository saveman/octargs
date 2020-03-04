#ifndef GETFILE_PROTOFILE_HPP_
#define GETFILE_PROTOFILE_HPP_

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "settings.hpp"

namespace oct_args_examples
{

class file_getter
{
public:
    file_getter(const common_settings& common_settings, const file_settings& file_settings)
        : m_common_settings(common_settings)
        , m_file_settings(file_settings)
    {
        // noop
    }

    int execute()
    {
        if (m_common_settings.m_verbose)
        {
            std::cerr << "FILE: path=" << m_file_settings.m_path << std::endl;
        }

        std::ifstream input_file(m_file_settings.m_path);
        if (!input_file.is_open())
        {
            std::cerr << "Failed to open file: " << m_file_settings.m_path << std::endl;
            return EXIT_FAILURE;
        }

        std::string line;
        while (std::getline(input_file, line))
        {
            std::cout << line << std::endl;
        }

        if (input_file.bad())
        {
            std::cerr << "Failed to read file: " << m_file_settings.m_path << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;
    }

private:
    const common_settings m_common_settings;
    const file_settings m_file_settings;
};

} // namespace oct_args_examples

#endif // GETFILE_PROTOFILE_HPP_
