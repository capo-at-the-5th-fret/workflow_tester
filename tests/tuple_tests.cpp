#include <doctest/doctest.h>
#include "mylib/tuple.h"

// NOTE: These four std includes are needed for tuple_like test
#include <ranges>
#include <list>
#include <iterator>
#include <array>
#include <utility>
#include "mylib/type_traits.h"
#include "mylib/test/type_list.h"

namespace
{
#ifdef __cpp_lib_ranges
    using tuple_like_types = std::tuple
    <
        std::tuple<>,
        std::tuple<int>,
        std::array<int, 1>,
        std::tuple<int, float>,
        std::pair<int, float>,
        std::tuple<int, float, bool>,
        std::ranges::subrange<int*, int*>
    >;
    static_assert(std::tuple_size_v<tuple_like_types> == 7);
#else
    using tuple_like_types = std::tuple
    <
        std::tuple<>,
        std::tuple<int>,
        std::array<int, 1>,
        std::tuple<int, float>,
        std::pair<int, float>,
        std::tuple<int, float, bool>
    >;
    static_assert(std::tuple_size_v<tuple_like_types> == 6);
#endif

    inline constexpr std::array tuple_like_type_sizes
    {
        std::tuple_size_v<std::tuple_element_t<0, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<1, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<2, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<3, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<4, tuple_like_types>>,
#ifdef __cpp_lib_ranges
        std::tuple_size_v<std::tuple_element_t<5, tuple_like_types>>,
        std::tuple_size_v<std::tuple_element_t<6, tuple_like_types>>
#else
        std::tuple_size_v<std::tuple_element_t<5, tuple_like_types>>
#endif
    };
    static_assert(tuple_like_type_sizes.size() == std::tuple_size_v<tuple_like_types>);
}

TEST_CASE_TEMPLATE_DEFINE("has_tuple_element", TupleLike,
    has_tuple_element_test_id)
{
    std::size_t i{ 0 };
    mylib::tuple_for_each_index<TupleLike>([&i]<std::size_t I>
    {
        CAPTURE(I);
        CAPTURE(i);
        REQUIRE(I == i);
        static_assert(mylib::detail::has_tuple_element<TupleLike, I>);
        ++i;
    });

    // Non tuple-like types
    static_assert(!mylib::detail::has_tuple_element<int, 0>);

    // NOTE: Won't compile
    // Out of range index
    //static_assert(!mylib::has_tuple_element<TestType,
      //  std::tuple_size_v<TestType>>);
}
TEST_CASE_TEMPLATE_APPLY(has_tuple_element_test_id, tuple_like_types);

TEST_CASE_TEMPLATE_DEFINE("tuple_like std types", TestType,
    tuple_like_std_types_test_id)
{
    // NOTE: std::tuple doesn't work with volatile qualifiers; volatile
    // specializations for tuple_element and tuple_size have been deprecated in
    // C++20
    // Reference:
    // https://stackoverflow.com/questions/26854320/volatile-and-const-volatile-stdtuple-and-stdget

    using qts_t = mylib::cv_qualified_set_t<TestType>;

    mylib::tuple_for_each_type<qts_t>([]<typename T>
    {
        static_assert(mylib::tuple_like<T>);
        static_assert(!mylib::tuple_like<T&>);
    });
}
TEST_CASE_TEMPLATE_APPLY(tuple_like_std_types_test_id, tuple_like_types);

TEST_CASE_TEMPLATE_DEFINE("tuple_like other types", TestType,
    tuple_like_other_types_test_id)
{
    using qts_t = mylib::cv_qualified_set_t<TestType>;

    mylib::tuple_for_each_type<qts_t>([]<typename T>
    {
        static_assert(!mylib::tuple_like<T>);
    });
}
TEST_CASE_TEMPLATE_APPLY(tuple_like_other_types_test_id,
    mylib::test::cv_qualifiable_types);

TEST_CASE_TEMPLATE_DEFINE("tuple_contains_type", TestType,
    tuple_contains_type_test_id)
{
    using tuple_t = TestType;
    using success_list = tuple_t;
    using failure_list = mylib::tuple_append_t<
        mylib::cv_qualified_set_t<void>, void*>;

    SUBCASE("successes")
    {
        mylib::tuple_for_each_type<success_list>([]<typename T>
        {
            static_assert(mylib::tuple_contains_type<T, tuple_t>::value);
            static_assert(mylib::tuple_contains_type_v<T, tuple_t>);
        });
    }

    SUBCASE("failures")
    {
        mylib::tuple_for_each_type<failure_list>([]<typename T>
        {
            static_assert(!mylib::tuple_contains_type<T, tuple_t>::value);
            static_assert(!mylib::tuple_contains_type_v<T, tuple_t>);
        });
    }
}
TEST_CASE_TEMPLATE_APPLY(tuple_contains_type_test_id, tuple_like_types);

namespace
{
    using tuple_cat_t_test_types = std::tuple
    <
        std::tuple<std::tuple<>, std::tuple<>, std::tuple<>>,
        std::tuple<std::tuple<>, std::tuple<int>, std::tuple<int>>,
        std::tuple<std::tuple<>, std::tuple<int,float>, std::tuple<int,float>>,
        std::tuple<std::tuple<int>, std::tuple<>, std::tuple<int>>,
        std::tuple<std::tuple<int>, std::tuple<int>, std::tuple<int,int>>,
        std::tuple<std::tuple<int>, std::tuple<int,float>, std::tuple<int,int,float>>,
        std::tuple<std::tuple<int,float>, std::tuple<>, std::tuple<int,float>>,
        std::tuple<std::tuple<int,float>, std::tuple<int>, std::tuple<int,float,int>>,
        std::tuple<std::tuple<int,float>, std::tuple<int,float>, std::tuple<int,float,int,float>>
    >;
}

TEST_CASE_TEMPLATE_DEFINE("tuple_cat_t", TestTypes, tuple_cat_t_test_id)
{
    using tuple_t_1 = std::tuple_element_t<0, TestTypes>;
    using tuple_t_2 = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;
    
    static_assert(std::is_same_v
    <
        mylib::tuple_cat_t<tuple_t_1, tuple_t_2>,
        expected_type
    >);
}
TEST_CASE_TEMPLATE_APPLY(tuple_cat_t_test_id, tuple_cat_t_test_types);

namespace
{
    using tuple_append_t_test_types = std::tuple
    <
        std::tuple<std::tuple<>, std::tuple<>, std::tuple<>>,
        std::tuple<std::tuple<>, std::tuple<int>, std::tuple<int>>,
        std::tuple<std::tuple<>, std::tuple<int,float,bool>, std::tuple<int,float,bool>>,

        std::tuple<std::tuple<int>, std::tuple<>, std::tuple<int>>,
        std::tuple<std::tuple<int>, std::tuple<int>, std::tuple<int,int>>,
        std::tuple<std::tuple<int>, std::tuple<float>, std::tuple<int,float>>,
        std::tuple<std::tuple<int>, std::tuple<float,bool,char>, std::tuple<int,float,bool,char>>,

        std::tuple<std::tuple<int,float,bool>, std::tuple<>, std::tuple<int,float,bool>>,
        std::tuple<std::tuple<int,float,bool>, std::tuple<char>, std::tuple<int,float,bool,char>>,
        std::tuple<std::tuple<int,float,bool>, std::tuple<char,double,long>, std::tuple<int,float,bool,char,double,long>>
    >;

    template <typename Tuple, typename Types>
    using tuple_append_t_applier = decltype(std::apply([](auto... args)
    {
        return mylib::tuple_append_t<Tuple, decltype(args)...>{};
    }, std::declval<Types>()));
}

TEST_CASE_TEMPLATE_DEFINE("tuple_append_t", TestTypes, tuple_append_t_test_id)
{
    using tuple_t = std::tuple_element_t<0, TestTypes>;
    using types_t = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;

    using t = tuple_append_t_applier<tuple_t, types_t>;
    static_assert(std::is_same_v<t, expected_type>);
}
TEST_CASE_TEMPLATE_APPLY(tuple_append_t_test_id, tuple_append_t_test_types);

namespace
{
    using tuple_prepend_t_test_types = std::tuple
    <
        std::tuple<std::tuple<>, std::tuple<>, std::tuple<>>,
        std::tuple<std::tuple<>, std::tuple<int>, std::tuple<int>>,
        std::tuple<std::tuple<>, std::tuple<int,float,bool>, std::tuple<int,float,bool>>,

        std::tuple<std::tuple<int>, std::tuple<>, std::tuple<int>>,
        std::tuple<std::tuple<int>, std::tuple<int>, std::tuple<int,int>>,
        std::tuple<std::tuple<int>, std::tuple<float>, std::tuple<float,int>>,
        std::tuple<std::tuple<int>, std::tuple<float,bool,char>, std::tuple<float,bool,char,int>>,

        std::tuple<std::tuple<int,float,bool>, std::tuple<>, std::tuple<int,float,bool>>,
        std::tuple<std::tuple<int,float,bool>, std::tuple<char>, std::tuple<char,int,float,bool>>,
        std::tuple<std::tuple<int,float,bool>, std::tuple<char,double,long>, std::tuple<char,double,long,int,float,bool>>
    >;

    template <typename Tuple, typename Types>
    using tuple_prepend_t_applier = decltype(std::apply([](auto... args)
    {
        return mylib::tuple_prepend_t<Tuple, decltype(args)...>{};
    }, std::declval<Types>()));
}

TEST_CASE_TEMPLATE_DEFINE("tuple_prepend_t", TestTypes, tuple_prepend_t_test_id)
{
    using tuple_t = std::tuple_element_t<0, TestTypes>;
    using types_t = std::tuple_element_t<1, TestTypes>;
    using expected_type = std::tuple_element_t<2, TestTypes>;

    using t = tuple_prepend_t_applier<tuple_t, types_t>;
    static_assert(std::is_same_v<t, expected_type>);
}
TEST_CASE_TEMPLATE_APPLY(tuple_prepend_t_test_id, tuple_prepend_t_test_types);

TEST_CASE_TEMPLATE_DEFINE("tuple_for_each_index", TupleLike,
    tuple_for_each_index_test_id)
{
    constexpr std::size_t expected_size = std::tuple_size_v<TupleLike>;

    std::size_t i{ 0 };
    mylib::tuple_for_each_index<TupleLike>([&i]<std::size_t I>
    {
        REQUIRE(I == i);
        static_assert(I < expected_size);
        ++i;
    });

    REQUIRE(i == expected_size);
    
}
TEST_CASE_TEMPLATE_APPLY(tuple_for_each_index_test_id, tuple_like_types);

TEST_CASE("tuple_for_each_type")
{
    // NOTE: The REQUIREs here need to use a literal type as
    // std::tuple_element_t requires a compile time index which isn't available
    // in for_each_type

#ifdef __cpp_lib_ranges
    static_assert(std::tuple_size_v<tuple_like_types> == 7);
#else
    static_assert(std::tuple_size_v<tuple_like_types> == 6);
#endif

    mylib::tuple_enumerate_types<tuple_like_types>(
        []<std::size_t I, typename TupleLike>()
    {
        using tuple_t = TupleLike;
        static_assert(std::tuple_size_v<tuple_t> == tuple_like_type_sizes[I]);

        std::size_t i{ 0 };
        mylib::tuple_for_each_type<tuple_t>([&i]<typename T>
        {
            CAPTURE(I);
            CAPTURE(i);

            REQUIRE(i < tuple_like_type_sizes[I]);

            if constexpr (I == 6)
            {
                REQUIRE(std::is_same_v<T,int*>);
            }
            else
            {
                if (i == 0)
                {
                    REQUIRE(std::is_same_v<T,int>);
                }
                else if (i == 1)
                {
                    REQUIRE(std::is_same_v<T,float>);
                }
                else if (i == 2)
                {
                    REQUIRE(std::is_same_v<T,bool>);
                }
            }

            ++i;
        });

        REQUIRE(i == tuple_like_type_sizes[I]);
    });
}

TEST_CASE_TEMPLATE_DEFINE("tuple_enumerate_types", TupleLike,
    tuple_enumerate_types_test_id)
{
    std::size_t i{ 0 };
    mylib::tuple_enumerate_types<TupleLike>([&i]<std::size_t I, typename T>
    {
        CAPTURE(I);
        CAPTURE(i);
        REQUIRE(i == I);
        static_assert(std::is_same_v<T, std::tuple_element_t<I,TupleLike>>);
        ++i;
    });

    REQUIRE(i == std::tuple_size_v<TupleLike>);
}
TEST_CASE_TEMPLATE_APPLY(tuple_enumerate_types_test_id, tuple_like_types);

namespace
{
    struct tuple_for_each_validator
    {
        template<typename T>
        bool validate_element(int i, T&& t)
        {
            using type = std::remove_cvref_t<T>;

            if (i == 0)
            {
                return std::is_same_v<type, char> && t == 'a';
            }
            else if (i == 1)
            {
                return std::is_same_v<type, int> && t == -12;
            }
            else if (i == 2)
            {
                return std::is_same_v<type, unsigned int> && t == 13;
            }
            return false;
        }

        template<typename T>
        bool validate_const_correctness(int i, T&& t)
        {
            if (i == 0)
            {
                return std::is_same_v<const char&, decltype(t)>;
            }
            else if (i == 1)
            {
                return std::is_same_v<const int&, decltype(t)>;
            }
            else if (i == 2)
            {
                return std::is_same_v<const unsigned int&, decltype(t)>;
            }
            return false;
        }
    };

    template <typename T>
    struct tuple_fixture;

    using tuple_test_type = std::tuple<char, int, unsigned int>;

    template <>
    struct tuple_fixture<tuple_test_type> : tuple_test_type
    {
        using tuple_type = tuple_test_type;

        tuple_test_type test_value{ 'a', -12, 13 };
        tuple_test_type modified_expected{ 'b', -11, 14 };
    };

    using pair_test_type = std::pair<char, int>;

    template <>
    struct tuple_fixture<pair_test_type>
    {
        using tuple_type = pair_test_type;

        pair_test_type test_value{ 'a', -12 };
        pair_test_type modified_expected{ 'b', -11 };
    };

    using array_test_type = std::array<char,1>;

    template <>
    struct tuple_fixture<array_test_type>
    {
        using tuple_type = array_test_type;

        array_test_type test_value{ 'a' };
        array_test_type modified_expected{ 'b' };
    };
}

TEST_CASE_TEMPLATE("tuple_for_each", TestType, tuple_test_type, pair_test_type,
    array_test_type)
{
    using fixture_t = tuple_fixture<TestType>;
    using tuple_t = typename fixture_t::tuple_type;

    fixture_t fixture;

    tuple_t t{ fixture.test_value };
    auto ct = std::as_const(fixture.test_value);

    SUBCASE("verify the element types")
    {
        int i = 0;
        mylib::tuple_for_each(t, [&]<typename T>(T element)
        {
            CAPTURE(i);
            tuple_for_each_validator v;
            REQUIRE(v.validate_element(i, element));
            ++i;
        });
    }

    SUBCASE("modify the elements")
    {
        mylib::tuple_for_each(t, [](auto& element)
        {
            ++element;
        });

        REQUIRE(t == fixture.modified_expected);
    }

    SUBCASE("verify const correctness")
    {
        int i = 0;
        mylib::tuple_for_each(std::as_const(t), [&](const auto& element)
        {
            CAPTURE(i);
            tuple_for_each_validator v;
            REQUIRE(v.validate_const_correctness(i, element));
            ++i;
        });
    }
}

TEST_CASE_TEMPLATE("tuple_enumerate", TestType, tuple_test_type, pair_test_type,
    array_test_type)
{
    using fixture_t = tuple_fixture<TestType>;
    using tuple_t = typename fixture_t::tuple_type;

    fixture_t fixture;

    tuple_t t{ fixture.test_value };

    SUBCASE("verify the indices and element types")
    {
        mylib::tuple_enumerate(t, [&t = t]<auto I>(auto element)
        {
            CAPTURE(I);
            static_assert(std::is_same_v<std::tuple_element_t<I, tuple_t>,
                decltype(element)>);
            REQUIRE(std::get<I>(t) == element);
        });
    }

    SUBCASE("modify the elements")
    {
        mylib::tuple_enumerate(t, []<auto I>(auto& element)
        {
            ++element;
        });

        REQUIRE(t == fixture.modified_expected );
    }

    SUBCASE("verify const correctness")
    {
        mylib::tuple_enumerate(std::as_const(t), []<auto I>(const auto& element)
        {
            CAPTURE(I);
            static_assert(
                std::is_const_v<std::remove_reference_t<decltype(element)>>);
        });
    }
}
