#ifndef OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_
#define OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_

#include <functional>

#include "converter.hpp"
#include "internal/argument_handler.hpp"

namespace oct
{
namespace args
{

template <typename DATA, typename TRAITS, typename VALUES_STORAGE>
class basic_argument_type_handler : public internal::basic_argument_handler<TRAITS, VALUES_STORAGE>
{
public:
    using data_type = DATA;
    using traits_type = TRAITS;
    using string_type = typename TRAITS::string_type;
    using values_storage_type = VALUES_STORAGE;

    using convert_function_type = std::function<data_type(const string_type&)>;
    using check_function_type = std::function<void(const data_type& data)>;
    using direct_store_function_type = std::function<void(const data_type& data)>;
    using store_function_type = std::function<void(values_storage_type& storage, const data_type& data)>;

    basic_argument_type_handler()
        : m_convert_function(basic_converter<traits_type, data_type>())
        , m_store_function()
    {
        // noop
    }

    basic_argument_type_handler& set_convert_function(convert_function_type convert_func)
    {
        m_convert_function = convert_func;
        return *this;
    }

    basic_argument_type_handler& set_check_function(check_function_type check_func)
    {
        m_check_function = check_func;
        return *this;
    }

    basic_argument_type_handler& set_store_function(store_function_type store_func)
    {
        m_store_function = store_func;
        return *this;
    }

    basic_argument_type_handler& set_store_function(direct_store_function_type store_func)
    {
        m_store_function = [store_func](values_storage_type& /*storage*/, const data_type& data) { store_func(data); };
        return *this;
    }

    basic_argument_type_handler& set_storage(data_type values_storage_type::*member_ptr)
    {
        return set_store_function(
            [member_ptr](values_storage_type& storage, const data_type& value) { storage.*member_ptr = value; });
    }

    basic_argument_type_handler& set_storage(std::vector<data_type> values_storage_type::*member_ptr)
    {
        return set_store_function([member_ptr](values_storage_type& storage, const data_type& value) {
            (storage.*member_ptr).push_back(value);
        });
    }

    void parse(values_storage_type& storage, const string_type& value_str) const final
    {
        if (!m_convert_function)
        {
            throw configuration_exception("Convert function not set");
        }

        data_type value = m_convert_function(value_str);

        if (m_check_function)
        {
            m_check_function(value);
        }

        if (m_store_function)
        {
            m_store_function(storage, value);
        }
    }

private:
    convert_function_type m_convert_function;
    check_function_type m_check_function;
    store_function_type m_store_function;
};

} // namespace args
} // namespace oct

#endif // OCTARGS_ARGUMENT_TYPE_HANDLER_HPP_
