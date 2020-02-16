#ifndef OCTARGS_PARSER_DATA_HPP_
#define OCTARGS_PARSER_DATA_HPP_

#include <list>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../argument.hpp"

namespace oct
{
namespace args
{
namespace internal
{

template <typename TRAITS>
struct basic_parser_data_iface
{
public:
    using string_type = typename TRAITS::string_type;
    using argument_tag_type = basic_argument_tag;
    using argument_tag_ptr_type = std::shared_ptr<argument_tag_type>;

    virtual ~basic_parser_data_iface() = default;
    virtual argument_tag_ptr_type find_argument(const string_type& arg_name) const = 0;
};

template <typename TRAITS, typename VALUES_STORAGE = internal::null_values_storage>
struct basic_parser_data : public basic_parser_data_iface<TRAITS>
{
    using char_type = typename TRAITS::char_type;
    using string_type = typename TRAITS::string_type;
    using string_vector_type = typename TRAITS::string_vector_type;

    using argument_type = basic_argument<TRAITS, VALUES_STORAGE>;
    using argument_ptr_type = std::shared_ptr<argument_type>;

    using argument_tag_type = basic_argument_tag;
    using argument_tag_ptr_type = std::shared_ptr<argument_tag_type>;

    basic_parser_data()
        : m_positional_arguments()
        , m_names_repository()
    {
        // noop
    }

    argument_tag_ptr_type find_argument(const string_type& arg_name) const override
    {
        auto name_arg_iter = m_names_repository.find(arg_name);
        if (name_arg_iter != m_names_repository.end())
        {
            return name_arg_iter->second;
        }
        return nullptr;
    }

    std::vector<argument_ptr_type> m_positional_arguments;
    std::map<string_type, argument_ptr_type> m_names_repository;
};

} // namespace internal
} // namespace args
} // namespace oct

#endif // OCTARGS_PARSER_DATA_HPP_
