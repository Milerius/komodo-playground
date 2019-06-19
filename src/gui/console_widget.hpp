//
// Created by sztergbaum roman on 2019-06-19.
//

#pragma once

#include <string>
#include <tuple>
#include <imgui.h>

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
            if (!ImGui::Begin(root_wdg.title.c_str(), &root_wdg.is_open)) {
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
                    ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing(); // 1 separator, 1 input text
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false,
                              ImGuiWindowFlags_HorizontalScrollbar); // Leave room for 1 separator + 1 InputText
            if (ImGui::BeginPopupContextWindow()) {
                if (ImGui::Selectable("Clear")) return;
                ImGui::EndPopup();
            }
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
        }

        static void end_footer_widget_draw() noexcept
        {
            ImGui::PopStyleVar();
            ImGui::EndChild();
            ImGui::Separator();
        }

        void input_widget_draw()
        {
            if (ImGui::InputText("input", input_.data(), input_.size(),
                                 ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion |
                                 ImGuiInputTextFlags_CallbackHistory, [](ImGuiInputTextCallbackData *data) {
                        return 0;
                    })) {

            }
            ImGui::SetItemDefaultFocus();
        }

    public:
        bool draw() noexcept
        {
            begin_root_widget_draw();
            begin_footer_widget_draw();
            end_footer_widget_draw();
            input_widget_draw();
            end_root_widget_draw();
            return root_wdg.is_open;
        }

    private:
        std::vector<char> input_{std::vector<char>(256)};
        using widget_title = std::string;
        using is_open = bool;
        widget root_wdg{"console", true};
    };
}
