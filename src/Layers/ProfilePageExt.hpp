#pragma once
#include "hooks.hpp"
#include "mod_utils.hpp"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

class ProfilePageExt : public ProfilePage {
public:
    inline static ProfilePageExt* me;
    CCMenuItemSpriteExtra* setBtn;
    LoadingCircle* LoadingCircle_;
    void onProfileUpdateHttpResponse(CCHttpClient*, CCHttpResponse*);
    static void CreateHooks();
};
