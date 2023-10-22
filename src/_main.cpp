#include "mod_utils.hpp"
#include "hooks.hpp"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

#include "Layers/ProfilePageExt.hpp"
#include "Layers/MenuLayerExt.hpp"

DWORD WINAPI PROCESS_ATTACH(void* hModule) {
    MH_SafeInitialize(); //CCMessageBox("just Initialized", __FUNCTION__);
    ProfilePageExt::CreateHooks();
    MenuLayerExt::CreateHooks();
    CCFileUtils::sharedFileUtils()->addSearchPath(std::filesystem::temp_directory_path().string().c_str());
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
        CreateThread(0, 0, PROCESS_ATTACH, hModule, 0, 0);
    return TRUE;
}
