//
// Created by sztergbaum roman on 2019-06-16.
//

#pragma once

#include <fstream>
#include <filesystem>
#include <string>
#include <nlohmann/json.hpp>

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

    void from_json(const nlohmann::json &json_data, rpc_config &rpc_cfg);

    rpc_config parse_cfg(const std::filesystem::path &config_path = std::filesystem::current_path() / "config.json") noexcept;
}
