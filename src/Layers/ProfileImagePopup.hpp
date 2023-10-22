#pragma once

#include <gd.h>
using namespace cocos2d;
using namespace gd;

class ProfileImagePopup : public FLAlertLayer {
public:
    static ProfileImagePopup* create();
    virtual bool init();
    virtual void keyBackClicked();
    void createAndShowMe(CCObject*);
    void onBtn1(CCObject*);
    void onBtn2(CCObject*);
};