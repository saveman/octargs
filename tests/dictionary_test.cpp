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

} // namespace args
} // namespace oct
