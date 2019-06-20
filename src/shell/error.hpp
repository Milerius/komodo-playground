//
// Created by sztergbaum roman on 2019-06-20.
//

#pragma once

namespace komodo
{
    static constexpr const char *decode_raw_transaction_error_syntax = "command decoderawtransaction take only 1 argument\n";
    static constexpr const char *decode_transaction_error_syntax = "command decodetransaction take only 1 argument\n";
    static constexpr const char *help_error_syntax = "command help take only 0 or 1 arguments\n";
    static constexpr const char *raw_transaction_error = "error when getting the raw transaction, please retry.\n";
    static constexpr const char *get_rawtransaction_error_syntax = "command getrawtransaction take only 1 or 2 arguments\n";
    static constexpr const char *get_rawtransaction_argument_invalid = "second argument should be a number\n";
}