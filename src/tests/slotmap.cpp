
// builtin
#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>

// local
#include "logging/assert.hpp"
#include "slotmap/slotmap.hpp"

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
        REQUIRE(smap.contains(Key{.idx = 0, .version = 0}) == false);

        auto rand_idx = (size_t)GENERATE(0, 1, UINT32_MAX);
        auto rand_version = (uint32_t)GENERATE(0, 1, UINT32_MAX);

        REQUIRE(smap.contains(Key{.idx = rand_idx, .version = rand_version}) == false);
    }

    SECTION("")
    {
        auto items = std::array{1, 2, 3};
        std::array<Key, 3> keys;

        for (size_t i = 0; i < items.size(); ++i)
            keys[i] = smap.push((int)items[i]);

        REQUIRE(smap.size() == 3);
        REQUIRE(smap.capacity() >= 3);

        for (size_t i = 0; i < items.size(); ++i)
            REQUIRE(items[i] == smap.get(keys[i]));

        for (size_t i = 0; i < items.size(); ++i)
        {
            auto removed_item = smap.remove(keys[i]);
            REQUIRE(removed_item.has_value());
            REQUIRE(items[i] == removed_item.value());
        }

        REQUIRE(smap.size() == 0);
        REQUIRE(smap.capacity() >= 3);
    }

    SECTION("")
    {
        auto key1 = smap.push(1);
        smap.remove(key1);

        REQUIRE(smap.get(key1).has_value() == false);

        auto key2 = smap.push(2);

        REQUIRE(key1.idx == key2.idx);
        REQUIRE(key2.version > key1.version);
        REQUIRE(smap.get(key1).has_value() == false);
        REQUIRE(smap.get(key2).value() == 2);

        REQUIRE(smap.size() == 1);
        REQUIRE(smap.capacity() == 1);
    }
    SECTION("")
    {
        auto key = smap.push(1);
        smap.remove(key);
        REQUIRE(smap.get(key).has_value() == false);
        REQUIRE_THROWS(smap.unchecked_get(key));
        REQUIRE_THROWS(smap.unchecked_remove(key));
    }
};

TEST_CASE("slotmap iterator")
{
    SlotMap<int> smap;

    REQUIRE(smap.begin() == smap.end());
    REQUIRE_THROWS(*smap.begin());
    REQUIRE_THROWS(++smap.begin());
    REQUIRE_THROWS(smap.begin()++);

    SECTION("")
    {
        auto values = std::array{1, 2, 3};

        for (auto& value: values)
            (void)smap.push((int)value);

        REQUIRE(smap.size() == 3);
        REQUIRE(std::mismatch(values.begin(), values.end(), smap.begin(), smap.end()) ==
                std::pair{values.end(), smap.end()});

        REQUIRE(*(++smap.begin()) == 2);
        REQUIRE(*(smap.begin()++) == 1);

        auto it = smap.begin();
        REQUIRE(*(it++) == 1);
        REQUIRE(*(it++) == 2);
        REQUIRE(*(it++) == 3);
        REQUIRE_THROWS(*(it++));
    }
}
