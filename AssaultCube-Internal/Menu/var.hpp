#pragma once

namespace var {
    bool enemyEsp = false;
    bool teamEsp = false;
    bool enemyTrace = false;
    bool teamTrace = false;

    float enemyColor[4] = { 31.0f / 255.0f, 31.0f / 255.0f, 231.0f / 255.0f, 1.0f };
    float teamColor[4] = { 231.0f / 255.0f, 31.0f / 255.0f, 31.0f / 255.0f, 1.0f };

    bool nameEsp = false;
    bool healthEsp = false;

    bool aimbot = false;
    int aimKey = 0x43;
    bool drawFov = false;
    float fov = 100.f;
    bool smooth = false;
    float smoothAmount = 10.f;

    bool infHealth = false;
    bool infArmor = false;
    
    bool noSpread = false;
    bool noRecoil = false;
    bool noPushback = false;
    bool infAmmo = false;
    bool noGunMvmnt = false;
    bool fullBrightness = false;
    bool tpPlayers = false;
    float tpDistance = 10.0f;

    bool flyHack = false;
    int flyKey = 0x51;

    bool debugConsole = false;
}