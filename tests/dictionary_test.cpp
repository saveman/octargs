#include "gtest/gtest.h"

#include <array>

#include "../include/octargs/octargs.hpp"

namespace oct
{
namespace args
{

TEST(dictionary_test, test_extra_literals)
{
    using dictionary_type = custom_dictionary<char>;

    auto dict = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);

    dict->add_true_literal("oui");
    dict->add_true_literal("tak");
    dict->add_false_literal("non");
    dict->add_false_literal("nie");

    parser parser(dict);
    parser.add_valued({ "--bool" }).set_type<bool>();

    results results = parser.parse(argument_table("app", {}));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--bool=yes" })));
    ASSERT_EQ(true, results.get_first_value_as<bool>("--bool"));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--bool=no" })));
    ASSERT_EQ(false, results.get_first_value_as<bool>("--bool"));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--bool=oui" })));
    ASSERT_EQ(true, results.get_first_value_as<bool>("--bool"));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--bool=tak" })));
    ASSERT_EQ(true, results.get_first_value_as<bool>("--bool"));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--bool=non" })));
    ASSERT_EQ(false, results.get_first_value_as<bool>("--bool"));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--bool=nie" })));
    ASSERT_EQ(false, results.get_first_value_as<bool>("--bool"));
}

TEST(dictionary_test, test_switch_enabled_literal)
{
    using dictionary_type = custom_dictionary<char>;

    auto dict = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);

    dict->set_switch_enabled_literal("tak");
    dict->add_true_literal("tak");

    parser parser(dict);
    parser.add_switch({ "--bool" });

    results results = parser.parse(argument_table("app", {}));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--bool" })));
    ASSERT_EQ(std::string("tak"), results.get_first_value("--bool"));
    ASSERT_EQ(true, results.get_first_value_as<bool>("--bool"));
}

TEST(dictionary_test, test_no_defaults)
{
    using dictionary_type = custom_dictionary<char>;

    auto dict = std::make_shared<dictionary_type>(dictionary_type::init_mode::NO_DEFAULTS);

    dict->set_usage_literal(dictionary_type::usage_literal::DECORATOR_DEFAULT, "domyślnie");

    ASSERT_EQ("domyślnie", dict->get_usage_literal(dictionary_type::usage_literal::DECORATOR_DEFAULT));
    ASSERT_THROW(dict->get_usage_literal(dictionary_type::usage_literal::DECORATOR_REQUIRED), invalid_dictionary_key);
}

TEST(dictionary_test, test_subparser_dictionary)
{
    using dictionary_type = custom_dictionary<char>;

    auto dict = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);

    dict->set_switch_enabled_literal("tak");
    dict->add_true_literal("tak");

    parser parser(dict);
    parser.add_switch({ "--bool" });

    auto subparsers = parser.add_subparsers("command");
    auto cmd1_parser = subparsers.add_parser("cmd1");
    cmd1_parser.add_switch({ "--switch" });

    auto results = parser.parse(argument_table("app", { "--bool", "cmd1", "--switch" }));

    ASSERT_EQ(std::string("tak"), results.get_first_value("--bool"));
    ASSERT_EQ(true, results.get_first_value_as<bool>("--bool"));
    ASSERT_EQ(std::string("cmd1"), results.get_first_value("command"));
    ASSERT_EQ(std::string("tak"), results.get_first_value("cmd1 --switch"));
    ASSERT_EQ(true, results.get_first_value_as<bool>("cmd1 --switch"));
}

TEST(dictionary_test, test_ignore_case_bool_converter)
{
    using dictionary_type = custom_dictionary<char>;

    auto dict = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);

    dict->set_case_sensitive(false);

    parser parser(dict);
    parser.add_valued({ "--bool" });

    auto results = parser.parse(argument_table("app", { "--bool", "true" }));
    ASSERT_EQ(true, results.has_value("--bool"));
    ASSERT_EQ(true, results.get_first_value_as<bool>("--bool"));

    results = parser.parse(argument_table("app", { "--bool", "TruE" }));
    ASSERT_EQ(true, results.has_value("--bool"));
    ASSERT_EQ(true, results.get_first_value_as<bool>("--bool"));

    results = parser.parse(argument_table("app", { "--bool", "FALSE" }));
    ASSERT_EQ(true, results.has_value("--bool"));
    ASSERT_EQ(false, results.get_first_value_as<bool>("--bool"));

    results = parser.parse(argument_table("app", { "--bool", "FalsE" }));
    ASSERT_EQ(true, results.has_value("--bool"));
    ASSERT_EQ(false, results.get_first_value_as<bool>("--bool"));
}

TEST(dictionary_test, test_ignore_case_argument_name)
{
    using dictionary_type = custom_dictionary<char>;

    auto dict = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);

    dict->set_case_sensitive(false);

    parser parser(dict);
    parser.add_valued({ "--bool" });
    ASSERT_THROW(parser.add_valued({ "--bool" }), invalid_argument_name);
    ASSERT_THROW(parser.add_valued({ "--BOOl" }), invalid_argument_name);
    ASSERT_THROW(parser.add_valued({ "--other", "--O", "--othER" }), invalid_argument_name);

    auto results = parser.parse(argument_table("app", { "--bool", "true" }));
    ASSERT_EQ(true, results.has_value("--BooL"));
    ASSERT_EQ("true", results.get_first_value("--BOol"));
}

TEST(dictionary_test, test_ignore_case_subparser_name)
{
    using dictionary_type = custom_dictionary<char>;

    auto dict = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);

    dict->set_case_sensitive(false);

    parser parser(dict);
    auto subparsers = parser.add_subparsers("command");

    subparsers.add_parser("cmd1");
    subparsers.add_parser("cmd2");
    ASSERT_THROW(subparsers.add_parser("cmd1"), invalid_parser_name);
    ASSERT_THROW(subparsers.add_parser("Cmd1"), invalid_parser_name);
    ASSERT_THROW(subparsers.add_parser("CMD2"), invalid_parser_name);

    auto results = parser.parse(argument_table("app", { "CMd2" }));
    ASSERT_EQ(true, results.has_value("command"));
    ASSERT_EQ("CMd2", results.get_first_value("COMMAND"));
}

TEST(dictionary_test, test_ignore_case_allowed_values)
{
    using dictionary_type = custom_dictionary<char>;

    auto dict = std::make_shared<dictionary_type>(dictionary_type::init_mode::WITH_DEFAULTS);

    dict->set_case_sensitive(false);

    parser parser(dict);
    parser.add_valued({ "--val" }).set_allowed_values({ "true", "FALSE", "file-not-FOUND" });

    auto results = parser.parse(argument_table("app", {}));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--val=TRUE" })));
    ASSERT_EQ(true, results.has_value("--val"));
    ASSERT_EQ("TRUE", results.get_first_value("--val"));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--val=false" })));
    ASSERT_EQ(true, results.has_value("--val"));
    ASSERT_EQ("false", results.get_first_value("--val"));

    ASSERT_NO_THROW(results = parser.parse(argument_table("app", { "--val=file-NOT-found" })));
    ASSERT_EQ(true, results.has_value("--val"));
    ASSERT_EQ("file-NOT-found", results.get_first_value("--val"));
}

} // namespace args
} // namespace oct
