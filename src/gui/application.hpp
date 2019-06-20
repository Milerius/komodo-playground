//
// Created by sztergbaum roman on 2019-06-19.
//

#pragma once

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <imgui-SFML.h>
#include <config/config.hpp>
#include "console_widget.hpp"

namespace komodo::gui
{
    class application
    {
    private:
        void process_events() noexcept
        {
            sf::Event event{};
            while (window_.pollEvent(event)) {
                ImGui::SFML::ProcessEvent(event);

                if (event.type == sf::Event::Closed) {
                    window_.close();
                }
            }
        }

        void update() noexcept
        {
            ImGui::SFML::Update(window_, delta_clock_.restart());
        }

        void render() noexcept
        {
            window_.clear();
            ImGui::SFML::Render(window_);
            window_.display();
        }

        void draw_gui() noexcept
        {
            if (!console_.draw()) {
                window_.close();
            }
        }

    public:
        application() noexcept
        {
            window_.setFramerateLimit(60);
            ImGui::SFML::Init(window_);
            ImGuiIO *io = &ImGui::GetIO();
            io->FontGlobalScale = 2.0;
        }

        void run()
        {
            while (window_.isOpen()) {
                process_events();
                update();
                draw_gui();
                render();
            }

        }

        ~application() noexcept
        {
            ImGui::SFML::Shutdown();
        }

    private:
        rpc_config rpc_cfg_{parse_cfg()};
        sf::Clock delta_clock_;
        sf::RenderWindow window_{sf::RenderWindow(sf::VideoMode(1920, 1080), "Komodo playground", sf::Style::None)};
        komodo::gui::console console_{rpc_cfg_};
    };
}
