#include "role.h"
#include "ui/CocosGUI.h"
#include "home.h"
USING_NS_CC;

Scene* CharacterRole::createScene() {
    return CharacterRole::create();
}

bool CharacterRole::init() {
    if (!Scene::init()) {
        return false;
    }

    // 获取屏幕可见区域大小
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    // 添加蓝天背景
    auto background = Sprite::create("sky.jpg");
    if (background == nullptr) {
        CCLOG("Error while loading: %s", "sky.jpg");
        return false;
    }
    else {
        const auto backgroundSize = background->getContentSize();
        const float scaleX = visibleSize.width / backgroundSize.width;
        const float scaleY = visibleSize.height / backgroundSize.height;
        background->setScale(scaleX, scaleY);
        background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(background, 0);
    }

    // 添加木板框
    auto frame = Sprite::create("letterBG..png");
    if (frame == nullptr) {
        CCLOG("Error while loading: %s", "letterBG..png");
        return false;
    }
    else {
        const auto frameSize = frame->getContentSize();
        const float frameScaleX = (visibleSize.width * 0.8) / frameSize.width; // 调整缩放比例
        const float frameScaleY = (visibleSize.height * 0.8) / frameSize.height; // 调整缩放比例
        frame->setScale(frameScaleX, frameScaleY);
        frame->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(frame, 1);
    }

    // 添加固定显示的人物形象
    auto characterSprite = Sprite::create("stand.png");
    if (characterSprite == nullptr) {
        CCLOG("Error while loading: %s", "stand.png");
        return false;
    }
    else {
        const auto characterSize = characterSprite->getContentSize();
        const float characterScaleX = (visibleSize.width / 5) / characterSize.width; // 调整缩放比例
        const float characterScaleY = (visibleSize.height / 3) / characterSize.height; // 调整缩放比例
        characterSprite->setScale(characterScaleX, characterScaleY);
        characterSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 150));
        this->addChild(characterSprite, 2);
    }

    // 工具函数: 创建文本框背景
    auto createTextBoxBg = [this](const Vec2& position) {
        auto textBoxBg = Sprite::create("textBox..png"); // 文本框背景图片
        if (textBoxBg != nullptr) {
            textBoxBg->setPosition(position);
            textBoxBg->setScale(1.3); // 调整背景大小
            this->addChild(textBoxBg, 2);
        }
        else {
            CCLOG("Error while loading: %s", "textBox..png");
        }
        return textBoxBg;
        };

    // 工具函数: 创建文本内容
    auto createLabel = [this](const std::string& text, const Vec2& position) {
        auto label = Label::createWithTTF(text, "fonts/Marker Felt.ttf", 24);
        if (label != nullptr) {
            label->setPosition(position);
            label->setTextColor(Color4B::BLACK);
            this->addChild(label, 3);
        }
        else {
            CCLOG("Error creating label: %s", text.c_str());
        }
        return label;
        };

    // 添加姓名文本框和内容
    auto nameBoxPosition = Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50);
    createTextBoxBg(nameBoxPosition);
    createLabel("NAME: Frank", nameBoxPosition);

    // 添加性别文本框和内容
    auto genderBoxPosition = Vec2(visibleSize.width / 2, visibleSize.height / 2 - 110);
    createTextBoxBg(genderBoxPosition);
    createLabel("GENDER: MAlE", genderBoxPosition);

    // 添加农场名字文本框和内容
    auto farmBoxPosition = Vec2(visibleSize.width / 2, visibleSize.height / 2 - 170);
    createTextBoxBg(farmBoxPosition);
    createLabel("FARMNAME: Disneyland", farmBoxPosition);

    // 创建 "下一步" 按钮
    auto nextButton = ui::Button::create("create_normal.png", "create_selected.png");
    if (nextButton == nullptr)
    {
        CCLOG("Error while loading: %s", "create_normal.png");
        return false;
    }
    else {
        nextButton->setPosition(Vec2(visibleSize.width - 100, 50)); // 右下角位置
        nextButton->setScale(1.7); // 缩放按钮大小
        this->addChild(nextButton, 1);

        // 添加点击事件监听器
        nextButton->addClickEventListener([](Ref* sender) {
            auto mapScene = HomeScene::createScene();
            if (mapScene != nullptr) {
                Director::getInstance()->replaceScene(TransitionFade::create(0.5, mapScene));
            }
            else {
                CCLOG("Error: Failed to create MapScene");
            }
            });
    }

    return true;
}
