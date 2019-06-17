//
// Created by sztergbaum roman on 2019-06-17.
//

#pragma once

#include <vector>
#include <string>
#include <sstream>

std::vector <std::string> split(const std::string &subject)
{
    std::istringstream ss{subject};
    using StrIt = std::istream_iterator<std::string>;
    std::vector <std::string> container{StrIt{ss}, StrIt{}};
    return container;
}
