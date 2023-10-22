#include "ProfileImagePopup.hpp"
#include "mod_utils.hpp"
using namespace gd;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace std;

bool ProfileImagePopup::init() {
    if (!this->initWithColor({ 0, 0, 0, 75 })) return false;
    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);

    m_pLayer = CCLayer::create();
    this->addChild(m_pLayer);

    m_pButtonMenu = CCMenu::create();
    m_pLayer->addChild(m_pButtonMenu);

    auto bg = CCScale9Sprite::create("square01_001.png");
    bg->setContentSize({ 360.0f, 260.0f });
    m_pButtonMenu->addChild(bg);

    auto Title = CCLabelBMFont::create("Custom Profile Image", "goldFont.fnt");
    Title->setPositionY(105.f);
    //Title->setPositionX(105.f);
    m_pButtonMenu->addChild(Title);

    auto desc = CCLabelBMFont::create("ass", "chatFont.fnt", bg->getContentSize().width - 60);
    desc->setPositionY(80.f);
    //desc->setPositionX(105.f);
    desc->setAnchorPoint({ 0.5, 1.0 });
    desc->setString("You can set avatar that will see other players...");
    m_pButtonMenu->addChild(desc);

    auto btn1Sprite = ButtonSprite::create("Go to adding page", 0, false, "goldFont.fnt", "GJ_button_05.png", 0, 0.6);
    auto btn1 = CCMenuItemSpriteExtra::create(btn1Sprite, this, menu_selector(ProfileImagePopup::onBtn1));
    btn1->setPositionY(-88.f);
    //btn1->setPositionX(105.f);
    m_pButtonMenu->addChild(btn1);

    auto btn2Sprite = ButtonSprite::create("Cancel", 0, false, "goldFont.fnt", "GJ_button_06.png", 0, 0.6);
    auto btn2 = CCMenuItemSpriteExtra::create(btn2Sprite, this, menu_selector(ProfileImagePopup::onBtn2));
    btn2->setPositionY(-88.f);
    btn2->setPositionX(123.f);
    m_pButtonMenu->addChild(btn2);

    return true;
}
void ProfileImagePopup::keyBackClicked() {
    this->setTouchEnabled(false);
    this->setKeypadEnabled(false);
    this->removeMeAndCleanup();
}
void ProfileImagePopup::createAndShowMe(CCObject* sneder) {
    this->create()->show();
}
#include "ProfilePageExt.hpp"
void ProfileImagePopup::onBtn1(CCObject*) {
    CCApplication::sharedApplication()->openURL(("https://user95401.undo.it/ProfileImage/?accountID=" + to_string(GJAccountManager::sharedState()->m_nPlayerAccountID)).c_str());
    this->keyBackClicked();
}
void ProfileImagePopup::onBtn2(CCObject*) {
    this->keyBackClicked();
}
ProfileImagePopup* ProfileImagePopup::create() {
    ProfileImagePopup* pRet = new ProfileImagePopup();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}