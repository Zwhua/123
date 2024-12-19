#include "MapScene.h"
#include "Hero.h"
#include "Crops.h"
#include "ui/CocosGUI.h"

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
    {;
        return false;
    }
    else
    {
        _map->setAnchorPoint(Vec2(0, 0)); // 设置锚点为 (0, 0)
        _map->setPosition(Vec2(0, 0)); // 设置位置为 (0, 0)
        this->addChild(_map, 0, 0); // 确保地图的 z-order 为 0
    }

    // 创建作物层
    _cropLayer = Node::create();
    _map->addChild(_cropLayer, 1); // 将作物层添加到地图中，确保作物层的 z-order 为 1

    // 创建人物精灵
    _hero = Hero::create();
    if (_hero != nullptr)
    {
        _hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(_hero, 2, 1); // 确保人物的 z-order 为 2
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

    // 添加触摸事件监听器
    addTouchListener();

    // 调度更新方法
    this->schedule(CC_SCHEDULE_SELECTOR(MapScene::updateMapPosition), 0.01f);

    // 创建工具栏
    createToolbar();

    return true;
}

void MapScene::createHero()
{
    // 创建一个精灵作为主人公
    _hero = Hero::create();
    if (_hero == nullptr)
    {
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

void MapScene::addTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        // 获取人物的世界坐标
        Vec2 heroPosition = _hero->getPosition();

        // 将人物的世界坐标转换为地图坐标
        Vec2 mapLocation = _map->convertToNodeSpace(heroPosition);

        // 定义四个不同的偏移量
        Vec2 offsetUp(-24, 20);    // 向上偏移
        Vec2 offsetDown(-24, 5); // 向下偏移
        Vec2 offsetLeft(-32, 13); // 向左偏移
        Vec2 offsetRight(-16, 13); // 向右偏移

        // 根据人物的当前方向选择相应的偏移量
        Vec2 offset;
        switch (_lastDirection)
        {
        case Hero::Direction::UP:
            offset = offsetUp;
            break;
        case Hero::Direction::DOWN:
            offset = offsetDown;
            break;
        case Hero::Direction::LEFT:
            offset = offsetLeft;
            break;
        case Hero::Direction::RIGHT:
            offset = offsetRight;
            break;
        default:
            offset = Vec2(0, 0); // 默认不偏移
            break;
        }

        // 调整作物的位置，使其种植在人物面向的方向
        Vec2 cropPosition = mapLocation + offset;

        // 创建并初始化作物对象
        crop = Crops::create(Crops::CropType::pumpkin);
       // crop->setContentSize(Size(20, -30)); // 设置内容大小
        if (crop != nullptr)
        {
            // 设置作物位置为调整后的位置
            crop->setPosition(cropPosition);
			_cropLayer->addChild(crop);// 将作物添加到作物层
            // 输出作物坐标和人物坐标
 //           CCLOG("Crop created and added to the scene at position (%f, %f)", cropPosition.x, cropPosition.y);
 //           CCLOG("Hero position at (%f, %f)", heroPosition.x, heroPosition.y);
        }

        return true;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void MapScene::updateMapPosition(float dt)
{
    if (_hero == nullptr) {
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
  //      newHeroPosition.y += 50.0f * dt;
  //     if (!_hero->checkCollisionWithNewPosition(newHeroPosition, crop)) {
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
//		else {
//			CCLOG("Collision detected, cannot move up");
//			return;
//		}
//    }
    if (_isMovingDown)
    {
//        newHeroPosition.y -= 50.0f * dt;
//        if (!_hero->checkCollisionWithNewPosition(newHeroPosition,crop)) {
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
 //       }
 //       else {
 //           CCLOG("Collision detected, cannot move down");
 //           return;
 //       }
    }
    if (_isMovingLeft)
    {
  //      newHeroPosition.x -= 50.0f * dt;
  //      if (!_hero->checkCollisionWithNewPosition(newHeroPosition, crop)) {
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
 //       else {
 //           CCLOG("Collision detected, cannot move left");
 //           return;
 //       }
 //   }
    if (_isMovingRight)
    {
  //      newHeroPosition.x += 50.0f * dt;
  //      if (!_hero->checkCollisionWithNewPosition(newHeroPosition, crop)) {
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
  //      }
  //      else {
  //          CCLOG("Collision detected, cannot move right");
  //          return;
  //      }
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

void MapScene::createToolbar()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const float toolbarHeight = 25.0f; // 工具栏高度
    const float toolbarWidth = 25.0f; // 工具栏宽度
    const float spacing = 10.0f; // 工具栏项之间的间距

    for (int i = 0; i < 6; ++i)
    {

        auto toolbarItem = Sprite::create("toolbar.png");
        if (toolbarItem != nullptr)
        {
            toolbarItem->setScale(0.5f); // 调整工具栏项大小
            float xPosition = (visibleSize.width - (6 * toolbarWidth + 5 * spacing)) / 2 + i * (toolbarWidth + spacing);
            float yPosition = toolbarHeight / 2;
            toolbarItem->setPosition(Vec2(xPosition, yPosition));
            this->addChild(toolbarItem, 1); // 确保工具栏项的 z-order 为 1
        }
    }
}