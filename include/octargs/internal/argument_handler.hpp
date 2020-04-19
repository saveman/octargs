#ifndef OCTARGS_ARGUMENT_HANDLER_HPP_
#define OCTARGS_ARGUMENT_HANDLER_HPP_

#include "../parser_dictionary.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T, typename values_storage_T>
class basic_argument_handler
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_T>;

    using dictionary_type = parser_dictionary<char_type>;

    virtual ~basic_argument_handler() = default;

    virtual void parse(
        values_storage_type& storage, const dictionary_type& dictionary, const string_type& value_str) const = 0;
};

template <typename char_T>
class basic_argument_handler<char_T, void>
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_T>;

    using dictionary_type = parser_dictionary<char_type>;

    virtual ~basic_argument_handler() = default;

    virtual void parse(const dictionary_type& dictionary, const string_type& value_str) const = 0;
};

template <typename char_T, typename values_storage_T>
class storage_handler_helper
{
public:
    using char_type = char_T;
    using values_storage_type = values_storage_T;

    using string_type = std::basic_string<char_T>;
    using argument_handler_type = basic_argument_handler<char_T, values_storage_T>;
    using dictionary_type = parser_dictionary<char_type>;

    storage_handler_helper(values_storage_type& storage)
        : m_storage(storage)
    {
        // noop
    }

    void parse_with_handler(
        const argument_handler_type& handler, const dictionary_type& dictionary, const string_type& value_str)
    {
        handler.parse(m_storage, dictionary, value_str);
    }

private:
    values_storage_type& m_storage;
};

template <typename char_T>
class storage_handler_helper<char_T, void>
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_T>;
    using argument_handler_type = basic_argument_handler<char_T, void>;
    using dictionary_type = parser_dictionary<char_type>;

    // cppcheck-suppress functionStatic
    void parse_with_handler(
        const argument_handler_type& handler, const dictionary_type& dictionary, const string_type& value_str)
    {
        handler.parse(dictionary, value_str);
    }
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_HANDLER_HPP_
