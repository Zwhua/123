#include "adventure.h"
#include "SimpleAudioEngine.h"
#include "Hero.h"
#include "mine.h"
#include "town.h" // 导入 town 头文件

USING_NS_CC;

Scene* adventure::createScene()
{
    return adventure::create();
}

bool adventure::init()
{
    if (!Scene::init())
    {
        CCLOG("Error: Failed to initialize Scene");
        return false;
    }

    // 获取完整的文件路径
    std::string mapPath2 = FileUtils::getInstance()->fullPathForFilename("adventure.tmx");
    if (mapPath2.empty())
    {
        CCLOG("Error: TMX file not found: adventure.tmx");
        return false;
    }

    // 加载 Tiled 地图
    map2 = TMXTiledMap::create(mapPath2);
    if (map2 == nullptr)
    {
        CCLOG("Error while loading: %s", mapPath2.c_str());
        return false;
    }
    else
    {
        CCLOG("Map loaded: %s", mapPath2.c_str());
        map2->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点为中心
        map2->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2));

        // 放大视图
        float scaleFactor = 3.0f;
        map2->setScale(scaleFactor);
        this->addChild(map2, 0, 0);
    }

    // 添加角色
    _hero = Hero::create();
    _hero->setPosition(Vec2(400,400));
    _hero->setScale(2.0f);
    this->addChild(_hero, 1, 1);

    // 初始化移动标志
    _isMovingUp = false;
    _isMovingDown = false;
    _isMovingLeft = false;
    _isMovingRight = false;

    // 添加键盘事件监听器
    addKeyboardListener();

    // 添加鼠标点击事件监听器
    addMouseListener();

    // 调度更新方法
    this->schedule(CC_SCHEDULE_SELECTOR(adventure::update), 0.016f); // 约 60 FPS

    return true;
}

void adventure::addKeyboardListener()
{
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode keyCode, Event* event) {
        if (_hero == nullptr) {
            CCLOG("Error: Hero is not initialized");
            return;
        }

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
        if (_hero == nullptr) {
            CCLOG("Error: Hero is not initialized");
            return;
        }

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

void adventure::addMouseListener()
{
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = [this](Event* event) {
        EventMouse* e = dynamic_cast<EventMouse*>(event);
        Vec2 clickPosition = e->getLocationInView();

        // 获取屏幕中心位置
        Vec2 screenCenter = Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2);

        // 检查点击是否在屏幕中心附近
        if (clickPosition.distance(screenCenter) < 100.0f) // 距离阈值，可以根据需要调整
        {
            // 切换到 mine 场景
            auto mineScene = mine::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, mineScene));
        }

        // 检查角色是否在地图正中心左边
        Vec2 heroPosition = _hero->getPosition();
        if (heroPosition.x < screenCenter.x)
        {
            // 切换到 town 场景，并设置角色位置为地图的正右边
            auto townScene = townScene::createScene(true);
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, townScene));
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void adventure::update(float dt)
{
    if (_hero == nullptr) {
        CCLOG("Error: Hero is not initialized");
        return;
    }

    Vec2 heroPosition = _hero->getPosition();
    float moveSpeed = 100.0f * dt; // 增加移动速度

    if (_isMovingUp)
    {
        heroPosition.y += moveSpeed;
    }
    if (_isMovingDown)
    {
        heroPosition.y -= moveSpeed;
    }
    if (_isMovingLeft)
    {
        heroPosition.x -= moveSpeed;
    }
    if (_isMovingRight)
    {
        heroPosition.x += moveSpeed;
    }

    // 更新人物位置
    _hero->setPosition(heroPosition);
}
