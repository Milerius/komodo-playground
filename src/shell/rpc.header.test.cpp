//
// Created by sztergbaum roman on 2019-06-18.
//

#include <doctest/doctest.h>
#include "rpc.header.hpp"

namespace komodo::tests
{
    TEST_CASE ("rpc header to json")
    {
        rpc_header hdr{"1.0", "komodo_playground", "getinfo", nlohmann::json::array()};
        nlohmann::json json_data = hdr;
        auto expected_json = R"({"jsonrpc": "1.0", "id":"komodo_playground", "method": "getinfo", "params": []})"_json;
        CHECK_EQ(json_data, expected_json);
    }
}
