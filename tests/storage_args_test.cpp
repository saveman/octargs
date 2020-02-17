#include "test_fixture.hpp"

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

class storage_args_test : public test_fixture
{
private:
    void test_single_value()
    {
        argument_table args1("appname",
            {
                "--compress=zip",
                "--verbose",
                "--output=myfile.txt",
                "filename.txt",
            });

        struct settings
        {
            bool m_verbose;
            std::string m_compression_name;
            std::string m_output_file;
            std::vector<std::string> m_files;
        };

        storing_parser<settings> parser;

        parser.add_switch({ "--verbose" }).set_type_and_storage(&settings::m_verbose);
        parser.add_valued({ "--compress" }).set_type_and_storage(&settings::m_compression_name);
        parser.add_valued({ "--output" }).set_type_and_storage(&settings::m_output_file);
        parser.add_positional("FILES", false, false).set_type_and_storage(&settings::m_files);

        settings settings1;
        parser.parse(args1, settings1);

        CPPUNIT_ASSERT_EQUAL(true, settings1.m_verbose);
        CPPUNIT_ASSERT_EQUAL(std::string("zip"), settings1.m_compression_name);
        CPPUNIT_ASSERT_EQUAL(std::string("myfile.txt"), settings1.m_output_file);
        CPPUNIT_ASSERT_EQUAL(std::size_t(1), settings1.m_files.size());
        CPPUNIT_ASSERT_EQUAL(std::string("filename.txt"), settings1.m_files[0]);
    }

    void test_multi_value()
    {
        argument_table args1("appname",
            {
                "--compress=zip",
                "--include=file1.h",
                "--verbose",
                "--include=file2.h",
                "--include=file3.h",
                "filename1.txt",
                "filename2.txt",
            });

        struct settings
        {
            bool m_verbose;
            std::string m_compression_name;
            std::vector<std::string> m_include_files;
            std::vector<std::string> m_files;
        };

        storing_parser<settings> parser;

        parser.add_switch({ "--verbose" }).set_type_and_storage(&settings::m_verbose);
        parser.add_valued({ "--compress" }).set_type_and_storage(&settings::m_compression_name);
        parser.add_valued({ "--include" }).set_unlimited_count().set_type_and_storage(&settings::m_include_files);
        parser.add_positional("FILES", false, true).set_type_and_storage(&settings::m_files);

        settings settings1;
        parser.parse(args1, settings1);

        CPPUNIT_ASSERT_EQUAL(true, settings1.m_verbose);
        CPPUNIT_ASSERT_EQUAL(std::string("zip"), settings1.m_compression_name);
        CPPUNIT_ASSERT_EQUAL(std::size_t(3), settings1.m_include_files.size());
        CPPUNIT_ASSERT_EQUAL(std::string("file1.h"), settings1.m_include_files[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("file2.h"), settings1.m_include_files[1]);
        CPPUNIT_ASSERT_EQUAL(std::string("file3.h"), settings1.m_include_files[2]);
        CPPUNIT_ASSERT_EQUAL(std::size_t(2), settings1.m_files.size());
        CPPUNIT_ASSERT_EQUAL(std::string("filename1.txt"), settings1.m_files[0]);
        CPPUNIT_ASSERT_EQUAL(std::string("filename2.txt"), settings1.m_files[1]);
    }

    void test_multi_stored_in_single()
    {
        argument_table args1("appname", { "0", "1", "2" });

        struct settings
        {
            int m_value;
        };

        storing_parser<settings> parser;

        parser.add_positional("FILES", false, true).set_type_and_storage(&settings::m_value);

        settings settings1;
        parser.parse(args1, settings1);

        CPPUNIT_ASSERT_EQUAL(int(2), settings1.m_value);
    }

    void test_types()
    {
        argument_table args1("appname",
            {
                "--int=5",
                "--int=0x14",
                "--double=-17.43",
                "--int=0765",
                "--longlong=12345678901",
            });

        struct settings
        {
            std::vector<int> m_ints;
            double m_double;
            long long m_longlong;
        };

        storing_parser<settings> parser;

        parser.add_valued({ "--int" }).set_unlimited_count().set_type_and_storage(&settings::m_ints);
        parser.add_valued({ "--double" }).set_type_and_storage(&settings::m_double);
        parser.add_valued({ "--longlong" }).set_type_and_storage(&settings::m_longlong);

        settings settings1;
        parser.parse(args1, settings1);

        CPPUNIT_ASSERT_EQUAL(std::size_t(3), settings1.m_ints.size());
        CPPUNIT_ASSERT_EQUAL(int(5), settings1.m_ints[0]);
        CPPUNIT_ASSERT_EQUAL(int(0x14), settings1.m_ints[1]);
        CPPUNIT_ASSERT_EQUAL(int(0765), settings1.m_ints[2]);
        CPPUNIT_ASSERT_EQUAL(double(-17.43), settings1.m_double);
        CPPUNIT_ASSERT_EQUAL((long long)(12345678901), settings1.m_longlong);
    }

    void test_null_convert_function()
    {
        argument_table args1("appname",
            {
                "--double=-17.43",
            });

        struct settings
        {
            double m_double;
        };

        storing_parser<settings> parser;
        auto& handler = parser.add_valued({ "--double" }).set_type<double>();

        settings settings1;
        parser.parse(args1, settings1);

        handler.set_convert_function(nullptr);
        CPPUNIT_ASSERT_THROW(parser.parse(args1, settings1), configuration_exception);
    }

    void test_convert_function()
    {
        argument_table args1("appname",
            {
                "--double=-17.43",
            });

        struct settings
        {
            double m_double;
        };

        storing_parser<settings> parser;
        auto& handler
            = parser.add_valued({ "--double" })
                  .set_type<double>()
                  .set_convert_function([](const std::string& value_str) { return std::stod(value_str) + 10.0; })
                  .set_store_function([](settings& settings, double value) { settings.m_double = value; });

        settings settings1;
        parser.parse(args1, settings1);
        CPPUNIT_ASSERT_EQUAL(double(-7.43), settings1.m_double);

        handler.set_convert_function(
            [](const std::string & /*value_str*/) -> double { throw std::invalid_argument("value_str"); });

        CPPUNIT_ASSERT_THROW(parser.parse(args1, settings1), std::invalid_argument);
    }

    void test_check_function()
    {
        argument_table args1("appname",
            {
                "--double=-17.43",
            });
        argument_table args2("appname",
            {
                "--double=600",
            });

        struct settings
        {
            double m_double;
        };

        storing_parser<settings> parser;
        parser.add_valued({ "--double" })
            .set_type_and_storage<double>(&settings::m_double)
            .set_check_function([](double value) {
                if ((value < -100) || (value > 100))
                {
                    throw std::invalid_argument("bad value");
                }
            });

        settings settings1;

        parser.parse(args1, settings1);
        CPPUNIT_ASSERT_EQUAL(double(-17.43), settings1.m_double);

        CPPUNIT_ASSERT_THROW(parser.parse(args2, settings1), std::invalid_argument);
    }

    void test_store_function()
    {
        argument_table args1("appname",
            {
                "--double=-17.43",
            });

        struct settings
        {
        };
        double my_double;

        storing_parser<settings> parser;
        parser.add_valued({ "--double" }).set_type<double>().set_store_function([&my_double](double value) {
            my_double = value;
        });

        settings settings1;
        parser.parse(args1, settings1);
        CPPUNIT_ASSERT_EQUAL(double(-17.43), my_double);
    }

    CPPUNIT_TEST_SUITE(storage_args_test);
    CPPUNIT_TEST(test_single_value);
    CPPUNIT_TEST(test_multi_value);
    CPPUNIT_TEST(test_multi_stored_in_single);
    CPPUNIT_TEST(test_types);
    CPPUNIT_TEST(test_null_convert_function);
    CPPUNIT_TEST(test_convert_function);
    CPPUNIT_TEST(test_check_function);
    CPPUNIT_TEST(test_store_function);
    CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(storage_args_test);

} // namespace args
} // namespace oct
