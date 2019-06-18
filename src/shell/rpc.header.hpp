//
// Created by sztergbaum roman on 2019-06-18.
//

#pragma once

#include <string>
#include <nlohmann/json.hpp>

namespace komodo
{
    struct rpc_header
    {
        std::string rpc_version{"1.0"};
        std::string id{"komodo_playground"};
        std::string method;
        nlohmann::json params;
    };

    inline void to_json(nlohmann::json &json_data, const rpc_header &rpc_hdr)
    {
        json_data["jsonrpc"] = rpc_hdr.rpc_version;
        json_data["id"] = rpc_hdr.id;
        json_data["method"] = rpc_hdr.method;
        json_data["params"] = rpc_hdr.params;
    }
}
