#include "core/Common.h"
#include "core/Config.h"
#include "sdk/GameSDK.h"
#include "hooks/Hooks.h"
#include "backends/imgui_impl_win32.h"
#include "backends/imgui_impl_dx11.h"
// Build: 2026-08-27 12:57 UTC+2 | Release v2.0

Il2CppResolver g_Il2Cpp;
HMODULE g_hDllModule = NULL;
HWND g_hWnd = NULL;
ID3D11Device* g_pd3dDevice = nullptr;
ID3D11DeviceContext* g_pd3dDeviceContext = nullptr;
ID3D11RenderTargetView* g_mainRenderTargetView = nullptr;
volatile bool g_IsInitialized = false;
volatile bool g_Uninjecting = false;
bool g_ShowMenu = false;

std::vector<CachedPlayerInfo> g_CachedPlayers;
CachedPlayerInfo              g_LocalPlayerInfo;
bool                          g_HasLocalPlayer = false;
std::vector<PlayerESPData>    g_ESPData;

// Settings definitions
bool  bEnableESP        = false;
bool  bEnableGlow       = false;
float fGlowIntensity    = 1.0f;
bool  bDrawBoxes        = false;
float fBoxThickness     = 1.8f;
bool  bDrawSkeleton     = false;
float fSkeletonThickness= 1.8f;
bool  bDrawHeadCircle   = false;
float fHeadCircleSize   = 1.0f;
bool  bDrawTracers      = false;
int   iTracerOrigin     = 0;
float fTracerThickness  = 1.8f;
bool  bDrawHealthBar    = false;
bool  bDrawInfoText     = false;
bool  bIgnoreTeammates  = false;
bool  bIgnoreLocal      = true;
bool  bIgnoreDead       = true;
float fMaxDistance      = 500.0f;

float colEnemy[4]     = { 1.0f, 0.22f, 0.35f, 1.0f };
float colTeam[4]      = { 0.20f, 0.70f, 1.00f, 1.0f };
float colSkeleton[4]  = { 0.95f, 0.95f, 0.98f, 0.90f };
float colTracers[4]   = { 1.0f, 0.85f, 0.20f, 0.80f };
float colHeadCircle[4]= { 1.0f, 0.35f, 0.50f, 1.0f };

bool  bEnableChams          = false;
int   iChamsStyle           = 0;
float fChamsAlpha           = 0.65f;
float fChamsJointSize       = 1.0f;
bool  bChamsVisibleOnly     = false;
float colChamsEnemyVis[4]   = { 1.0f, 0.20f, 0.40f, 0.75f };
float colChamsEnemyOcc[4]   = { 0.85f, 0.10f, 0.90f, 0.55f };
float colChamsTeamVis[4]    = { 0.20f, 0.70f, 1.00f, 0.75f };
float colChamsTeamOcc[4]    = { 0.10f, 0.40f, 0.80f, 0.50f };

bool  bEnableSilentAim    = false;
int   iSilentAimTarget    = 1;
float fSilentAimFOV       = 180.0f;
bool  bDrawSilentAimFOV   = false;
bool  bSilentAimFull360   = true;

bool  bEnableAimbot     = false;
int   iAimbotKey        = 0;
bool  bDrawAimbotFOV    = false;
int   iAimbotTarget     = 0;
float aimbotFOV         = 150.0f;
float aimbotSmooth      = 6.0f;
float aimbotMaxSpeed    = 35.0f;
bool  bAimbotAutoFire   = true;
bool  bAimbotWhileFlashed = false;
bool  bAimbotThroughSmoke = false;
float fKillDelay        = 0.300f;
float fMouseLockX       = 1.000f;
float fMouseLockY       = 1.000f;

bool  bRecoilCompensation = false;
int   iRecoilStartBullet  = 1;
float fRecoilX            = 0.000f;
float fRecoilY            = 0.000f;
float fRecoilSmooth       = 1.000f;

bool  bTriggerbot         = false;
bool  bTriggerbotHeadOnly = false;
float fTriggerbotDelay    = 0.050f;

bool  bEnableTeleportKill = false;
bool  bTeleportHoldKey    = false;
int   iTeleportKey        = 4;
int   iTeleportPosition   = 0;
int   iTeleportTargetMode = 0;
float fTeleportDistance   = 1.2f;
float fTeleportHeight     = 0.3f;
bool  bTeleportAutoShoot  = true;
bool  bTeleportLookAt     = true;
float fTeleportShootRate  = 45.0f;

bool  bEnableMassKill     = false;
float fMassKillInterval   = 80.0f;
int   iMassKillMode       = 1;

int   iSelectedWeaponIndex= 0;
bool  bInfiniteAmmo       = true;
bool  bOneHitKillDamage   = true;
bool  bRapidFire          = true;
bool  bInfiniteRange      = true;
bool  bWeaponSpawnBypass  = true;

bool  bEnableSpeedhack       = false;
float fSpeedMultiplier       = 2.5f;
bool  bEnableSuperJump       = false;
float fJumpMultiplier        = 2.0f;
bool  bInfiniteAirJump       = false;
bool  bZeroGravity           = false;
float fGravityMultiplier     = 1.0f;
bool  bBunnyhop              = false;
bool  bNoClip                = false;
float fNoClipSpeed           = 2.5f;
bool  bAntiKnockback         = false;

bool  bInfiniteGrappleRange  = false;
bool  bSuperGrappleSpeed     = false;
float fGrappleSpeedMult      = 2.5f;
bool  bInstantGrappleBoost   = false;
bool  bGrappleMagnetAim      = false;

bool  bCustomFOV             = false;
float fCustomFOVValue        = 100.0f;
bool  bFastLoadingOptimizer  = true;
bool  bFpsBoostUltra         = true;
bool  bDisableGameShadows    = true;
bool  bDisableFogAndBlur     = true;

bool  bGodMode               = false;
bool  bEndGameMatchTrigger   = false;
bool  bServerCrashActive     = false;
bool  bCrashAllPlayersNow    = false;
bool  bMapDestroyerActive    = false;

int   iTopNavTab             = 0;
int   iSidebarCategory       = 0;
char  szSearchQuery[64]      = "";

const char* const g_KeyNames[] = {
    "Left Alt [DEFAULT]",
    "Right Mouse [RMB]",
    "Left Shift",
    "Left Ctrl",
    "[X] Key",
    "[C] Key",
    "[V] Key",
    "[F] Key",
    "[CAPS LOCK]",
    "[Mouse 4] (Thumb 1)",
    "[Mouse 5] (Thumb 2)",
    "Always Active [Toggle]"
};
const int g_KeyNamesCount = 12;

bool IsKeyActive(int keyIndex) {
    switch (keyIndex) {
        case 0: return (GetAsyncKeyState(VK_MENU) & 0x8000) || (GetAsyncKeyState(VK_LMENU) & 0x8000);
        case 1: return (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        case 2: return (GetAsyncKeyState(VK_SHIFT) & 0x8000) || (GetAsyncKeyState(VK_LSHIFT) & 0x8000);
        case 3: return (GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(VK_LCONTROL) & 0x8000);
        case 4: return (GetAsyncKeyState('X') & 0x8000) != 0;
        case 5: return (GetAsyncKeyState('C') & 0x8000) != 0;
        case 6: return (GetAsyncKeyState('V') & 0x8000) != 0;
        case 7: return (GetAsyncKeyState('F') & 0x8000) != 0;
        case 8: return (GetAsyncKeyState(VK_CAPITAL) & 0x8000) != 0;
        case 9: return (GetAsyncKeyState(VK_XBUTTON1) & 0x8000) != 0;
        case 10: return (GetAsyncKeyState(VK_XBUTTON2) & 0x8000) != 0;
        case 11: return true;
        default: return (GetAsyncKeyState(VK_MENU) & 0x8000) != 0;
    }
}

void TraceLog(const char* category, const char* fmt, ...) {
    SYSTEMTIME st;
    GetLocalTime(&st);
    char timeBuf[32];
    snprintf(timeBuf, sizeof(timeBuf), "[%02d:%02d:%02d.%03d] ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    char msgBuf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, sizeof(msgBuf), fmt, args);
    va_end(args);

    char fullMsg[1200];
    snprintf(fullMsg, sizeof(fullMsg), "[%s] %s", category, msgBuf);

    char path[MAX_PATH];
    if (g_hDllModule && GetModuleFileNameA(g_hDllModule, path, MAX_PATH)) {
        std::string s(path);
        size_t pos = s.find_last_of("\\/");
        std::string logPath = (pos != std::string::npos) ? (s.substr(0, pos + 1) + "XUYBYA_Cheat.log") : "XUYBYA_Cheat.log";
        FILE* f = fopen(logPath.c_str(), "a");
        if (f) {
            fprintf(f, "%s%s\n", timeBuf, fullMsg);
            fclose(f);
        }
    }
}

void CheatLog(const char* fmt, ...) {
    char msgBuf[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, sizeof(msgBuf), fmt, args);
    va_end(args);
    TraceLog("CHEAT", "%s", msgBuf);
}

// ─── Uninject Thread ────────────────────────────────────────────────────────
static DWORD WINAPI UninjectThread(LPVOID /*lpParam*/) {
    g_Uninjecting = true;
    g_ShowMenu = false;
    bEnableESP = false;
    bEnableAimbot = false;
    bEnableTeleportKill = false;
    bGodMode = false;

    Sleep(120);
    Hooks::Shutdown();
    Sleep(80);

    if (g_IsInitialized) {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    Sleep(100);
    if (g_hDllModule) {
        FreeLibraryAndExitThread(g_hDllModule, 0);
    }
    return 0;
}

// ─── Main Initializer Thread ────────────────────────────────────────────────
static DWORD WINAPI InitThread(LPVOID lpParam) {
    g_hDllModule = (HMODULE)lpParam;

    CheatLog("========================================================");
    CheatLog("★ XUYBYA Modular Cheat Initialized");
    CheatLog("PID: %lu | Base: 0x%p", GetCurrentProcessId(), g_hDllModule);
    CheatLog("========================================================");

    while (!GetModuleHandleA("dxgi.dll") || !GetModuleHandleA("d3d11.dll")) Sleep(200);
    while (!GetModuleHandleA("GameAssembly.dll")) Sleep(300);

    Sleep(1200);

    __try {
        if (SDK::Initialize()) {
            CheatLog("[+] SDK and IL2CPP Initialized.");
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        CheatLog("[-] SDK::Initialize caught exception 0x%08X", GetExceptionCode());
    }

    __try {
        if (Hooks::Initialize()) {
            CheatLog("[+] Hooks Installed. Ready for battle!");
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER) {
        CheatLog("[-] Hooks::Initialize caught exception 0x%08X", GetExceptionCode());
    }

    Config::Load();
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(NULL, 0, InitThread, hModule, 0, NULL);
    }
    return TRUE;
}
