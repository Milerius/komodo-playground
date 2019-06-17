//
// Created by sztergbaum roman on 2019-06-17.
//

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "config/config.hpp"
#include "command.hpp"
#include "help.hpp"

namespace komodo
{
    class executor
    {
    public:
        explicit executor(const rpc_config &rpc_cfg) noexcept : rpc_cfg_(rpc_cfg)
        {

        }

        bool help([[maybe_unused]]const std::vector<std::string>& args)
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
                std::cerr << "command help take only 1 or 2 arguments\n";
            }
            return successfully_executed;
        }

        bool operator()(const std::vector<std::string> &splitted_command_line) noexcept
        {
            bool command_successfully_executed = false;
            try {
                auto command_name = splitted_command_line[0];
                auto well_formed = cmd_registry_.at(command_name).callback(splitted_command_line);
                if (not well_formed) {
                    std::cerr << global_help_message << "\n";
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
        [[maybe_unused]] const rpc_config &rpc_cfg_;
        using command_name = std::string;
        std::unordered_map<command_name, command> cmd_registry_{
                {"help", command{0u,
                                 global_help_message,
                                 [this]([[maybe_unused]]const std::vector<std::string> &args) {
                                     return this->help(args);
                                 }}
                }
        };
    };
}