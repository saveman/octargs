#include <octargs/octargs.hpp>

#include <iostream>

using namespace oct::args;

//! [Snippet Data Storage]
struct data_storage
{
    data_storage()
        : m_help_requested(false)
        , m_verbose(false)
        , m_lines(10)
    {
    }

    bool m_help_requested;
    bool m_verbose;
    int m_lines;
    std::string m_header;
    std::vector<std::string> m_files;
};
//! [Snippet Data Storage]

//! [Snippet Parser Type]
using my_parser = storing_parser<data_storage>;
//! [Snippet Parser Type]

int main(int argc, char* argv[])
{
    try
    {
        //! [Snippet Build Parser]
        my_parser parser;

        parser.add_exclusive({ "--help" }).set_type_and_storage(&data_storage::m_help_requested);
        parser.add_switch({ "--verbose" }).set_type_and_storage(&data_storage::m_verbose);
        parser.add_valued({ "--lines" }).set_type_and_storage(&data_storage::m_lines);
        parser.add_valued({ "--header" }).set_type_and_storage(&data_storage::m_header);
        parser.add_positional("files").set_max_count_unlimited().set_type_and_storage(&data_storage::m_files);
        //! [Snippet Build Parser]

        //! [Snippet Parse]
        data_storage data;
        parser.parse(argc, argv, data);
        //! [Snippet Parse]

        if (data.m_help_requested)
        {
            std::cout << parser.get_usage() << std::endl;
            return 0;
        }

        // TODO: application logic
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error occurred: " << e.what() << std::endl;
    }

    return 0;
}
