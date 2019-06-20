//
// Created by sztergbaum roman on 2019-06-19.
//

#pragma once

#include <string>
#include <tuple>
#include <imgui.h>
#include <sstream>
#include <shell/executor.hpp>
#include "trim.hpp"
#include "split.hpp"

namespace komodo::gui
{
    struct widget
    {
        std::string title;
        bool is_open{true};
    };

    class console
    {
    private:

        void begin_root_widget_draw() noexcept
        {
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(1920, 1080), ImGuiCond_FirstUseEver);
            if (!ImGui::Begin(root_wdg.title.c_str(), &root_wdg.is_open, ImGuiWindowFlags_NoMove)) {
                ImGui::End();
            }
            if (ImGui::BeginPopupContextItem()) {
                if (ImGui::MenuItem("Close Console")) {
                    root_wdg.is_open = false;
                }
                ImGui::EndPopup();
            }
        }

        static void end_root_widget_draw() noexcept
        {
            ImGui::End();
        }

        static void begin_footer_widget_draw() noexcept
        {
            const float footer_height_to_reserve =
                    ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false,
                              ImGuiWindowFlags_HorizontalScrollbar);
            if (ImGui::BeginPopupContextWindow()) {
                if (ImGui::Selectable("Clear")) return;
                ImGui::EndPopup();
            }
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
        }

        static void end_footer_widget_draw() noexcept
        {
            ImGui::PopStyleVar();
            ImGui::EndChild();
            ImGui::Separator();
        }

        void clear_input()
        {
            input_ = std::vector<char>(256);
            reclaim_focus_ = true;
        }

        void input_focus()
        {
            ImGui::SetItemDefaultFocus();
            if (reclaim_focus_)
                ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
        }

        void input_widget_draw()
        {
            reclaim_focus_ = false;
            if (ImGui::InputText("input", input_.data(), input_.size(),
                                 ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
                                 ImGuiInputTextFlags_CallbackHistory,
                                 []([[maybe_unused]] ImGuiInputTextCallbackData *data) {
                                     return 0;
                                 })) {
                std::string buff = input_.data();
                log("#: " + buff);
                trim(buff);
                if (buff == "clear") {
                    items_.clear();
                    clear_input();
                    input_focus();
                    return;
                } else if (buff == "exit") {
                    root_wdg.is_open = false;
                    return;
                }
                auto command_line_splitted = split(buff);
                auto res = cmd_executor_(command_line_splitted);
                log(res.second);
                scroll_to_bottom = true;
                clear_input();
            }
            input_focus();
        }

        template<typename ...TArgs>
        void log(TArgs &&...args) noexcept
        {
            std::stringstream ss;
            (ss << ... << args);
            items_.push_back(ss.str());
        }

        static void welcome_message_draw() noexcept
        {
            ImGui::TextWrapped("Welcome to the komodo gui console, type Help.");
        }

        void console_content_draw()
        {
            for (auto &&line: items_) {
                if (!filter_.PassFilter(line.c_str()))
                    continue;
                ImGui::TextUnformatted(line.c_str());
            }
            if (scroll_to_bottom)
                ImGui::SetScrollHereY(1.0f);
            scroll_to_bottom = false;
        }

        void extra_gui_buttons_draw()
        {
            if (ImGui::SmallButton("Show balance")) {
                auto res = cmd_executor_({"getbalance"});
                log(res.second);
                scroll_to_bottom = true;
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("Clear console")) {
                items_.clear();
            }
            ImGui::Separator();
        }

    public:
        explicit console(const rpc_config &rpc_cfg) : rpc_cfg_{rpc_cfg}
        {

        }

        bool draw() noexcept
        {
            begin_root_widget_draw();
            welcome_message_draw();
            extra_gui_buttons_draw();
            begin_footer_widget_draw();
            console_content_draw();
            end_footer_widget_draw();
            input_widget_draw();
            end_root_widget_draw();
            return root_wdg.is_open;
        }

    private:
        const rpc_config &rpc_cfg_;
        executor cmd_executor_{rpc_cfg_};
        ImGuiTextFilter filter_;
        bool reclaim_focus_{false};
        bool scroll_to_bottom{true};
        std::vector<std::string> items_{};
        std::vector<char> input_{std::vector<char>(256)};;
        widget root_wdg{"console", true};
    };
}
