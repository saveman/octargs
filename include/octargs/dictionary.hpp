#ifndef OCTARGS_DICTIONARY_HPP_
#define OCTARGS_DICTIONARY_HPP_

#include <map>
#include <string>
#include <vector>

namespace oct
{
namespace args
{

/// \brief Exception thrown when unsupported key is used with dictionary.
class invalid_dictionary_key : public std::logic_error
{
public:
    explicit invalid_dictionary_key(const std::string& message)
        : std::logic_error(message)
    {
        // noop
    }
};

/// \brief Parser literals dictionary
///
/// \tparam char_T              char type (as in std::basic_string)
template <typename char_T>
class dictionary
{
public:
    enum class usage_literal
    {
        LEAD,
        NO_ARGUMENTS_INFO,
        HEADER_OPTIONS_NAME,
        HEADER_SUBOPTIONS_NAME,
        HEADER_MULTIVALUE_MARKER,
        DEFAULT_NAMED_ARGUMENTS_GROUP_NAME,
        DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME,
        DECORATOR_REQUIRED,
        DECORATOR_MIN_COUNT,
        DECORATOR_MAX_COUNT,
        DECORATOR_MAX_COUNT_UNLIMITED,
        DECORATOR_VALUE_SEPARATOR,
        DECORATOR_DEFAULT,
        DECORATOR_ALLOWED,
    };

    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    virtual ~dictionary() = default;

    virtual const string_type& get_switch_enabled_literal() const = 0;
    virtual const string_vector_type& get_true_literals() const = 0;
    virtual const string_vector_type& get_false_literals() const = 0;

    virtual const string_type& get_value_separator_literal() const = 0;
    virtual const string_type& get_subparser_separator_literal() const = 0;

    virtual bool is_short_name(const string_type& name) const = 0;
    virtual const string_type& get_usage_literal(usage_literal key) const = 0;

protected:
    static bool is_short_name(const string_type& name, char_type prefix_char)
    {
        auto len = name.size();
        auto first_not_dash = name.find_first_not_of(prefix_char);
        if (first_not_dash != string_type::npos)
        {
            len -= first_not_dash;
        }

        return (len <= 1);
    }
};

/// \brief Parser literals dictionary
///
/// \tparam char_T              char type (as in std::basic_string)
template <typename char_T>
class default_dictionary
{
public:
    default_dictionary() = delete;
};

/// \brief Parser literals dictionary (specialization for char type)
///
template <>
class default_dictionary<char> : public dictionary<char>
{
public:
    static constexpr char_type DEFAULT_NAME_PREFIX = '-';

    using base_type = dictionary<char>;

    // cppcheck-suppress functionStatic
    const string_type& get_switch_enabled_literal() const override
    {
        static const string_type TRUE_LITERAL { "true" };
        return TRUE_LITERAL;
    }

    // cppcheck-suppress functionStatic
    const string_vector_type& get_true_literals() const override
    {
        static const string_vector_type TRUE_LITERALS {
            "true",
            "1",
            "yes",
        };
        return TRUE_LITERALS;
    }

    // cppcheck-suppress functionStatic
    const string_vector_type& get_false_literals() const override
    {
        static const string_vector_type FALSE_LITERALS {
            "false",
            "0",
            "no",
        };
        return FALSE_LITERALS;
    }

    // cppcheck-suppress functionStatic
    const string_type& get_value_separator_literal() const override
    {
        static const string_type SEPARATOR { "=" };
        return SEPARATOR;
    }

    // cppcheck-suppress functionStatic
    const string_type& get_subparser_separator_literal() const override
    {
        static const string_type SEPARATOR(" ");
        return SEPARATOR;
    }

    // cppcheck-suppress functionStatic
    bool is_short_name(const string_type& name) const override
    {
        return base_type::is_short_name(name, DEFAULT_NAME_PREFIX);
    }

    // cppcheck-suppress functionStatic
    const string_type& get_usage_literal(usage_literal key) const override
    {
        auto& literals_map = get_usage_literals_map();

        auto iter = literals_map.find(key);
        if (iter != literals_map.end())
        {
            return iter->second;
        }
        throw invalid_dictionary_key(std::string("Invalid key: ") + std::to_string(static_cast<int>(key)));
    }

    static const std::map<usage_literal, string_type>& get_usage_literals_map()
    {
        static const std::map<usage_literal, string_type> USAGE_LITERALS = {
            { usage_literal::LEAD, "Usage" },
            { usage_literal::NO_ARGUMENTS_INFO, "(no arguments available)" },
            { usage_literal::HEADER_OPTIONS_NAME, "OPTIONS" },
            { usage_literal::HEADER_SUBOPTIONS_NAME, "SUBOPTS" },
            { usage_literal::HEADER_MULTIVALUE_MARKER, "..." },
            { usage_literal::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, "Optional arguments" },
            { usage_literal::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, "Positional arguments" },
            { usage_literal::DECORATOR_REQUIRED, "required" },
            { usage_literal::DECORATOR_MIN_COUNT, "min" },
            { usage_literal::DECORATOR_MAX_COUNT, "max" },
            { usage_literal::DECORATOR_MAX_COUNT_UNLIMITED, "unlimited" },
            { usage_literal::DECORATOR_VALUE_SEPARATOR, ": " },
            { usage_literal::DECORATOR_DEFAULT, "default" },
            { usage_literal::DECORATOR_ALLOWED, "allowed" },
        };
        return USAGE_LITERALS;
    }
};

/// \brief Parser literals dictionary (specialization for wchar_t type)
///
template <>
class default_dictionary<wchar_t> : public dictionary<wchar_t>
{
public:
    static constexpr char_type DEFAULT_NAME_PREFIX = L'-';

    using base_type = dictionary<wchar_t>;

    // cppcheck-suppress functionStatic
    const string_type& get_switch_enabled_literal() const override
    {
        static const string_type TRUE_LITERAL { L"true" };
        return TRUE_LITERAL;
    }

    // cppcheck-suppress functionStatic
    const string_vector_type& get_true_literals() const override
    {
        static const string_vector_type TRUE_LITERALS {
            L"true",
            L"1",
            L"yes",
        };
        return TRUE_LITERALS;
    }

    // cppcheck-suppress functionStatic
    const string_vector_type& get_false_literals() const override
    {
        static const string_vector_type FALSE_LITERALS {
            L"false",
            L"0",
            L"no",
        };
        return FALSE_LITERALS;
    }

    // cppcheck-suppress functionStatic
    const string_type& get_value_separator_literal() const override
    {
        static const string_type SEPARATOR { L"=" };
        return SEPARATOR;
    }

    // cppcheck-suppress functionStatic
    const string_type& get_subparser_separator_literal() const override
    {
        static const string_type SEPARATOR { L" " };
        return SEPARATOR;
    }

    // cppcheck-suppress functionStatic
    bool is_short_name(const string_type& name) const override
    {
        return base_type::is_short_name(name, DEFAULT_NAME_PREFIX);
    }

    // cppcheck-suppress functionStatic
    const string_type& get_usage_literal(usage_literal key) const override
    {
        auto& literals_map = get_usage_literals_map();

        auto iter = literals_map.find(key);
        if (iter != literals_map.end())
        {
            return iter->second;
        }
        throw invalid_dictionary_key(std::string("Invalid key: ") + std::to_string(static_cast<int>(key)));
    }

    static const std::map<usage_literal, string_type>& get_usage_literals_map()
    {
        static const std::map<usage_literal, string_type> USAGE_LITERALS = {
            { usage_literal::LEAD, L"Usage" },
            { usage_literal::NO_ARGUMENTS_INFO, L"(no arguments available)" },
            { usage_literal::HEADER_OPTIONS_NAME, L"OPTIONS" },
            { usage_literal::HEADER_SUBOPTIONS_NAME, L"SUBOPTS" },
            { usage_literal::HEADER_MULTIVALUE_MARKER, L"..." },
            { usage_literal::DEFAULT_NAMED_ARGUMENTS_GROUP_NAME, L"Optional arguments" },
            { usage_literal::DEFAULT_POSITIONAL_ARGUMENTS_GROUP_NAME, L"Positional arguments" },
            { usage_literal::DECORATOR_REQUIRED, L"required" },
            { usage_literal::DECORATOR_MIN_COUNT, L"min" },
            { usage_literal::DECORATOR_MAX_COUNT, L"max" },
            { usage_literal::DECORATOR_MAX_COUNT_UNLIMITED, L"unlimited" },
            { usage_literal::DECORATOR_VALUE_SEPARATOR, L": " },
            { usage_literal::DECORATOR_DEFAULT, L"default" },
            { usage_literal::DECORATOR_ALLOWED, L"allowed" },
        };
        return USAGE_LITERALS;
    }
};

template <typename char_T>
class custom_dictionary : public dictionary<char_T>
{
public:
    enum class init_mode
    {
        WITH_DEFAULTS,
        NO_DEFAULTS
    };

    using base_type = dictionary<char_T>;

    using char_type = typename base_type::char_type;
    using string_type = typename base_type::string_type;
    using string_vector_type = typename base_type::string_vector_type;
    using usage_literal = typename base_type::usage_literal;

    explicit custom_dictionary(init_mode init_mode)
        : m_name_prefix(0)
    {
        if (init_mode == init_mode::WITH_DEFAULTS)
        {
            using default_dict_type = default_dictionary<char_type>;

            default_dict_type default_dict;

            m_name_prefix = default_dict_type::DEFAULT_NAME_PREFIX;
            m_switch_enabled_literal = default_dict.get_switch_enabled_literal();
            m_true_literals = default_dict.get_true_literals();
            m_false_literals = default_dict.get_false_literals();
            m_value_separator_literal = default_dict.get_value_separator_literal();
            m_subparser_separator_literal = default_dict.get_subparser_separator_literal();
            m_usage_literals = default_dict.get_usage_literals_map();
        }
    }

    void set_switch_enabled_literal(const string_type& literal)
    {
        m_switch_enabled_literal = literal;
    }

    const string_type& get_switch_enabled_literal() const override
    {
        return m_switch_enabled_literal;
    }

    void add_true_literal(const string_type& literal)
    {
        m_true_literals.emplace_back(literal);
    }

    const string_vector_type& get_true_literals() const override
    {
        return m_true_literals;
    }

    void add_false_literal(const string_type& literal)
    {
        m_false_literals.emplace_back(literal);
    }

    const string_vector_type& get_false_literals() const override
    {
        return m_false_literals;
    }

    void set_value_separator_literal(const string_type& literal)
    {
        m_value_separator_literal = literal;
    }

    const string_type& get_value_separator_literal() const override
    {
        return m_value_separator_literal;
    }

    void set_subparser_separator_literal(const string_type& literal)
    {
        m_value_separator_literal = literal;
    }

    const string_type& get_subparser_separator_literal() const override
    {
        return m_subparser_separator_literal;
    }

    virtual bool is_short_name(const string_type& name) const override
    {
        if (m_name_prefix == 0)
        {
            return false;
        }
        return base_type::is_short_name(name, m_name_prefix);
    }

    void set_usage_literal(usage_literal key, const string_type& value)
    {
        m_usage_literals[key] = value;
    }

    const string_type& get_usage_literal(usage_literal key) const override
    {
        auto iter = m_usage_literals.find(key);
        if (iter != m_usage_literals.end())
        {
            return iter->second;
        }
        throw invalid_dictionary_key(std::string("Invalid key: ") + std::to_string(static_cast<int>(key)));
    }

private:
    using usage_string_map = std::map<usage_literal, string_type>;

    char_type m_name_prefix;
    string_type m_switch_enabled_literal;
    string_vector_type m_true_literals;
    string_vector_type m_false_literals;
    string_type m_value_separator_literal;
    string_type m_subparser_separator_literal;
    usage_string_map m_usage_literals;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_DICTIONARY_HPP_
