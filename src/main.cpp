#include <Geode/Geode.hpp>
using namespace geode::prelude;

void ChangeBtnImage(CCMenuItemSpriteExtra* asd, std::string name) {
    CCTextureCache::sharedTextureCache()->reloadTexture(name.c_str());
    auto news = CCSprite::create(name.c_str());
    if (!news or !asd) return;
    if (news->getContentSize().width > news->getContentSize().height)//if wide
        news->setScale(asd->getContentSize().width / news->getContentSize().width);
    else //square or portrait
        news->setScale(asd->getContentSize().height / news->getContentSize().height);
    asd->setNormalImage(news);//replace finally
    asd->setContentSize(news->getScaledContentSize());
}

#include <urlmon.h>
#pragma comment (lib, "urlmon.lib")
#include <regex>
std::string TempFileFromURL(std::string url, std::string filename) {
    auto file_path = Mod::get()->getTempDir() / filename;
    bool dwnlrtn = S_OK == URLDownloadToFile(NULL, url.data(), file_path.string().data(), 0, NULL);
    return (dwnlrtn ? file_path.string() : "");
}

std::string ImgGetUrl(int id, std::string name) {
    std::stringstream ret;
    ret << "http://profileimage.user95401.undo.it/?linker&";//get open
    ret << "id=" << id;//id
    ret << "&";//and
    ret << "name=" << name;//name
    return ret.str();
}

std::string SetupPageUrl(int id, std::string name) {
    std::stringstream ret;
    ret << "http://profileimage.user95401.undo.it/?";//get open
    ret << "id=" << id;//id
    ret << "&";//and
    ret << "name=" << name;//name
    return ret.str();
}

class ProfileImagePopup : public FLAlertLayer {
public:
    static ProfileImagePopup* create() {
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
    };
    virtual bool init() {
        if (!this->initWithColor({ 0, 0, 0, 75 })) return false;
        this->setTouchEnabled(true);
        this->setKeypadEnabled(true);

        m_mainLayer = CCLayer::create();
        this->addChild(m_mainLayer);

        m_mainLayer->setAnchorPoint({ 0.875f, 0.875f });//huh

        m_buttonMenu = CCMenu::create();
        m_mainLayer->addChild(m_buttonMenu);

        auto bg = CCScale9Sprite::create("square01_001.png");
        bg->setContentSize({ 360.0f, 160.0f });
        m_buttonMenu->addChild(bg);

        auto Title = CCLabelBMFont::create("Custom Profile Image", "goldFont.fnt");
        Title->setPositionY(50.f);
        m_buttonMenu->addChild(Title);

        auto desc = CCLabelBMFont::create("ass", "chatFont.fnt", bg->getContentSize().width - 60);
        desc->setPositionY(25.f);
        //desc->setPositionX(105.f);
        desc->setAnchorPoint({ 0.5, 1.0 });
        desc->setString("You can set avatar that will see other players...");
        m_buttonMenu->addChild(desc);

        auto btn1Sprite = ButtonSprite::create("Go to adding page", 0, false, "goldFont.fnt", "GJ_button_05.png", 0, 0.6);
        auto btn1 = CCMenuItemSpriteExtra::create(btn1Sprite, this, menu_selector(ProfileImagePopup::onBtn1));
        btn1->setPositionY(-42.f);
        btn1->setPositionX(-60.f);
        m_buttonMenu->addChild(btn1);

        auto btn2Sprite = ButtonSprite::create("Cancel", 0, false, "goldFont.fnt", "GJ_button_06.png", 0, 0.6);
        auto btn2 = CCMenuItemSpriteExtra::create(btn2Sprite, this, menu_selector(ProfileImagePopup::onBtn2));
        btn2->setPositionY(-42.f);
        btn2->setPositionX(110.f);
        m_buttonMenu->addChild(btn2);

        return true;
    };
    virtual void keyBackClicked() {
        this->setTouchEnabled(false);
        this->setKeypadEnabled(false);
        this->removeMeAndCleanup();
    };
    void createAndShowMe(CCObject*) {
        this->create()->show();
    };
    void onBtn1(CCObject*) {
        int id = GJAccountManager::sharedState()->m_accountID;
        std::string name = GJAccountManager::sharedState()->m_username;
        auto url = SetupPageUrl(id, name);
        CCApplication::sharedApplication()->openURL(url.c_str());
        this->keyBackClicked();
    };
    void onBtn2(CCObject*) {
        this->keyBackClicked();
    };
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MenuLayerExt, MenuLayer) {
	CCMenuItemSpriteExtra* profileBtn() {
        auto profilebutton = this->getChildByIDRecursive("profile-button");
		return static_cast<CCMenuItemSpriteExtra*>(profilebutton ? profilebutton : CCNode::create());
	}
    void onProfileUpdateHttpResponse(CCHttpClient* client, CCHttpResponse* response) {
        //responseString
        std::vector<char>* responseData = response->getResponseData();
        std::string responseString(responseData->begin(), responseData->end());
        //filenema
        std::string filenema = response->getHttpRequest()->getTag();
        //stop animating profileBtn
        profileBtn()->stopAllActions();
        profileBtn()->setOpacity(255);
        //bad response
        if (responseString == "0" or responseString == "" or response->getResponseCode() != 200) return;
        auto dwnloadedFile = TempFileFromURL(responseString, filenema);
        if (dwnloadedFile != "") {
            ChangeBtnImage(profileBtn(), dwnloadedFile);
            profileBtn()->m_colorEnabled = true;
            profileBtn()->m_animationEnabled = false;
        }
        else {
            Notification::create("Failed download Profile Image.", NotificationIcon::Error, 0.5f)->show();
            //AchievementNotifier::sha()->notifyAchievement("Profile Image", "Failed download Profile Image.\n", "deleteFilter_none_001.png", true);
        }
    }
    void ProfileUpdateHttpReq(float) {
        CCHttpRequest* ProfileUpdateHttp = new CCHttpRequest;
        ProfileUpdateHttp->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
        int id = GJAccountManager::sharedState()->m_accountID;
        std::string name = GJAccountManager::sharedState()->m_username;
        auto url = ImgGetUrl(id, name);
        ProfileUpdateHttp->setUrl(url.c_str());
        ProfileUpdateHttp->setTag(std::format("{}.{}.ProfileImage", id, name).c_str());
        ProfileUpdateHttp->setResponseCallback(this, httpresponse_selector(MenuLayerExt::onProfileUpdateHttpResponse));
        CCHttpClient::getInstance()->send(ProfileUpdateHttp);
        ProfileUpdateHttp->release();
        //profileBtn upd
        profileBtn()->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.3f, 90), CCFadeTo::create(0.3f, 160), nullptr)));
        //try set already dwnloadded a img
        std::string filenema = ProfileUpdateHttp->getTag(); 
        ChangeBtnImage(profileBtn(), filenema);
    };
	bool init() {
		if (!MenuLayer::init()) return false;
        if (Mod::get()->getSettingValue<bool>("DnttchMenuLayer")) return true;
		//Request
        ProfileUpdateHttpReq(0.f);
        return true;
	}
};

#include <Geode/modify/ProfilePage.hpp>
class $modify(ProfilePageExt, ProfilePage) {
    CCMenuItemSpriteExtra* profileBtn() {
        auto profilebutton = this->getChildByIDRecursive("profile-image");
        return static_cast<CCMenuItemSpriteExtra*>(profilebutton ? profilebutton : CCNode::create());
    }
    void onProfileUpdateHttpResponse(CCHttpClient * client, CCHttpResponse * response) {
        //responseString
        std::vector<char>* responseData = response->getResponseData();
        std::string responseString(responseData->begin(), responseData->end());
        //filenema
        std::string filenema = response->getHttpRequest()->getTag();
        //stop animating profileBtn
        profileBtn()->stopAllActions();
        profileBtn()->setOpacity(255);
        //bad response
        if (responseString == "0" or responseString == "" or response->getResponseCode() != 200) return;
        auto dwnloadedFile = TempFileFromURL(responseString, filenema);
        if (dwnloadedFile != "") {
            ChangeBtnImage(profileBtn(), dwnloadedFile);
        }
        else {
            Notification::create("Failed download Profile Image.", NotificationIcon::Error, 0.5f)->show();
            //AchievementNotifier::sha()->notifyAchievement("Profile Image", "Failed download Profile Image.\n", "deleteFilter_none_001.png", true);
        }
    }
    void ProfileUpdateHttpReq(float) {
        CCHttpRequest* ProfileUpdateHttp = new CCHttpRequest;
        ProfileUpdateHttp->setRequestType(CCHttpRequest::HttpRequestType::kHttpPost);
        int id = m_score->m_accountID;
        std::string name = m_score->m_userName;
        auto url = ImgGetUrl(id, name);
        ProfileUpdateHttp->setUrl(url.c_str());
        ProfileUpdateHttp->setTag(std::format("{}.{}.ProfileImage", id, name).c_str());
        ProfileUpdateHttp->setResponseCallback(this, httpresponse_selector(ProfilePageExt::onProfileUpdateHttpResponse));
        CCHttpClient::getInstance()->send(ProfileUpdateHttp);
        ProfileUpdateHttp->release();
        ///profileBtn upd
        profileBtn()->runAction(CCRepeatForever::create(CCSequence::create(CCFadeTo::create(0.3f, 90), CCFadeTo::create(0.3f, 160), nullptr)));
        //try set already dwnloadded a img
        std::string filenema = ProfileUpdateHttp->getTag();
        ChangeBtnImage(profileBtn(), filenema);
    };
    bool init(int accountID, bool ownProfile) {
        if (!ProfilePage::init(accountID, ownProfile)) return false;
        if (Mod::get()->getSettingValue<bool>("DnttchProfilePage")) return true;
        /// p o i n
        CCMenuItemSpriteExtra* setBtn = CCMenuItemSpriteExtra::create(CCSprite::createWithSpriteFrameName("bgIcon_10_001.png"), this, /*nullptr);*/menu_selector(ProfileImagePopup::createAndShowMe));
        setBtn->setID("profile-image");
        setBtn->setPositionX(396.f);
        setBtn->setPositionY(-26.000f);
        setBtn->setScale(1.35f);
        setBtn->m_baseScale = setBtn->getScale();//unselect fixa
        setBtn->m_colorEnabled = true;
        setBtn->m_animationEnabled = false;
        this->m_buttonMenu->addChild(setBtn, 10, 68302);
        this->m_buttonMenu->setZOrder(20);//overlapo
        //socialsmenu
        auto socialsmenu = this->getChildByIDRecursive("socials-menu");
        if(socialsmenu) socialsmenu->setPositionX(socialsmenu->getPositionX() + 40);
        //lol
        if(ownProfile and not Mod::get()->getSettingValue<bool>("DAOPFC")) m_mainLayer->setAnchorPoint({ 0.1f, 0.3f });//huh
        return true;
    }
    void loadPageFromUserInfo(GJUserScore* p0) {
        ProfilePage::loadPageFromUserInfo(p0);
        if (Mod::get()->getSettingValue<bool>("DnttchProfilePage")) return;
        //Request
        ProfileUpdateHttpReq(0.f);
        auto statsmenu = this->getChildByIDRecursive("stats-menu");
        auto background = this->getChildByIDRecursive("background");
        if (statsmenu and background) {
            statsmenu->setScale((background->getContentSize().width - 132) / statsmenu->getContentSize().width);
        };
    }
};