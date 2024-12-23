#include "home.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "MapScene.h"

USING_NS_CC;

Scene* HomeScene::createScene() {
    return HomeScene::create();
}

void HomeScene::createHero()
{
    // 创建一个精灵作为主人公
    _hero = Hero::create();
    if (_hero == nullptr)
    {
        log("Error: Failed to create Hero");
        return;
    }
    else
    {
        log("Hero created successfully");
        const auto visibleSize = Director::getInstance()->getVisibleSize();
        _hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_hero, 1);
    }
}

bool HomeScene::init() {
    if (!Scene::init()) {
        return false;
    }

    // 加载 home.png 并创建背景
    auto sprite = Sprite::create(imagePath);
    if (sprite == nullptr) {
        CCLOG("Error while loading: %s", imagePath.c_str());
        return false;
    }

    // 获取屏幕大小并将背景放置在屏幕中心
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    sprite->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));

    // 设置背景的缩放比例
    sprite->setScale(3.2); // 将图片放大

    // 将背景添加到场景中
    this->addChild(sprite, 0);

    // 创建人物精灵
    _hero = Hero::create();
    if (_hero != nullptr)
    {
        _hero->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点为中心
        _hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        _hero->setScale(2.0); // 将人物放大两倍
        this->addChild(_hero, 2, 1); // 确保人物的 z-order 为 2
    }
    else
    {
        CCLOG("Error: Failed to create Hero");
    }

    // 初始化上一次移动方向
    _lastDirection = Hero::Direction::DOWN;

    // 添加键盘事件监听器
    addKeyboardListener();

    // 添加鼠标点击事件监听器
    addMouseListener();

    // 调度更新方法
    this->schedule(CC_SCHEDULE_SELECTOR(HomeScene::updateMapPosition), 0.01f);

    return true;
}

void HomeScene::addKeyboardListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {

        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_W:
            _isMovingUp = true;
            _hero->playWalkAnimation(Hero::Direction::UP);
            _lastDirection = Hero::Direction::UP;
            break;
        case EventKeyboard::KeyCode::KEY_S:
            _isMovingDown = true;
            _hero->playWalkAnimation(Hero::Direction::DOWN);
            _lastDirection = Hero::Direction::DOWN;
            break;
        case EventKeyboard::KeyCode::KEY_A:
            _isMovingLeft = true;
            _hero->playWalkAnimation(Hero::Direction::LEFT);
            _lastDirection = Hero::Direction::LEFT;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            _isMovingRight = true;
            _hero->playWalkAnimation(Hero::Direction::RIGHT);
            _lastDirection = Hero::Direction::RIGHT;
            break;
        default:
            break;
        }
        };
    listener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event* event) {

        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_W:
            _isMovingUp = false;
            break;
        case EventKeyboard::KeyCode::KEY_S:
            _isMovingDown = false;
            break;
        case EventKeyboard::KeyCode::KEY_A:
            _isMovingLeft = false;
            break;
        case EventKeyboard::KeyCode::KEY_D:
            _isMovingRight = false;
            break;
        default:
            break;
        }
        if (!_isMovingUp && !_isMovingDown && !_isMovingLeft && !_isMovingRight)
        {
            _hero->stopWalkAnimation(_lastDirection);
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HomeScene::addMouseListener()
{
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = [this](EventMouse* event) {
        Vec2 clickPosition = event->getLocation();
        CCLOG("Mouse clicked at: (%f, %f)", clickPosition.x, clickPosition.y);

    // 检查点击位置是否在特定区域内
    Rect sleepArea = Rect(800, 580, 120, 120); // 床的区域为 (800, 580) 到 (920, 700)
    if (sleepArea.containsPoint(clickPosition))
    {
        // 触发睡觉事件
        sleep();
    }
};
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HomeScene::sleep()
{
    // 创建一个黑色的层覆盖屏幕
    auto blackLayer = LayerColor::create(Color4B(0, 0, 0, 255));
    this->addChild(blackLayer, 3);

    // 创建一个标签显示 "SLEEP..."
    auto sleepLabel = Label::createWithTTF("SLEEP...", "fonts/Marker Felt.ttf", 38);
    sleepLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
    blackLayer->addChild(sleepLabel);

    // 延迟 5 秒后移除黑色层和标签
    blackLayer->runAction(Sequence::create(
        DelayTime::create(5.0f),
        CallFunc::create([blackLayer]() {
            blackLayer->removeFromParent();
            }),
        nullptr));
}


void HomeScene::updateMapPosition(float dt)
{
    if (_hero == nullptr) {
        return;
    }
    Vec2 heroPosition = _hero->getPosition();
    bool heroMoved = false;

    if (_isMovingUp)
    {
        heroPosition.y += 75.0f * dt;
        heroMoved = true;
    }

    if (_isMovingDown)
    {
        heroPosition.y -= 75.0f * dt;
        heroMoved = true;
    }

    if (_isMovingLeft)
    {
        heroPosition.x -= 75.0f * dt;
        heroMoved = true;
    }

    if (_isMovingRight)
    {
        heroPosition.x += 75.0f * dt;
        heroMoved = true;
    }

    if (heroPosition.y < 0)
    {
        auto mapScene = MapScene::createScene();
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, mapScene));
        return;
    }

    // 更新地图位置或人物位置
    _hero->setPosition(heroPosition);
}
