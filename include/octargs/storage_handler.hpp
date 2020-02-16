#ifndef OCTARGS_STORAGE_HANDLER_HPP_
#define OCTARGS_STORAGE_HANDLER_HPP_

#include <limits>
#include <memory>
#include <string>

#include "converter.hpp"
#include "exception.hpp"
#include "internal/misc.hpp"

namespace oct
{
namespace args
{

template <typename TRAITS, typename VALUES_STORAGE>
class storage_handler
{
public:
    using string_type = typename TRAITS::string_type;
    using values_storage_type = VALUES_STORAGE;

    virtual ~storage_handler() = default;
    virtual void store(values_storage_type& storage, const string_type& value_str) const = 0;
};

template <typename TRAITS, typename VALUES_STORAGE, typename DATA, typename CONVERTER = basic_converter<TRAITS, DATA>>
class member_storage_handler : public storage_handler<TRAITS, VALUES_STORAGE>
{
public:
    using string_type = typename TRAITS::string_type;
    using data_type = DATA;
    using values_storage_type = VALUES_STORAGE;
    using converter_type = CONVERTER;

    member_storage_handler(data_type values_storage_type::*member_ptr)
        : m_member_ptr(member_ptr)
    {
        // noop
    }

    void store(values_storage_type& storage, const string_type& value_str) const override
    {
        CONVERTER converter;

        storage.*m_member_ptr = converter(value_str);
    }

private:
    data_type values_storage_type::*const m_member_ptr;
};

template <typename TRAITS, typename VALUES_STORAGE, typename DATA_CONTAINER, typename DATA,
    typename CONVERTER = basic_converter<TRAITS, DATA>>
class member_container_storage_handler : public storage_handler<TRAITS, VALUES_STORAGE>
{
public:
    using string_type = typename TRAITS::string_type;
    using data_type = DATA;
    using data_container_type = DATA_CONTAINER;
    using values_storage_type = VALUES_STORAGE;
    using converter_type = CONVERTER;

    member_container_storage_handler(data_container_type values_storage_type::*member_ptr)
        : m_member_ptr(member_ptr)
    {
        // noop
    }

    void store(values_storage_type& storage, const string_type& value_str) const override
    {
        CONVERTER converter;

        (storage.*m_member_ptr).push_back(converter(value_str));
    }

private:
    data_container_type values_storage_type::*const m_member_ptr;
};

} // namespace args
} // namespace oct

#endif /*OCTARGS_STORAGE_HANDLER_HPP_*/
