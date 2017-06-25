
#include <type_traits>

namespace urdl {
namespace detail {

template <bool B, class T = void>
struct enable_if_c
{
    typedef T type;
};

template <class T>
struct enable_if_c<false, T>
{
};

template <class Cond, class T = void>
struct enable_if : public enable_if_c<Cond::value, T>
{
};

template <bool B, class T = void>
struct disable_if_c
{
    typedef T type;
};

template <class T>
struct disable_if_c<true, T>
{
};

template <class Cond, class T = void>
struct disable_if : public disable_if_c<Cond::value, T>
{
};

template <typename T>
struct remove_cv_ref
{
    typedef typename ::std::remove_cv<
        typename
        ::std::remove_reference<T>::type
    >::type type;
};  // boost::detail::remove_cv_ref

template <typename T, typename U>
struct is_related
    : public std::is_same<
        typename remove_cv_ref<T>::type,
        typename remove_cv_ref<U>::type
    >
{
};

template <typename ...T>
struct enable_if_unrelated
    : public enable_if_c<true>
{
};

template <typename T, typename U, typename ...U2>
struct enable_if_unrelated<T, U, U2...>
    : public disable_if<is_related<T, U>>
{
};

}// namespace detail


template < typename MemberType, int UniqueID = 0 >
class base_from_member
{
protected:
    MemberType  member;

    template <typename ...T, typename EnableIf = typename
    detail::enable_if_unrelated<base_from_member, T...>::type>
    explicit constexpr base_from_member( T&& ...x )
    noexcept(noexcept(::new ((void*) nullptr) MemberType(
        static_cast<T&&>(x)... )) )  // no std::is_nothrow_constructible...
        : member( static_cast<T&&>(x)... )     // ...nor std::forward needed
    {}
};  // boost::base_from_member

template < typename MemberType, int UniqueID >
class base_from_member<MemberType&, UniqueID>
{
protected:
    MemberType& member;

    explicit constexpr base_from_member( MemberType& x ) noexcept
        : member( x )
    {}
};  // boost::base_from_member


}// urdl
