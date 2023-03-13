#pragma once
#include <iostream>
#include "../Menu/menu.hpp"
#include "../Menu/var.hpp"
#include "offsets.hpp"

# define M_PI 3.14159265358979323846

namespace Game {
	class Entity
	{
	public:
		char pad_0000[4]; //0x0000
		float x; //0x0004
		float y; //0x0008
		float z; //0x000C
		char pad_0010[24]; //0x0010
		float x_foot; //0x0028
		float y_foot; //0x002C
		float z_foot; //0x0030
		float Yaw; //0x0034
		float Pitch; //0x0038
		char pad_003C[176]; //0x003C
		int32_t Health; //0x00EC
		int32_t Armor; //0x00F0
		char pad_00F4[273]; //0x00F4
		char Name[16]; //0x0204
		char pad_0214[247]; //0x0214
		int32_t TeamNum; //0x030C
		char pad_0310[87]; //0x0310
		class Weapon* CurrentWeapon; //0x0368
		char pad_036C[223]; //0x036C
	}; //Size: 0x044C

	class Weapon
	{
	public:
		char pad_0000[8]; //0x0000
		void* Owner; //0x0008
		class WeaponData* WeaponData; //0x000C
		int32_t* MagazinePointer; //0x0010
		int32_t* AmmoPointer; //0x0014
		char pad_0018[1036]; //0x0018
	}; //Size: 0x0424

	class WeaponData
	{
	public:
		char pad_0000[32]; //0x0000
		int16_t N00000253; //0x0020
		int16_t N0000035D; //0x0022
		int16_t N00000254; //0x0024
		int16_t N00000360; //0x0026
		int16_t N00000255; //0x0028
		int16_t N00000363; //0x002A
		int16_t N00000256; //0x002C
		int16_t N00000366; //0x002E
		int16_t N00000257; //0x0030
		int16_t N00000369; //0x0032
		int16_t N00000258; //0x0034
		int16_t N0000036C; //0x0036
		int16_t N00000259; //0x0038
		int16_t N0000036F; //0x003A
		int16_t N0000025A; //0x003C
		int16_t N00000372; //0x003E
		int16_t N0000025B; //0x0040
		int16_t N00000375; //0x0042
		int16_t N0000025C; //0x0044
		int16_t N00000378; //0x0046
		int16_t breakstheammoandgun; //0x0048
		int16_t N0000037B; //0x004A
		int16_t N0000025E; //0x004C
		int16_t N0000037E; //0x004E
		int16_t N0000025F; //0x0050
		int16_t spread; //0x0052
		int16_t recoilPushBack; //0x0054
		int16_t magSize; //0x0056
		int16_t N00000261; //0x0058
		int16_t gunMovement; //0x005A
		int16_t N00000262; //0x005C
		int16_t recoil1; //0x005E
		int16_t recoil2; //0x0060
	}; //Size: 0x0062

	struct ViewMatrix {
		float Matrix[16]{};
	};
	struct Vec2 {
		float x, y;
	};

	struct Vec3 {
		float x, y, z;
	};

	struct Vec4 {
		float x, y, z, w;
	};

	struct Angle
	{
		float Yaw, Pitch;
	};

	bool WorldToScreen(Vec3 pos, Vec2& screen, float matrix[16], int windowWidth, int windowHeight)
	{
		Vec4 clipCoords = {};

		clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
		clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
		clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
		clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

		if (clipCoords.w < 0.1f)
		{
			return false;
		}

		Vec3 TranslatedCoords;
		TranslatedCoords.x = clipCoords.x / clipCoords.w;
		TranslatedCoords.y = clipCoords.y / clipCoords.w;
		TranslatedCoords.z = clipCoords.z / clipCoords.w;

		screen.x = (windowWidth / 2 * TranslatedCoords.x) + (TranslatedCoords.x + windowWidth / 2);
		screen.y = -(windowHeight / 2 * TranslatedCoords.y) + (TranslatedCoords.y + windowHeight / 2);

		return true;
	}

	float Distance(Vec2 Position1, Vec2 Position2)
	{
		Vec3 Difference;
		Difference.x = Position1.x - Position2.x;
		Difference.y = Position1.y - Position2.y;
		return sqrtf(Difference.x * Difference.x + Difference.y * Difference.y);
	}

	float Distance3D(Vec3 Position1, Vec3 Position2)
	{
		return sqrtf(powf(Position2.x - Position1.x, 2) + powf(Position2.y - Position1.y, 2) + powf(Position2.z - Position1.z, 2));
	}

	Angle GetAngle(Vec3 Position1, Vec3 Position2)
	{
		Angle Result;
		Result.Yaw = -atan2f(Position2.x - Position1.x, Position2.y - Position1.y) / M_PI * 180.f + 180.f;
		Result.Pitch = asinf((Position2.z - Position1.z) / Distance3D(Position1, Position2)) * 180 / M_PI;

		return Result;
	}

	Entity* GetClosestEntity()
	{
		Entity* Target = nullptr;
		uintptr_t BaseAddress = (uintptr_t)GetModuleHandle("ac_client.exe");
		Entity* LocalPlayer = (Entity*)(*(uintptr_t*)(BaseAddress + Offsets::LocalEntity));
		ViewMatrix Matrix = *(ViewMatrix*)(BaseAddress + Offsets::ViewMatrix);
		int EntityCount = *(int*)(BaseAddress + Offsets::EntityCount);
		uintptr_t EntityList = *(uintptr_t*)(BaseAddress + Offsets::EntityList);

		for (int i = 1; i <= EntityCount; i++)
		{
			if (EntityList == NULL)
				continue;
			uintptr_t EntityPointer = *(uintptr_t*)(EntityList + i * 4);
			if (EntityPointer == NULL)
				continue;
			Entity* Enemy = (Entity*)(EntityPointer);
			if (LocalPlayer == NULL)
				continue;

			if (Enemy->Health < 0)
				continue;

			Vec2 TargetScreen;
			if (!WorldToScreen({ Enemy->x, Enemy->y, Enemy->z }, TargetScreen, Matrix.Matrix, 1280, 720)) continue;
			float DistanceToFOV = Distance({ 1280 / 2, 720 / 2 }, TargetScreen);
			if (DistanceToFOV < var::fov)
			{
				Target = Enemy;
			}
		}
		return Target;
	}
}