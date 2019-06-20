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
#include "error.hpp"
#include "rpc.header.hpp"

namespace komodo
{
    class executor
    {
    public:
        using command_result = std::pair<bool, std::string>;
    private:
        static command_result dump_answer(const RestClient::Response &response) noexcept
        {
            nlohmann::json json_data = nlohmann::json::parse(response.body);
            std::cout << json_data.dump(4) << std::endl;
            return {response.code == 200, json_data.dump(4)};
        }

    public:
        explicit executor(const rpc_config &rpc_cfg) noexcept : rpc_cfg_(rpc_cfg)
        {

        }

        command_result help(const std::vector<std::string> &args) noexcept
        {
            bool successfully_executed = false;
            std::string result;
            if (args.size() == 1) {
                std::cerr << global_help_message << "\n";
                result = global_help_message;
                successfully_executed = true;
            } else if (args.size() == 2) {
                if (this->cmd_registry_.find(args[1]) != this->cmd_registry_.end()) {
                    std::cout << cmd_registry_.at(args[1]).help_message << "\n";
                    result = cmd_registry_.at(args[1]).help_message;
                    successfully_executed = true;
                }
            } else {
                std::cerr << help_error_syntax;
                result = help_error_syntax;
            }
            return {successfully_executed, result};
        }

        command_result getinfo() noexcept
        {
            using namespace nlohmann;
            json json_rpc_header = rpc_header{"1.0", "komodo_playground", "getinfo", json::array()};
            RestClient::Response r = RestClient::post(entry_point_,
                                                      "application/json",
                                                      json_rpc_header.dump());
            return dump_answer(r);
        }

        command_result getrawtransaction(const std::vector<std::string> &args, bool show_result = true,
                                         std::string *save_result = nullptr) noexcept
        {
            if (args.size() == 1 || args.size() > 3) {
                std::cerr << get_rawtransaction_error_syntax;
                return {false, get_rawtransaction_error_syntax};
            }
            using namespace nlohmann;
            json json_rpc_header = rpc_header{"1.0", "komodo_playground",
                                              "getrawtransaction", json::array({args[1]})};
            if (args.size() == 3) {
                try {
                    json_rpc_header["params"].push_back(std::stoi(args[2]));
                }
                catch (const std::invalid_argument &error) {
                    std::cerr << get_rawtransaction_argument_invalid;
                    return {false, get_rawtransaction_argument_invalid};
                }
            }
            RestClient::Response r = RestClient::post(entry_point_,
                                                      "application/json",
                                                      json_rpc_header.dump());
            std::string result;
            if (r.code == 200 && args.size() == 2) {
                nlohmann::json json_data = nlohmann::json::parse(r.body);
                if (save_result != nullptr) {
                    *save_result = json_data["result"].get<std::string>();
                }
                if (show_result) {
                    std::cout << json_data["result"] << "\n";
                    result = json_data["result"];
                }
                return {true, result};
            }
            return dump_answer(r);
        }

        command_result decoderawtransaction(const std::vector<std::string> &args) noexcept
        {
            if (args.size() != 2) {
                std::cerr << decode_raw_transaction_error_syntax;
                return {false, decode_raw_transaction_error_syntax};
            }
            using namespace nlohmann;
            json json_rpc_header = rpc_header{"1.0", "komodo_playground",
                                              "decoderawtransaction", json::array({args[1]})};
            RestClient::Response r = RestClient::post(entry_point_,
                                                      "application/json",
                                                      json_rpc_header.dump());
            return dump_answer(r);
        }

        command_result decodetransaction(const std::vector<std::string> &args) noexcept
        {
            if (args.size() != 2) {
                std::cerr << decode_transaction_error_syntax;
                return {false, decode_transaction_error_syntax};
            }
            std::string result;
            auto success = getrawtransaction({"getrawtransaction", args[1]}, false, &result);
            if (not success.first) {
                std::cerr << raw_transaction_error;
                return {false, raw_transaction_error};
            }
            return decoderawtransaction({"decoderawtransaction", result});
        }

        command_result operator()(const std::vector<std::string> &splitted_command_line) noexcept
        {
            bool command_successfully_executed = false;
            std::string result;
            try {
                auto command_name = splitted_command_line[0];
                auto well_formed = cmd_registry_.at(command_name).callback(splitted_command_line);
                if (not well_formed.first) {
                    return {false, well_formed.second};
                }
                command_successfully_executed = true;
                result = well_formed.second;
            }
            catch (const std::out_of_range &err) {
                std::cerr << global_help_message << "\n";
                result = global_help_message;
            }
            return {command_successfully_executed, result};
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
                {
                 "decodetransaction",    command{1u, "",
                                                 [this](const std::vector<std::string> &args) {
                                                     return this->decodetransaction(args);
                                                 }}
                },
        };
    };
}