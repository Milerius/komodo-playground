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

    TEST_CASE ("getrawtransaction")
    {
        executor exc(parse_cfg());
                CHECK(exc.getrawtransaction(
                {"getrawtransaction", "8cedbf6529611704df32cea570974a1ca83027ee75f109f424fef4aa0e8e391d"}));
                CHECK(exc.getrawtransaction(
                {"getrawtransaction", "8cedbf6529611704df32cea570974a1ca83027ee75f109f424fef4aa0e8e391d", "1"}));
                CHECK_FALSE(exc.getrawtransaction(
                {"getrawtransaction", "8cedbf6529611704df32cea570974a1ca83027ee75f109f424fef4aa0e8e391d", "fake"}));
    }

    TEST_CASE ("decoderawtransaction")
    {
        executor exc(parse_cfg());
        CHECK_FALSE(exc.decoderawtransaction({"decoderawtransaction", "fakehexid"}));
        CHECK(exc.decoderawtransaction({"decoderawtransaction",
                                        "01000000010000000000000000000000000000000000000000000000000000000000000000ffffffff060320c90c0101ffffffff0158bfe2110000000023210370bcf10575d8fb0291afad7bf3a76929734f888228bc49e35c5c49b336002153acedc1fc5a"}));
    }
}