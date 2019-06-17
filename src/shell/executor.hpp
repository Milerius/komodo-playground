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

        void help([[maybe_unused]]std::vector<std::string> args)
        {
            if (args.size() == 1) {
                std::cerr << global_help_message << "\n";
            } else {
                // TODO: Use help message from additional command.
            }
        }

        bool operator()(const std::vector<std::string> &splitted_command_line) noexcept
        {
            bool command_successfully_executed = false;
            try {
                auto command_name = splitted_command_line[0];
                cmd_registry_.at(command_name).callback(splitted_command_line);
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
                                     this->help(args);
                                 }}
                }
        };
    };
}