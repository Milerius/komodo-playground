//
// Created by sztergbaum roman on 2019-06-17.
//

#pragma once

#include <vector>
#include <string>

namespace komodo
{
    struct command
    {
        unsigned int max_args;
        std::string help_message;
        std::function<bool(const std::vector<std::string> &)> callback;
    };
}
