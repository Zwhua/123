#include "MapScene.h"
#include "Hero.h"

USING_NS_CC;

Scene* MapScene::createScene()
{
    auto scene = MapScene::create();
    if (scene == nullptr)
    {
        CCLOG("Error: Failed to create MapScene");
    }
    return scene;
}

bool MapScene::init()
{
    if (!Scene::init())
    {
        CCLOG("Error: Failed to initialize Scene");
        return false;
    }

    // 获取窗口的大小
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    // 获取完整的文件路径
    std::string mapPath = FileUtils::getInstance()->fullPathForFilename("last.tmx");
    if (mapPath.empty())
    {
        CCLOG("Error: TMX file not found: last.tmx");
        return false;
    }

    // 加载 Tiled 地图
    _map = TMXTiledMap::create(mapPath);
    if (_map == nullptr)
    {
        CCLOG("Error while loading: %s", mapPath.c_str());
        return false;
    }
    else
    {
        CCLOG("Map loaded: %s", mapPath.c_str());
        _map->setPosition(Vec2(0, 0));
        this->addChild(_map, 0, 0); // 确保地图的 z-order 为 0
    }

    // 创建人物精灵
    _hero = Hero::create();
    if (_hero != nullptr)
    {
        _hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_hero, 1, 1); // 确保人物的 z-order 为 1
        CCLOG("Hero created and added to the scene");
    }
    else
    {
        CCLOG("Error: Failed to create Hero");
    }

    // 初始化上一次移动方向
    _lastDirection = Hero::Direction::DOWN;

    // 添加键盘事件监听器
    addKeyboardListener();

    // 调度更新方法
    this->schedule(CC_SCHEDULE_SELECTOR(MapScene::updateMapPosition), 0.01f);

    return true;
}

void MapScene::createHero()
{
    // 创建一个精灵作为主人公
    _hero = Hero::create();
    if (_hero == nullptr)
    {
        CCLOG("Error while loading: %s", "Hero");
        return;
    }
    else
    {
        const auto visibleSize = Director::getInstance()->getVisibleSize();
        _hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_hero, 1);
    }
}

void MapScene::addKeyboardListener()
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


void MapScene::updateMapPosition(float dt)
{
    if (_hero == nullptr) {
        CCLOG("Error: Hero is not initialized");
        return;
    }

    Vec2 mapPosition = _map->getPosition();
    Vec2 heroPosition = _hero->getPosition();

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto mapSize = _map->getContentSize();

    bool mapMoved = false;
    bool heroMoved = false;

    Vec2 newHeroPosition = heroPosition;

    if (_isMovingUp)
    {
        if (mapPosition.y > visibleSize.height - mapSize.height)
        {
            mapPosition.y -= 50.0f * dt;
            mapMoved = true;
        }
        else if (heroPosition.y < mapSize.height - _hero->getContentSize().height / 2)
        {
            heroPosition.y += 50.0f * dt;
            heroMoved = true;
        }
    }
    if (_isMovingDown)
    {
        if (mapPosition.y < 0)
        {
            mapPosition.y += 50.0f * dt;
            mapMoved = true;
        }
        else if (heroPosition.y > _hero->getContentSize().height / 2)
        {
            heroPosition.y -= 50.0f * dt;
            heroMoved = true;
        }
    }
    if (_isMovingLeft)
    {
        if (mapPosition.x < 0)
        {
            mapPosition.x += 50.0f * dt;
            mapMoved = true;
        }
        else if (heroPosition.x > _hero->getContentSize().width / 2)
        {
            heroPosition.x -= 50.0f * dt;
            heroMoved = true;
        }
    }
    if (_isMovingRight)
    {
        if (mapPosition.x > visibleSize.width - mapSize.width)
        {
            mapPosition.x -= 50.0f * dt;
            mapMoved = true;
        }
        else if (heroPosition.x < mapSize.width - _hero->getContentSize().width / 2)
        {
            heroPosition.x += 50.0f * dt;
            heroMoved = true;
        }
    }

    // 限制地图位置，防止显示超出地图边界
    mapPosition.x = MIN(mapPosition.x, 0);
    mapPosition.y = MIN(mapPosition.y, 0);
    mapPosition.x = MAX(mapPosition.x, visibleSize.width - mapSize.width);
    mapPosition.y = MAX(mapPosition.y, visibleSize.height - mapSize.height);

    // 更新地图位置或人物位置
    if (mapMoved)
    {
        _map->setPosition(mapPosition);
    }
    else if (heroMoved)
    {
        _hero->setPosition(heroPosition);
    }
}

Vec2 MapScene::tileCoordForPosition(const Vec2& position)
{
    const auto mapSize = _map->getMapSize();
    const auto tileSize = _map->getTileSize();
    int x = position.x / tileSize.width;
    int y = (mapSize.height - position.y / tileSize.height) / tileSize.height;
    return Vec2(x, y);
}