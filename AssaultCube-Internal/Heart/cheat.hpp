#pragma once
#include "../Menu/menu.hpp"
#include "../Menu/var.hpp"
#include "game.hpp"
#include "globals.hpp"
#include "offsets.hpp"

namespace egirl
{
	void ESP() {
		uintptr_t ent = *(uintptr_t*)(Globals::BaseAddress + Offsets::LocalEntity);
		Game::Entity* LocalPlayer = (Game::Entity*)(ent);
		Game::ViewMatrix Matrix = *(Game::ViewMatrix*)(Globals::BaseAddress + Offsets::ViewMatrix);
		int EntityCount = *(int*)(Globals::BaseAddress + Offsets::EntityCount);
		uintptr_t EntityList = *(uintptr_t*)(Globals::BaseAddress + Offsets::EntityList);

		for (int i = 1; i <= EntityCount; i++)
		{
			if (EntityList == NULL)
				continue;
			uintptr_t EntityPointer = *(uintptr_t*)(EntityList + i * 4);
			if (EntityPointer == NULL)
				continue;
			Game::Entity* Enemy = (Game::Entity*)(EntityPointer);
			if (LocalPlayer == NULL)
				continue;
			if (Enemy->Health < 0)
				continue;

			Game::Vec2 PlayerCoords;
			if (!WorldToScreen({ Enemy->x, Enemy->y, Enemy->z }, PlayerCoords, Matrix.Matrix, 1280, 720)) continue;

			Game::Vec2 FeetCoords;
			if (!WorldToScreen({ Enemy->x_foot, Enemy->y_foot, Enemy->z_foot }, FeetCoords, Matrix.Matrix, 1280, 720)) continue;

			float EntityHeight = PlayerCoords.y - FeetCoords.y;
			float EntityWidth = EntityHeight / 2.5;

			float ESPBottom = FeetCoords.y;
			float ESPLeft = FeetCoords.x + (EntityWidth / 2);
			float ESPRight = FeetCoords.x - (EntityWidth / 2);
			float ESPTop = PlayerCoords.y + (EntityHeight * 0.1);

			if (Enemy->TeamNum != LocalPlayer->TeamNum && var::enemyEsp)
			{
				ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft, ESPTop }, { ESPRight, ESPTop }, ImColor(var::enemyColor[0], var::enemyColor[1], var::enemyColor[2]));
				ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft, ESPBottom }, { ESPRight, ESPBottom }, ImColor(var::enemyColor[0], var::enemyColor[1], var::enemyColor[2]));
				ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft, ESPTop }, { ESPLeft, ESPBottom }, ImColor(var::enemyColor[0], var::enemyColor[1], var::enemyColor[2]));
				ImGui::GetBackgroundDrawList()->AddLine({ ESPRight, ESPTop }, { ESPRight, ESPBottom }, ImColor(var::enemyColor[0], var::enemyColor[1], var::enemyColor[2]));

				if (var::nameEsp)
					ImGui::GetBackgroundDrawList()->AddText(esp_text, 19, ImVec2(ESPLeft, ESPTop - 20.f), ImColor(var::enemyColor[0], var::enemyColor[1], var::enemyColor[2]), Enemy->Name);

				if (var::healthEsp) {
					float HPRight = Enemy->Health * (ESPRight - ESPLeft) / 100;
					ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft, ESPBottom + 20.f }, { ESPRight, ESPBottom + 20.f }, ImColor(50, 205, 50), 5.f);
					ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft + HPRight, ESPBottom + 20.f }, { ESPRight, ESPBottom + 20.f }, ImColor(128, 128, 128), 5.f);
				}
					
				//ImGui::GetBackgroundDrawList()->AddText(ImVec2(ESPLeft, ESPTop - 20.f), ImColor(var::teamColor[0], var::teamColor[1], var::enemyColor[2]), Enemy->Name);
			}
			else if(Enemy->TeamNum == LocalPlayer->TeamNum && var::teamEsp)
			{
				if (LocalPlayer->Yaw == Enemy->Yaw) return;

				ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft, ESPTop }, { ESPRight, ESPTop }, ImColor(var::teamColor[0], var::teamColor[1], var::teamColor[2]));
				ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft, ESPBottom }, { ESPRight, ESPBottom }, ImColor(var::teamColor[0], var::teamColor[1], var::teamColor[2]));
				ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft, ESPTop }, { ESPLeft, ESPBottom }, ImColor(var::teamColor[0], var::teamColor[1], var::teamColor[2]));
				ImGui::GetBackgroundDrawList()->AddLine({ ESPRight, ESPTop }, { ESPRight, ESPBottom }, ImColor(var::teamColor[0], var::teamColor[1], var::teamColor[2]));

				if (var::nameEsp)
					ImGui::GetBackgroundDrawList()->AddText(esp_text, 19, ImVec2(ESPLeft, ESPTop - 20.f), ImColor(var::teamColor[0], var::teamColor[1], var::teamColor[2]), Enemy->Name);

				if (var::healthEsp) {
					float HPRight = Enemy->Health * (ESPRight - ESPLeft) / 100;
					ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft, ESPBottom + 20.f }, { ESPRight, ESPBottom + 20.f }, ImColor(50, 205, 50), 5.f);
					ImGui::GetBackgroundDrawList()->AddLine({ ESPLeft + HPRight, ESPBottom + 20.f }, { ESPRight, ESPBottom + 20.f }, ImColor(128,128,128), 5.f);
				}
				//ImGui::GetBackgroundDrawList()->AddText(ImVec2(ESPLeft, ESPTop - 20.f), ImColor(var::teamColor[0], var::teamColor[1], var::teamColor[2]), Enemy->Name);
			}

		}
	}

	void Aimbot() {
		if (var::drawFov) ImGui::GetBackgroundDrawList()->AddCircle({ 1280 / 2, 720 / 2 }, var::fov, ImColor(255, 255, 255));
		if (!var::aimbot) return;

		uintptr_t ent = *(uintptr_t*)(Globals::BaseAddress + Offsets::LocalEntity);
		Game::Entity* LocalPlayer = (Game::Entity*)(ent);
		Game::ViewMatrix Matrix = *(Game::ViewMatrix*)(Globals::BaseAddress + Offsets::ViewMatrix);
		int EntityCount = *(int*)(Globals::BaseAddress + Offsets::EntityCount);
		uintptr_t EntityList = *(uintptr_t*)(Globals::BaseAddress + Offsets::EntityList);

		if (GetAsyncKeyState(var::aimKey))
		{
			Game::Entity* Enemy = Game::GetClosestEntity();
			if (LocalPlayer == NULL || Enemy == NULL)
				return;
			if (Enemy->Health < 0 || Enemy->TeamNum == LocalPlayer->TeamNum)
				return;


			Game::Angle Angle = Game::GetAngle({ LocalPlayer->x, LocalPlayer->y, LocalPlayer->z }, { Enemy->x, Enemy->y, Enemy->z });

			Game::Angle Smooth = { Angle.Yaw - LocalPlayer->Yaw,  Angle.Pitch - LocalPlayer->Pitch};

			if (var::smooth)
			{
				LocalPlayer->Yaw += Smooth.Yaw / var::smoothAmount;
				LocalPlayer->Pitch += Smooth.Pitch / var::smoothAmount;
			}
			else
			{
				LocalPlayer->Yaw = Angle.Yaw;
				LocalPlayer->Pitch = Angle.Pitch;
			}
			
		}
	}

	void Misc() {
		uintptr_t ent = *(uintptr_t*)(Globals::BaseAddress + Offsets::LocalEntity);
		Game::Entity* LocalPlayer = (Game::Entity*)(ent);
		if (LocalPlayer->CurrentWeapon == NULL) return;

		if (var::infHealth) LocalPlayer->Health = 1337;
		if (var::infArmor) LocalPlayer->Armor = 1337;
		if (var::noSpread) LocalPlayer->CurrentWeapon->WeaponData->spread = 0;
		if (var::noRecoil) {
			LocalPlayer->CurrentWeapon->WeaponData->recoil1 = 0;
			LocalPlayer->CurrentWeapon->WeaponData->recoil2 = 0;
		}
		if (var::noPushback) LocalPlayer->CurrentWeapon->WeaponData->recoilPushBack = 0;
		if (var::infAmmo) *LocalPlayer->CurrentWeapon->AmmoPointer = 999;
		if (var::noGunMvmnt) LocalPlayer->CurrentWeapon->WeaponData->gunMovement = 0;
	}
}