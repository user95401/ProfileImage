#include "MenuLayerExt.hpp"
#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")
void MenuLayerExt::onProfileUpdateHttpResponse(CCHttpClient* client, CCHttpResponse* response) {
    std::vector<char>* responseData = response->getResponseData();
    std::string responseString(responseData->begin(), responseData->end());
    m_profileBtn->stopAllActions();
    m_profileBtn->runAction(CCFadeIn::create(0.0));
    if (responseString == "0") return;
    // URLDownloadToFile returns S_OK on success
    if (S_OK == URLDownloadToFile(NULL, responseString.c_str(), (std::filesystem::temp_directory_path() / ".ProfileBtnImage").string().c_str(), 0, NULL))
    {
        CCSprite* Profile = ModUtils::createSprite(".ProfileBtnImage");
        if (Profile->getContentSize().width > Profile->getContentSize().height)
            Profile->setScale(m_profileBtn->getContentSize().width / Profile->getContentSize().width);
        else
            Profile->setScale(m_profileBtn->getContentSize().height / Profile->getContentSize().height);
        Profile->setPosition({ m_profileBtn->getContentSize().width / 2, m_profileBtn->getContentSize().height / 2 });
        //Profile->setAnchorPoint(CCPoint());
        m_profileBtn->addChild(Profile);
        CCSprite* hiddenNode = dynamic_cast<CCSprite*>(m_profileBtn->getNormalImage());
        hiddenNode->runAction(CCRepeatForever::create(CCFadeOut::create(0.f)));
        m_profileBtn->setNormalImage(hiddenNode);
        m_profileBtn->setSelectedImage(hiddenNode);
        CCTextureCache::sharedTextureCache()->reloadTexture(".ProfileBtnImage");
    }
    else
    {
        AchievementNotifier::sharedState()->notifyAchievement("Profile Image", "Failed download Profile Image.\n", "deleteFilter_none_001.png", true);
    }
}

inline bool (__thiscall* MenuLayer_init)(MenuLayerExt*);
bool __fastcall MenuLayer_init_H(MenuLayerExt* self) {
    MenuLayer_init(self);
    self->me = self;
    twoTimesLayerInitHookEscape(self);//fucking works

    //profilebtn
    //Request
    CCHttpRequest* ProfileUpdateHttp = new CCHttpRequest;
    ProfileUpdateHttp->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
    ProfileUpdateHttp->setUrl(("http://user95401.undo.it/ProfileImage/get.php?accountID=" + std::to_string(GJAccountManager::sharedState()->m_nPlayerAccountID)).c_str());
    ProfileUpdateHttp->setResponseCallback(self, httpresponse_selector(MenuLayerExt::onProfileUpdateHttpResponse));
    CCHttpClient::getInstance()->send(ProfileUpdateHttp);
    ProfileUpdateHttp->release();

    self->m_profileBtn->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.3, 90), CCFadeTo::create(0.3, 160), nullptr)));

    return true;
}

void MenuLayerExt::CreateHooks() {
    HOOK(base + 0x1907B0, MenuLayer_init);
}