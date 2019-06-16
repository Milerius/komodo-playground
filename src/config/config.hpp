//
// Created by sztergbaum roman on 2019-06-16.
//

#pragma once

#include <nlohmann/json.hpp>
#include <filesystem>
#include <string>

namespace komodo
{
    struct rpc_config
    {
        bool operator==(const rpc_config &rhs) const
        {
            return ip_address == rhs.ip_address &&
                   rpc_user == rhs.rpc_user &&
                   rpc_password == rhs.rpc_password &&
                   rpc_port == rhs.rpc_port;
        }

        bool operator!=(const rpc_config &rhs) const
        {
            return !(rhs == *this);
        }

        std::string ip_address;
        std::string rpc_user;
        std::string rpc_password;
        unsigned short rpc_port;
    };

    inline void from_json(const nlohmann::json &json_data, rpc_config &rpc_cfg)
    {
        rpc_cfg.ip_address = json_data.at("ip_address").get<std::string>();
        rpc_cfg.rpc_user = json_data.at("rpc_user").get<std::string>();
        rpc_cfg.rpc_password = json_data.at("rpc_password").get<std::string>();
        rpc_cfg.rpc_port = json_data.at("rpc_port").get<unsigned short>();
    }
}
