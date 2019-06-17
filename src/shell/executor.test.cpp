//
// Created by sztergbaum roman on 2019-06-17.
//

#include <doctest/doctest.h>
#include "executor.hpp"

namespace komodo::tests
{
    TEST_CASE ("getinfo")
    {
        executor exc(parse_cfg());
                CHECK(exc.getinfo());
    }

    TEST_CASE ("help")
    {
        executor exc(parse_cfg());
        CHECK(exc.help({"help"}));
        CHECK(exc.help({"help", "getinfo"}));
    }
}