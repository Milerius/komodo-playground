//
// Created by sztergbaum roman on 2019-06-17.
//

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include "restclient-cpp/restclient.h"
#include "config/config.hpp"
#include "command.hpp"
#include "help.hpp"
#include "rpc.header.hpp"

namespace komodo
{
    class executor
    {
    private:
        static bool dump_answer(const RestClient::Response &response) noexcept
        {
            nlohmann::json json_data = nlohmann::json::parse(response.body);
            std::cout << json_data.dump(4) << std::endl;
            return response.code == 200;
        }

    public:
        explicit executor(const rpc_config &rpc_cfg) noexcept : rpc_cfg_(rpc_cfg)
        {

        }

        bool help(const std::vector<std::string> &args) noexcept
        {
            bool successfully_executed = false;
            if (args.size() == 1) {
                std::cerr << global_help_message << "\n";
                successfully_executed = true;
            } else if (args.size() == 2) {
                if (this->cmd_registry_.find(args[1]) != this->cmd_registry_.end()) {
                    std::cout << cmd_registry_.at(args[1]).help_message << "\n";
                    successfully_executed = true;
                }
            } else {
                std::cerr << "command help take only 0 or 1 arguments\n";
            }
            return successfully_executed;
        }

        bool getinfo() noexcept
        {
            using namespace nlohmann;
            json json_rpc_header = rpc_header{"1.0", "komodo_playground", "getinfo", json::array()};
            RestClient::Response r = RestClient::post(entry_point_,
                                                      "application/json",
                                                      json_rpc_header.dump());
            return dump_answer(r);
        }

        bool getrawtransaction(const std::vector<std::string> &args, bool show_result = true) noexcept
        {
            if (args.size() == 1 || args.size() > 3) {
                std::cerr << "command getrawtransaction take only 1 or 2 arguments\n";
                return false;
            }
            using namespace nlohmann;
            json json_rpc_header = rpc_header{"1.0", "komodo_playground",
                                              "getrawtransaction", json::array({args[1]})};
            if (args.size() == 3) {
                try {
                    json_rpc_header["params"].push_back(std::stoi(args[2]));
                }
                catch (const std::invalid_argument &error) {
                    std::cerr << "second argument should be a number\n";
                    return false;
                }
            }
            RestClient::Response r = RestClient::post(entry_point_,
                                                      "application/json",
                                                      json_rpc_header.dump());

            if (show_result && r.code == 200 && args.size() == 2) {
                nlohmann::json json_data = nlohmann::json::parse(r.body);
                std::cout << json_data["result"] << "\n";
                return true;
            } else {
                return dump_answer(r);
            }
        }

        bool decoderawtransaction(const std::vector<std::string> &args) noexcept
        {
            if (args.size() != 2) {
                std::cerr << "command decoderawtransaction take only 1 argument\n";
                return false;
            }
            using namespace nlohmann;
            json json_rpc_header = rpc_header{"1.0", "komodo_playground",
                                              "decoderawtransaction", json::array({args[1]})};
            RestClient::Response r = RestClient::post(entry_point_,
                                                      "application/json",
                                                      json_rpc_header.dump());
            return dump_answer(r);
        }

        bool operator()(const std::vector<std::string> &splitted_command_line) noexcept
        {
            bool command_successfully_executed = false;
            try {
                auto command_name = splitted_command_line[0];
                auto well_formed = cmd_registry_.at(command_name).callback(splitted_command_line);
                if (not well_formed) {
                    return false;
                }
                command_successfully_executed = true;
            }
            catch (const std::out_of_range &err) {
                std::cerr << global_help_message << "\n";
            }
            return command_successfully_executed;
        }

    private:
        const rpc_config &rpc_cfg_;
        std::string entry_point_{
                "http://" + rpc_cfg_.rpc_user + ":" + rpc_cfg_.rpc_password + "@" + rpc_cfg_.ip_address + ":" +
                std::to_string(rpc_cfg_.rpc_port) + "/"};
        using command_name = std::string;
        std::unordered_map<command_name, command> cmd_registry_{
                {"help",                 command{1u,
                                                 global_help_message,
                                                 [this]([[maybe_unused]]const std::vector<std::string> &args) {
                                                     return this->help(args);
                                                 }}
                },
                {
                 "getinfo",              command{0u, get_info_help_message,
                                                 [this]([[maybe_unused]]const std::vector<std::string> &args) {
                                                     return this->getinfo();
                                                 }}
                },
                {
                 "getrawtransaction",    command{2u, get_rawtransaction_help_message,
                                                 [this](const std::vector<std::string> &args) {
                                                     return this->getrawtransaction(args);
                                                 }}
                },
                {
                 "decoderawtransaction", command{1u, decode_rawtransaction_help_message,
                                                 [this](const std::vector<std::string> &args) {
                                                     return this->decoderawtransaction(args);
                                                 }}
                },
        };
    };
}