﻿#pragma once
#include "hooks.hpp"
#include "mod_utils.hpp"
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace gd;

class MenuLayerExt : public MenuLayer {
public:
    inline static MenuLayerExt* me;
    void onProfileUpdateHttpResponse(CCHttpClient*, CCHttpResponse*);
    static void CreateHooks();
};

