#include <fstream>
#include <iostream>

#include "parser.hpp"

namespace oct_args_examples
{

class cat_app
{
public:
    int run(int argc, char* argv[])
    {
        oct::args::parser arg_parser;
        arg_parser.set_positional_arguments_enabled(true);

        // cat [OPTION]... [FILE]...
        //
        // Concatenate FILE(s) to standard output.
        // With no FILE, or when FILE is -, read standard input.

        auto& results = arg_parser.parse(argc, argv);

        if (results.get_positional_arguments().size() == 0)
        {
            cat_contents(std::cin);
        }
        else
        {
            for (const auto& argument : results.get_positional_arguments())
            {
                if (argument == "-")
                {
                    cat_contents(std::cin);
                }
                else
                {
                    cat_file(argument);
                }
            }
        }

        return 0;
    }

private:
    bool cat_file(const std::string& file_name)
    {
        std::ifstream input_file(file_name);
        return cat_contents(input_file);
    }

    bool cat_contents(std::istream& input_stream)
    {
        std::string line;

        if (input_stream.eof())
        {
            // already at end - exit with success
            return true;
        }

        while (std::getline(input_stream, line))
        {
            cat_line(line);
        }

        return input_stream.eof();
    }

    void cat_line(const std::string& line)
    {
        std::cout << line << std::endl;
    }
};

} // namespace oct_args_examples

int main(int argc, char* argv[])
{
    return oct_args_examples::cat_app().run(argc, argv);
}
