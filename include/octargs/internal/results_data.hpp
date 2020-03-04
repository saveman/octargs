#ifndef OCTARGS_RESULTS_DATA_HPP_
#define OCTARGS_RESULTS_DATA_HPP_

#include <map>

#include "../exception.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename char_T>
class basic_results_data
{
public:
    using char_type = char_T;

    using string_type = std::basic_string<char_type>;
    using string_vector_type = std::vector<string_type>;

    using argument_tag_type = basic_argument_tag;
    using argument_tag_ptr_type = std::shared_ptr<argument_tag_type>;

    basic_results_data()
        : m_app_name()
        , m_names_repository()
        , m_argument_values()
    {
        // noop
    }

    const string_type& get_app_name() const
    {
        return m_app_name;
    }

    void set_app_name(const string_type& app_name)
    {
        this->m_app_name = app_name;
    }

    bool has_value(const argument_tag_ptr_type& arg_ptr) const
    {
        return this->m_argument_values.find(arg_ptr) != this->m_argument_values.end();
    }

    std::size_t value_count(const argument_tag_ptr_type& arg_ptr) const
    {
        auto arg_values_iter = this->m_argument_values.find(arg_ptr);
        if (arg_values_iter != this->m_argument_values.end())
        {
            return arg_values_iter->second.size();
        }
        else
        {
            return 0;
        }
    }

    void append_value(const argument_tag_ptr_type& arg_ptr, const string_type& value)
    {
        auto arg_values_iter = this->m_argument_values.find(arg_ptr);
        if (arg_values_iter != this->m_argument_values.end())
        {
            arg_values_iter->second.push_back(value);
        }
        else
        {
            this->m_argument_values.insert(std::make_pair(arg_ptr, string_vector_type({ value })));
        }
    }

    argument_tag_ptr_type find_argument(const string_type& arg_name) const
    {
        auto name_arg_iter = m_names_repository.find(arg_name);
        if (name_arg_iter == m_names_repository.end())
        {
            throw unknown_argument_ex<char_type>(arg_name);
        }

        return name_arg_iter->second;
    }

    std::size_t count(const string_type& arg_name) const
    {
        auto name_arg_iter = find_argument(arg_name);
        auto values_iter = m_argument_values.find(name_arg_iter);
        if (values_iter != m_argument_values.end())
        {
            return values_iter->second.size();
        }
        else
        {
            return 0;
        }
    }

    const string_vector_type& values(const string_type& arg_name) const
    {
        auto name_arg_iter = find_argument(arg_name);
        auto values_iter = m_argument_values.find(name_arg_iter);
        if (values_iter != m_argument_values.end())
        {
            return values_iter->second;
        }
        else
        {
            return get_empty_values();
        }
    }

    void add_name(const string_type& name, argument_tag_ptr_type tag)
    {
        m_names_repository.emplace(name, tag);
    }

private:
    static const string_vector_type& get_empty_values()
    {
        static const string_vector_type EMPTY_VALUES;
        return EMPTY_VALUES;
    }

    string_type m_app_name;
    std::map<string_type, argument_tag_ptr_type> m_names_repository;
    std::map<argument_tag_ptr_type, string_vector_type> m_argument_values;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_RESULTS_DATA_HPP_
