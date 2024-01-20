
// builtin
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

// local
#include "slotmap/slotmap.hpp"
#include "utils/logging/assert.hpp"

// extern
#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>
#include <catch2/generators/catch_generators_random.hpp>



TEST_CASE("slotmap", "[core]")
{
    // empty slotmap
    SlotMap<int> smap;
    using Key = decltype(smap)::Key;


    REQUIRE(smap.size() == 0);


    SECTION("")
    {
        REQUIRE(smap.capacity() == 0);
        REQUIRE(smap.contains(Key::__build_key(0, 0)) == false);

        auto rand_idx = (size_t)GENERATE(0, 1, UINT32_MAX);
        auto rand_version = (uint32_t)GENERATE(0, 1, UINT32_MAX);

        REQUIRE(smap.contains(Key::__build_key(rand_idx, rand_version)) == false);
    }

    SECTION("")
    {
        auto items = std::array{1, 2, 3};
        std::array<std::optional<Key>, 3> keys;

        for (size_t i = 0; i < items.size(); ++i)
            keys[i] = smap.push((int)items[i]);

        REQUIRE(smap.size() == 3);
        REQUIRE(smap.capacity() >= 3);

        for (size_t i = 0; i < items.size(); ++i)
            REQUIRE(items[i] == smap.get(keys[i].value()));

        for (size_t i = 0; i < items.size(); ++i)
            smap.remove(keys[i].value());

        REQUIRE(smap.size() == 0);
        REQUIRE(smap.capacity() >= 3);
    }

    SECTION("")
    {
        auto key1 = smap.push(1);
        smap.remove(key1);

        auto key2 = smap.push(2);

        REQUIRE(key1.get_idx() == key2.get_idx());
        REQUIRE(key2.get_version() > key1.get_version());


        REQUIRE(smap.size() == 1);
        REQUIRE(smap.capacity() == 1);
    }
    SECTION("")
    {
        auto key = smap.push(1);
        smap.remove(key);
    }
};

//TEST_CASE("slotmap iterator")
//{
//    SlotMap<int> smap;
//
//    REQUIRE(smap.begin() == smap.end());
//    REQUIRE_THROWS(*smap.begin());
//    REQUIRE_THROWS(++smap.begin());
//    REQUIRE_THROWS(smap.begin()++);
//
//    SECTION("")
//    {
//        auto values = std::array{1, 2, 3};
//
//        for (auto& value: values)
//            (void)smap.push((int)value);
//
//        REQUIRE(smap.size() == 3);
//        REQUIRE(std::mismatch(values.begin(), values.end(), smap.begin(), smap.end()) ==
//                std::pair{values.end(), smap.end()});
//
//        REQUIRE(*(++smap.begin()) == 2);
//        REQUIRE(*(smap.begin()++) == 1);
//
//        auto it = smap.begin();
//        REQUIRE(*(it++) == 1);
//        REQUIRE(*(it++) == 2);
//        REQUIRE(*(it++) == 3);
//        REQUIRE_THROWS(*(it++));
//    }
//}
