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
        unsigned int nb_args;
        std::string help_message;
        std::function<void(const std::vector<std::string> &)> callback;
    };
}
