//
// Created by sztergbaum roman on 2019-06-17.
//

#include "config.hpp"

namespace komodo
{
    void from_json(const nlohmann::json &json_data, rpc_config &rpc_cfg)
    {
        rpc_cfg.ip_address = json_data.at("ip_address").get<std::string>();
        rpc_cfg.rpc_user = json_data.at("rpc_user").get<std::string>();
        rpc_cfg.rpc_password = json_data.at("rpc_password").get<std::string>();
        rpc_cfg.rpc_port = json_data.at("rpc_port").get<unsigned short>();
    }

    rpc_config parse_cfg(const std::filesystem::path &config_path) noexcept
    {
        rpc_config rpc_cfg{};
        std::ifstream ifs(config_path);
        assert(ifs.is_open());
        nlohmann::json config_json_data;
        ifs >> config_json_data;
        rpc_cfg = config_json_data;
        return rpc_cfg;
    }
}