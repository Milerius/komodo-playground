//
// Created by sztergbaum roman on 2019-06-17.
//

#pragma once

#include <string>
#include <filesystem>
#include <replxx.hxx>
#include "executor.hpp"
#include "config/config.hpp"
#include "trim.hpp"
#include "split.hpp"

namespace komodo
{
    class shell
    {
    public:
        shell() noexcept
        {
            rx_.install_window_change_handler();
            rx_.history_load(history_file_);
            rx_.set_max_history_size(128);
            rx_.set_max_hint_rows(3);
            rx_.set_word_break_characters(" \t.,-%!;:=*~^'\"/?<>|[](){}");
            rx_.set_completion_count_cutoff(128);
            rx_.set_double_tab_completion(false);
            rx_.set_complete_on_empty(true);
            rx_.set_beep_on_ambiguous_completion(false);
            rx_.set_no_color(false);
        }

        void run()
        {
            for (;;) {
                char const *cinput{nullptr};

                do {
                    cinput = rx_.input(prompt_);
                } while ((cinput == nullptr) && (errno == EAGAIN));

                if (cinput == nullptr) {
                    break;
                }

                std::string input{cinput};
                if (input.empty()) {
                    continue;
                }
                trim(input);
                if (input == "exit")
                    break;
                auto args = split(input);
                if (cmd_executor_(args)) {
                    rx_.history_add(input);
                }
            }
            rx_.history_save(history_file_.string());
        }

    private:
        static constexpr const char *prompt_ = "> ";
        std::filesystem::path history_file_{std::filesystem::temp_directory_path() / "komodo_playground_history.txt"};
        rpc_config rpc_cfg_{parse_cfg()};
        replxx::Replxx rx_;
        executor cmd_executor_{rpc_cfg_};
    };
}