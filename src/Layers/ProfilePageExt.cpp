#include "ProfilePageExt.hpp"
#include "ProfileImagePopup.hpp"
#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")

void ProfilePageExt::onProfileUpdateHttpResponse(CCHttpClient* client, CCHttpResponse* response) {
    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());
    setBtn->stopAllActions();
    setBtn->runAction(CCFadeIn::create(0.0));
    if (responseString == "0") {
        if (GJAccountManager::sharedState()->m_nPlayerAccountID != m_nAccountID) setBtn->removeFromParent();
        return;
    }
    // URLDownloadToFile returns S_OK on success
    if (S_OK == URLDownloadToFile(NULL, responseString.c_str(), "geode/temp/.ProfileImage", 0, NULL))
    {
        CCTextureCache::sharedTextureCache()->reloadTexture(".ProfileImage");
        CCSprite* Profile = ModUtils::createSprite(".ProfileImage");
        if (Profile->getContentSize().width > Profile->getContentSize().height)
            Profile->setScale(setBtn->getContentSize().width / Profile->getContentSize().width);
        else
            Profile->setScale(setBtn->getContentSize().height / Profile->getContentSize().height);
        Profile->setPosition({ setBtn->getContentSize().width / 2, setBtn->getContentSize().height / 2 });
        //Profile->setAnchorPoint(CCPoint());
        setBtn->addChild(Profile);
        CCSprite* hiddenNode = dynamic_cast<CCSprite*>(setBtn->getNormalImage());
        hiddenNode->runAction(CCRepeatForever::create(CCFadeOut::create(0.f)));
        setBtn->setNormalImage(hiddenNode);
        setBtn->setSelectedImage(hiddenNode);
    }
    else
    {
        AchievementNotifier::sharedState()->notifyAchievement("Profile Image", "Failed download Profile Image.\n", "deleteFilter_none_001.png", true);
    }
}

inline bool(__thiscall* ProfilePage_init)(ProfilePageExt*, int, bool);//0x20ef00
bool __fastcall ProfilePage_init_H(ProfilePageExt* self, void*, int accID, bool idkB) {
    bool ret = ProfilePage_init(self, accID, idkB);
    twoTimesLayerInitHookEscape(self);
    self->setBtn = CCMenuItemSpriteExtra::create(ModUtils::createSprite("GJ_everyplayBtn_001.png"), self, menu_selector(ProfileImagePopup::createAndShowMe));
    self->setBtn->setPositionX(22.000f);
    self->setBtn->setPositionY(-48.000f);
    self->setBtn->CCMenuItemSpriteExtra::setScale(0.75f);
    self->setBtn->setContentSize({50.f, 50.f});
    self->setBtn->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.3, 90), CCFadeTo::create(0.3, 160),nullptr)));
    self->getButtonMenu()->addChild(self->setBtn, 10, 68302);
    return ret;
}

inline void(__thiscall* ProfilePage_loadPageFromUserInfo)(ProfilePageExt*, GJUserScore*);//0x210040
void __fastcall ProfilePage_loadPageFromUserInfo_H(ProfilePageExt* self, void*, GJUserScore* a2) {
    ProfilePage_loadPageFromUserInfo(self, a2);
    //Request
    CCHttpRequest* ProfileUpdateHttp = new CCHttpRequest;
    ProfileUpdateHttp->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    ProfileUpdateHttp->setTag(std::to_string(a2->getAccountID()).c_str());
    ProfileUpdateHttp->setUrl(("http://user95401.undo.it/ProfileImage/get.php?accountID=" + std::string(ProfileUpdateHttp->getTag())).c_str());
    ProfileUpdateHttp->setResponseCallback(self, httpresponse_selector(ProfilePageExt::onProfileUpdateHttpResponse));
    CCHttpClient::getInstance()->send(ProfileUpdateHttp);
    ProfileUpdateHttp->release();
}

void ProfilePageExt::CreateHooks() {
    HOOK(base + 0x20ef00, ProfilePage_init);
    HOOK(base + 0x210040, ProfilePage_loadPageFromUserInfo);
}