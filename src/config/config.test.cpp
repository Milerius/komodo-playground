//
// Created by sztergbaum roman on 2019-06-16.
//

#include <doctest/doctest.h>
#include "config.hpp"

namespace komodo::tests
{
    TEST_CASE ("rpc config from json")
    {
        auto json_rpc_cfg = R"({"ip_address": "127.0.0.1","rpc_port": 7771,"rpc_user": "myrpcusername","rpc_password": "myrpcpassword"})"_json;
        rpc_config rpc_cfg{};
        CHECK_NOTHROW(from_json(json_rpc_cfg, rpc_cfg));
        CHECK_EQ(rpc_cfg, rpc_config{"127.0.0.1", "myrpcusername", "myrpcpassword", 7771u});
    }

    SCENARIO ("loading configuration")
    {
        GIVEN("without specifying any path") {
            THEN("We load the configuration") {
                REQUIRE_NE(rpc_config{}, parse_cfg());
            }
        }

        GIVEN ("with specifying the path") {
            THEN("We load the configuration") {
                REQUIRE_NE(rpc_config{}, parse_cfg(std::filesystem::current_path() / "config.json"));
            }
        }
    }
}