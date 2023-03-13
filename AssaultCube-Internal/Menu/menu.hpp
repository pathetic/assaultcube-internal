#pragma once
#include <MinHook.h>
#include <imgui_internal.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_win32.h>
#include <ico_pack.h>
#include <inter.h>
#include <cmath>

#include "colors.hpp"
#include "var.hpp"

ImFont* ico = nullptr;
ImFont* ico_minimize = nullptr;
ImFont* tab_text;
ImFont* ico_logo;
ImFont* ico_subtab;
ImFont* esp_text;

float slider_blur = 3.f;

static float tab_alpha = 0.f;
static float tab_add;
static int active_tab = 0;
int tabs = 0;
int sub_tabs = 0;

namespace Menu
{
    bool showMenu = false;
	bool initialized = false;
    
    void MenuIO() {
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 20, NULL, io.Fonts->GetGlyphRangesCyrillic());
        ico = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 25, NULL, io.Fonts->GetGlyphRangesCyrillic());
        ico_subtab = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 35, NULL, io.Fonts->GetGlyphRangesCyrillic());
        ico_logo = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 31, NULL, io.Fonts->GetGlyphRangesCyrillic());
        tab_text = io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 19, NULL, io.Fonts->GetGlyphRangesCyrillic());
        ico_minimize = io.Fonts->AddFontFromMemoryTTF(&icon, sizeof icon, 27, NULL, io.Fonts->GetGlyphRangesCyrillic());
        esp_text = io.Fonts->AddFontFromMemoryTTF(&inter, sizeof inter, 19, NULL, io.Fonts->GetGlyphRangesCyrillic());
    }

	void StyleMenu() {
        ImGuiStyle& s = ImGui::GetStyle();

        s.Colors[ImGuiCol_WindowBg] = ImColor(60, 65, 80, 60);
        s.Colors[ImGuiCol_ChildBg] = ImColor(20, 20, 20, 255);
        s.Colors[ImGuiCol_PopupBg] = ImColor(26, 26, 26, 255);
        s.Colors[ImGuiCol_Text] = ImColor(120, 120, 120, 255);
        s.Colors[ImGuiCol_TextDisabled] = ImColor(100, 100, 100, 255);
        s.Colors[ImGuiCol_Border] = ImColor(28, 28, 28, 255);
        s.Colors[ImGuiCol_TextSelectedBg] = ImColor(25, 22, 33, 100);

        s.Colors[ImGuiCol_ScrollbarGrab] = ImColor(24, 24, 24, 255);
        s.Colors[ImGuiCol_ScrollbarGrabHovered] = ImColor(24, 24, 24, 255);
        s.Colors[ImGuiCol_ScrollbarGrabActive] = ImColor(24, 24, 24, 255);

        s.WindowBorderSize = 0;
        s.WindowPadding = ImVec2(0, 0);
        s.WindowRounding = 5.f;
        s.PopupBorderSize = 0.f;
        s.PopupRounding = 5.f;
        s.ChildRounding = 7;
        s.ChildBorderSize = 1.f;
        s.FrameBorderSize = 1.0f;
        s.ScrollbarSize = 3.0f;
        s.FrameRounding = 5.f;
        s.ItemSpacing = ImVec2(0, 20);
        s.ItemInnerSpacing = ImVec2(10, 0);
	}

	void Render() {
        if (!showMenu)
            return;

        StyleMenu();

        ImGui::SetNextWindowSize(ImVec2(705, 490));
        ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
        {

            const auto& p = ImGui::GetWindowPos();

            ImGuiStyle& s = ImGui::GetStyle();

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));

            ImGui::BeginChild("G-Tab", ImVec2(173, 490), false);
            {

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, 41 + p.y), ImVec2(173 + p.x, 447 + p.y), ImGui::GetColorU32(colors::Tab_Child), s.WindowRounding);

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, 0 + p.y), ImVec2(173 + p.x, 38 + p.y), ImGui::GetColorU32(colors::lite_color), s.WindowRounding);

                ImGui::GetWindowDrawList()->AddText(ico_logo, 31, ImVec2(16 + p.x, 2 + p.y), ImGui::GetColorU32(colors::general_color), "Z");

                ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(0 + p.x, 449 + p.y), ImVec2(173 + p.x, 490 + p.y), ImGui::GetColorU32(colors::lite_color), s.WindowRounding);

                ImGui::GetWindowDrawList()->AddText(ico_logo, 23, ImVec2(45 + p.x, 457 + p.y), ImGui::GetColorU32(colors::general_color), "Y");

                ImGui::SetCursorPosY(60);

                if (ImGui::Tab("A", "Visuals", 0 == tabs, ImVec2(150, 30))) tabs = 0;

                if (ImGui::Tab("B", "Aimbot", 1 == tabs, ImVec2(150, 30))) tabs = 1;

                if (ImGui::Tab("C", "Misc", 2 == tabs, ImVec2(150, 30))) tabs = 2;

            }ImGui::EndChild();

            ImGui::PopStyleColor();

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(182 + p.x, 0 + p.y), ImVec2(705 + p.x, 490 + p.y), ImGui::GetColorU32(colors::main_color), s.WindowRounding);

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(182 + p.x, 0 + p.y), ImVec2(705 + p.x, 38 + p.y), ImGui::GetColorU32(colors::lite_color), s.WindowRounding);

            ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(182 + p.x, 38 + p.y), ImVec2(705 + p.x, 41 + p.y), ImGui::GetColorU32(colors::general_color), s.WindowRounding);

            ImGui::GetForegroundDrawList()->AddText(ico, 25, ImVec2(670 + p.x, 7 + p.y), ImGui::GetColorU32(colors::gray_color), "G");

            tab_alpha = ImClamp(tab_alpha + (7.f * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);
            tab_add = ImClamp(tab_add + (std::round(50.f) * ImGui::GetIO().DeltaTime * (tabs == active_tab ? 1.f : -1.f)), 0.f, 1.f);

            if (tab_alpha == 0.f && tab_add == 0.f) active_tab = tabs;


            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, tab_alpha * s.Alpha);

            switch (active_tab) {

            case 0:
                ImGui::GetForegroundDrawList()->AddText(ico, 25, ImVec2(195 + p.x, 7 + p.y), ImGui::GetColorU32(colors::Text_Active), "A");
                ImGui::GetForegroundDrawList()->AddText(ImVec2(225 + p.x, 8 + p.y), ImGui::GetColorU32(colors::Text), "Visuals");

                break;

            case 1:
                ImGui::GetForegroundDrawList()->AddText(ico, 25, ImVec2(195 + p.x, 7 + p.y), ImGui::GetColorU32(colors::Text_Active), "B");
                ImGui::GetForegroundDrawList()->AddText(ImVec2(225 + p.x, 8 + p.y), ImGui::GetColorU32(colors::Text), "Aimbot");

                break;

            case 2:
                ImGui::GetForegroundDrawList()->AddText(ico, 25, ImVec2(195 + p.x, 7 + p.y), ImGui::GetColorU32(colors::Text_Active), "C");
                ImGui::GetForegroundDrawList()->AddText(ImVec2(225 + p.x, 8 + p.y), ImGui::GetColorU32(colors::Text), "Misc");

                break;
            }

            ImGui::SetCursorPos(ImVec2(203, 67));

            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0, 0, 0, 0));

            ImGui::BeginChild("General", ImVec2(487, 395), false);
            {
                ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetColorU32(colors::lite_color));
                switch (active_tab) {

                case 0:
                {

                    ImGui::BeginChild("ESP", ImVec2(225, 150), true);
                    {
                        ImGui::Checkbox("Enemy ESP", &var::enemyEsp);
                        ImGui::Checkbox("Team ESP", &var::teamEsp);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(243, 0));

                    ImGui::BeginChild("Colors", ImVec2(225, 150), true);
                    {
                        ImGui::ColorEdit4("Enemy Color", var::enemyColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                        ImGui::ColorEdit4("Team Color", var::teamColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoInputs);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(0, 170));

                    ImGui::BeginChild("Additional", ImVec2(225, 150), true);
                    {
                        ImGui::Checkbox("Name", &var::nameEsp);
                        ImGui::Checkbox("Health", &var::healthEsp);
                    }
                    ImGui::EndChild();
                }
                break;

                //////////////////////////////////////////////////////////

                case 1:
                {
                    ImGui::BeginChild("Aimbot", ImVec2(225, 155), true);
                    {
                        ImGui::Checkbox("Aimbot", &var::aimbot);
                        ImGui::Keybind("Aim Key", &var::aimKey, true);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(243, 0));

                    ImGui::BeginChild("Additional", ImVec2(225, 245), true);
                    {
                        ImGui::Checkbox("FOV Circle", &var::drawFov);
                        ImGui::SliderFloat("FOV Radius", &var::fov, 1, 300, "%.1f");
                        ImGui::Checkbox("Smooth", &var::smooth);
                        ImGui::SliderFloat("Smooth amount", &var::smoothAmount, 1, 100, "%.1f");
                    }
                    ImGui::EndChild();
                }
                break;
                
                case 2:
                {
                    ImGui::BeginChild("Player", ImVec2(225, 140), true);
                    {
                        ImGui::Checkbox("Infinite Health", &var::infHealth);
                        ImGui::Checkbox("Infinite Armor", &var::infArmor);
                    }
                    ImGui::EndChild();

                    ImGui::SetCursorPos(ImVec2(243, 0));

                    ImGui::BeginChild("Weapon", ImVec2(225, 265), true);
                    {
                        ImGui::Checkbox("No Spread", &var::noSpread);
                        ImGui::Checkbox("No Recoil", &var::noRecoil);
                        ImGui::Checkbox("No Pushback", &var::noPushback);
                        ImGui::Checkbox("Infinite Ammo", &var::infAmmo);
                        ImGui::Checkbox("No Gun Mvmnt", &var::noGunMvmnt);
                    }
                    ImGui::EndChild();
                }
                break;

                }
                ImGui::PopStyleColor(1);

                ImGui::Spacing();

                ImGui::EndChild();

                ImGui::PopStyleColor(1);
            }

            ImGui::PopStyleVar(1);

        }
        ImGui::End();
	}
}