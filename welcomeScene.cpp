#include "WelcomeScene.h"
#include "MapScene.h"
#include "ui/CocosGUI.h" // 包含 Cocos2d-x UI 头文件

USING_NS_CC;

Scene* WelcomeScene::createScene()
{
    return WelcomeScene::create();
}

bool WelcomeScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // 获取窗口的大小
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建一个精灵作为背景
    auto background = Sprite::create("sky.jpg");
    if (background == nullptr)
    {
        CCLOG("Error while loading: %s", "sky.jpg");
        return false;
    }
    else
    {
        // 获取图片的大小
            const auto backgroundSize = background->getContentSize();

        // 计算缩放比例
        const float scaleX = visibleSize.width / backgroundSize.width;
        const float scaleY = visibleSize.height / backgroundSize.height;
        const float scale = std::max(scaleX, scaleY);

        // 设置背景精灵的缩放比例
        background->setScale(scale);

        // 设置背景精灵的位置为屏幕中心
        background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(background, 0);
    }

    // 创建 "游戏标题" 精灵
    auto titleSprite = Sprite::create("title.png");
    if (titleSprite == nullptr)
    {
        CCLOG("Error while loading: %s", "title.png");
        return false;
    }
    else
    {
        titleSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.6));
        titleSprite->setScale(1.8); // 调整标题大小
        this->addChild(titleSprite, 1);

        // 创建上下移动的动作
        auto moveUp = MoveBy::create(1.0f, Vec2(0, 10));
        auto moveDown = MoveBy::create(1.0f, Vec2(0, -10));
        auto sequence = Sequence::create(moveUp, moveDown, nullptr);
        auto repeat = RepeatForever::create(sequence);

        // 运行动作
        titleSprite->runAction(repeat);
    }

    // 按钮之间的间距
    const float buttonSpacing = visibleSize.width / 4;

    // 创建 "创建" 按钮
    auto createButton = ui::Button::create("create_normal.png", "create_selected.png");
    createButton->setPosition(Vec2(buttonSpacing, visibleSize.height * 0.2));
    createButton->setScale(1.8); // 调整按钮大小
    this->addChild(createButton, 1);
   
    // 添加点击事件监听器
    createButton->addClickEventListener([](Ref* sender) {
        auto mapScene = MapScene::createScene();
        if (mapScene != nullptr) {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, mapScene));
        }
        else {
            CCLOG("Error: Failed to create MapScene");
            // 处理错误，例如记录日志或创建默认场景
        }
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, mapScene));
        });

    // 创建 "加载" 按钮
    auto loadButton = ui::Button::create("load_normal.png", "load_selected.png");
    loadButton->setPosition(Vec2(buttonSpacing * 2, visibleSize.height * 0.2));
    loadButton->setScale(1.8); // 调整按钮大小
    this->addChild(loadButton, 1);

    // 创建 "退出" 按钮
    auto exitButton = ui::Button::create("exit_normal.png", "exit_selected.png");
    exitButton->setPosition(Vec2(buttonSpacing * 3, visibleSize.height * 0.2));
    exitButton->setScale(1.8); // 调整按钮大小
    this->addChild(exitButton, 1);

    // 添加点击事件监听器
    exitButton->addClickEventListener([](Ref* sender) {
        Director::getInstance()->end();
        });

    return true;
}