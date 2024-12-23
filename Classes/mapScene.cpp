#include "MapScene.h"
#include "Hero.h"
#include "Crops.h"
#include "Toolbar.h"
#include "Tool.h"
#include "Home.h"
#include "ui/CocosGUI.h"
#include "Weather.h"
#include "BackpackSystem.h"
#include "Animal.h"
#include "AnimalBackpack.h"
#include "cocos2d.h"
#include "town.h"
#include "SimpleAudioEngine.h"

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
    {
        return false;
    }
    else
	{
		CCLOG("Map loaded successfully");
        _map->setAnchorPoint(Vec2(0, 0)); // 设置锚点为 (0, 0)
        _map->setPosition(Vec2(0, 0)); // 设置位置为 (0, 0)
        this->addChild(_map, 0, 0); // 确保地图的 z-order 为 0
    }

    // 创建作物层
    _cropLayer = Node::create();
    if (_cropLayer != nullptr) {
        _cropLayer->setAnchorPoint(Vec2(0, 0)); // 设置锚点为 (0, 0)

        _cropLayer->setPosition(Vec2(0, 0)); // 设置位置为 (0, 0)
        _map->addChild(_cropLayer, 1); // 将作物层添加到地图中，确保作物层的 z-order 为 1
    }
	else
	{
		CCLOG("Error: Failed to create crop layer");
	}

    // 创建动物层
    _animalLayer = Node::create();
    if (_animalLayer != nullptr) {
        _animalLayer->setAnchorPoint(Vec2(0, 0)); // 设置锚点为 (0, 0)
        _animalLayer->setPosition(Vec2(0, 0)); // 设置位置为 (0, 0)
        _map->addChild(_animalLayer, 1); // 将动物层添加到地图中，确保动物层的 z-order 为 1
    }
    else
    {
        CCLOG("Error: Failed to create animal layer");
    }

    // 加载地图状态
    loadMapState();

    // 创建人物精灵
    _hero = Hero::create();
    if (_hero != nullptr)
    {
        _hero->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点为中心
        _hero->setPosition(Vec2(615, 685));
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

    // 添加触摸事件监听器
    addTouchListener();

    // 调度更新方法
    this->schedule(CC_SCHEDULE_SELECTOR(MapScene::updateMapPosition), 0.01f);

    // 创建天气对象并添加到场景中
    _weather = Weather::create();
    if (_weather != nullptr)
    {
        this->addChild(_weather, 3); // 确保天气图标的 z-order 为 3
    }

    // 设置初始天气
    _weather->randomDayWeather();

    // 调度切换到夜晚的方法
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(MapScene::switchToNight), 10.0f); // 5分钟后切换到夜晚

    _currentCropType = Crops::CropType::pumpkin; // 初始化为某种作物类型

    // 在 MapScene::init() 方法中初始化 _backpackSystem
    _backpackSystem = BackpackSystem::create();
    if (_backpackSystem != nullptr) {
        this->addChild(_backpackSystem, 4); // 确保背包系统的 z-order 为 4
        _backpackSystem->setSelectCropCallback([this](const std::string& cropImage, Crops::CropType cropType) {
            CCLOG("Selected crop image: %s", cropImage.c_str());
            // 更新工具框中的图片
            auto toolbarItem = dynamic_cast<Sprite*>(_toolbar->getChildren().at(0));
            if (toolbarItem) {
                toolbarItem->removeAllChildren();
                auto cropItem = Sprite::create(cropImage);
                if (cropItem) {
                    cropItem->setScale(5.0f);
                    cropItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                    toolbarItem->addChild(cropItem, 4);
                }
            }
            // 更新当前作物类型
            _currentCropType = cropType;
            });
    }

    // 在 MapScene::init() 方法中初始化 _animalBackpackSystem
    _animalBackpackSystem = AnimalBackpack::create();
    if (_animalBackpackSystem != nullptr) {
        this->addChild(_animalBackpackSystem, 4); // 确保背包系统的 z-order 为 4
        _animalBackpackSystem->setSelectAnimalCallback([this](const std::string& animalImage, Animal::AnimalType animalType) {
            CCLOG("Selected animal image: %s", animalImage.c_str());
            // 更新工具框中的图片
            auto toolbarItem = dynamic_cast<Sprite*>(_toolbar->getChildren().at(3));
            if (toolbarItem) {
                toolbarItem->removeAllChildren();
                auto animalItem = Sprite::create(animalImage);
                if (animalItem) {
                    animalItem->setScale(5.0f);
                    animalItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                    toolbarItem->addChild(animalItem, 4);
                }
            }
            // 更新当前动物类型
            _currentAnimalType = animalType;
            });
    }

    // 创建工具栏并初始化成员变量
    _toolbar = dynamic_cast<Toolbar*>(Toolbar::create());
    if (_toolbar != nullptr) {
        this->addChild(_toolbar, 3); // 确保工具栏的 z-order 为 3
        _toolbar->setUpdateCropTypeCallback([this](Crops::CropType cropType) {
            this->updateCurrentCropType(cropType);
            });
    }
    else {
        CCLOG("Error: Failed to create Toolbar");
        return false;
    }

    // 播放背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Cloud Country.mp3", true);

    return true;
}

void MapScene::updateCurrentCropType(Crops::CropType cropType) {
    _currentCropType = cropType;
}

// 添加更新当前动物类型的回调函数
void MapScene::updateCurrentAnimalType(Animal::AnimalType animalType) {
    _currentAnimalType = animalType;
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

void MapScene::toggleBackpackLayer()
{
    if (_isBackpackOpen)
    {
        _backpackSystem->hideBackpackLayer();
        _isBackpackOpen = false;
    }
    else
    {
        _backpackSystem->showBackpackLayer(this);
        _isBackpackOpen = true;
    }
}

void MapScene::toggleAnimalBackpackLayer()
{
    if (_isAnimalBackpackOpen)
    {
        _animalBackpackSystem->hideBackpackLayer();
        _isAnimalBackpackOpen = false;
    }
    else
    {
        _animalBackpackSystem->showBackpackLayer(this);
        _isAnimalBackpackOpen = true;
    }
}


void MapScene::addTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event) {
        // 获取触摸点位置
        Vec2 touchLocation = touch->getLocation();
        CCLOG("Touch location: (%f, %f)", touchLocation.x, touchLocation.y);

        // 定义跳转到 townScene 的矩形区域
        Rect townArea = Rect(900, 500, 200, 200); // 你可以根据需要调整这个矩形区域

        if (townArea.containsPoint(touchLocation))
        {
            // 切换到 townScene
            auto townScene = townScene::createScene(false, false);
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, townScene));
            return true; // 确保点击跳转区域时立即返回
        }

        Rect homeArea = Rect(540.000488, 585.000244, 115.999390, 145.000549); // 回家的区域为 (547.000488, 642.000244) 到 (653.999878, 766.000061)
        if (homeArea.containsPoint(touchLocation))
        {
            // 保存地图状态
            saveMapState();

            // 切换到 HomeScene
            auto homeScene = HomeScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, homeScene));
            return true; // 确保点击回家区域时立即返回
        }

        // 如果背包层打开，则不进行操作
        if (_backpackSystem->isBackpackOpen()) {
            log("Backpack is open, ignoring touch");
            return false;
        }

        // 如果动物背包层打开，则不进行操作
        if (_animalBackpackSystem->isBackpackOpen()) {
            log("Animal backpack is open, ignoring touch");
            return false;
        }

        // 检查触摸点是否在工具框范围内
        bool isTouchOnToolbar = false;
        for (const auto& child : _toolbar->getChildren())
        {
            auto toolbarItem = dynamic_cast<Sprite*>(child);
            if (toolbarItem)
            {
                Rect itemRect = toolbarItem->getBoundingBox();
                if (itemRect.containsPoint(touchLocation))
                {
                    isTouchOnToolbar = true;
                    break;
                }
            }
        }

        // 如果触摸点在工具框范围内，则不进行操作
        if (isTouchOnToolbar)
        {
            return false;
        }

        // 定义允许种菜的区域
        Rect plantingArea = Rect(100, 20, 575, 400); // 允许种菜的区域

        // 获取人物的世界坐标
        Vec2 heroPosition = _hero->getPosition();

        // 检查人物是否在允许种菜的区域内
        bool isInPlantingArea = plantingArea.containsPoint(heroPosition);

        // 根据选中的工具框执行相应的操作
        int selectedTool = _toolbar->getSelectedTool();
        if (selectedTool == 0)
        {
            if (isInPlantingArea)
            {
                CCLOG("Planting area");
                // 将人物的世界坐标转换为地图坐标
                Vec2 mapLocation = _map->convertToNodeSpace(heroPosition);

                // 定义四个不同的偏移量
                Vec2 offsetUp(-15, 5);    // 向上偏移
                Vec2 offsetDown(-15, -24); // 向下偏移
                Vec2 offsetLeft(-32, -10); // 向左偏移
                Vec2 offsetRight(0, -10); // 向右偏移

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
                crop = Crops::create(_currentCropType);

                if (crop != nullptr)
                {
                    crop->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点为中心
                    // 设置作物位置为调整后的位置
                    crop->setPosition(cropPosition);
                    _cropLayer->addChild(crop);// 将作物添加到作物层
                    // 输出作物坐标和人物坐标
                    CCLOG("Crop created and added to the scene at position (%f, %f)", cropPosition.x, cropPosition.y);
                    //CCLOG("Hero position at (%f, %f)", heroPosition.x, heroPosition.y);
                }
            }
            else
            {
                CCLOG("Not in planting area");
            }
        }
        else if (selectedTool == 1)
        {
            // 创建工具对象
            Tool* tool1 = Tool::create();
            if (tool1 != nullptr)
            {
                tool1->harvestCropsAroundHero(_hero, _cropLayer);//处理收割事件
            }
        }
        else if (selectedTool == 2)
        {
            cocos2d::Rect fishingArea = cocos2d::Rect(114.000488, 375.000244, 400.999390, 325.000549); // 钓鱼区域
            // 检查点击位置是否在钓鱼区域内
            if (fishingArea.containsPoint(touchLocation))
            {
                // 创建工具对象
                Tool* tool2 = Tool::create();
                if (tool2 != nullptr)
                {
                    this->addChild(tool2); // 将工具添加到场景中
                    CCLOG("fishing tool ready");
                    tool2->handleFishingEvent();
                }
            }
        }
        else if (selectedTool == 3)
        {
            // 获取人物的世界坐标
            Vec2 heroPosition = _hero->getPosition();

            // 将人物的世界坐标转换为地图坐标
            Vec2 mapLocation = _map->convertToNodeSpace(heroPosition);

            // 定义四个不同的偏移量
            Vec2 offsetUp(-15, 5);    // 向上偏移
            Vec2 offsetDown(-15, -24); // 向下偏移
            Vec2 offsetLeft(-32, -10); // 向左偏移
            Vec2 offsetRight(0, -10); // 向右偏移

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

            // 调整动物的位置，使其出现在人物面向的方向
            Vec2 animalPosition = mapLocation + offset;

            // 创建并初始化动物对象
            auto animal = Animal::create(_currentAnimalType);

            if (animal != nullptr)
            {
                animal->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点为中心
                // 设置动物位置为调整后的位置
                animal->setPosition(animalPosition);
                _cropLayer->addChild(animal); // 将动物添加到作物层
                // 输出动物坐标和人物坐标
                CCLOG("Animal created and added to the scene at position (%f, %f)", animalPosition.x, animalPosition.y);
            }
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
            if (mapPosition.y > visibleSize.height - mapSize.height)
            {
                mapPosition.y -= 100.0f * dt;
                mapMoved = true;
            }
            else if (heroPosition.y < mapSize.height - _hero->getContentSize().height / 2)
            {
                heroPosition.y += 100.0f * dt;
                heroMoved = true;
            }
        }

    if (_isMovingDown)
    {
            if (mapPosition.y < 0)
            {
                mapPosition.y += 100.0f * dt;
                mapMoved = true;
            }
            else if (heroPosition.y > _hero->getContentSize().height / 2)
            {
                heroPosition.y -= 100.0f * dt;
                heroMoved = true;
            }    }
    if (_isMovingLeft)
    {

            if (mapPosition.x < 0)
            {
                mapPosition.x += 100.0f * dt;
                mapMoved = true;
            }
            else if (heroPosition.x > _hero->getContentSize().width / 2)
            {
                heroPosition.x -= 100.0f * dt;
                heroMoved = true;
            }
        }

    if (_isMovingRight)
    {
            if (mapPosition.x > visibleSize.width - mapSize.width)
            {
                mapPosition.x -= 100.0f * dt;
                mapMoved = true;
            }
            else if (heroPosition.x < mapSize.width - _hero->getContentSize().width / 2)
            {
                heroPosition.x += 100.0f * dt;
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

void MapScene::switchToNight(float dt)
{
    if (_weather != nullptr)
    {
        _weather->setNightWeather();
    }

    // 创建一个半透明的黑色层，覆盖整个屏幕
    auto nightLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    if (nightLayer == nullptr) {
        CCLOG("Failed to create nightLayer");
        return;
    }
    this->addChild(nightLayer, 5); // 确保夜晚层的 z-order 为 5

    // 在屏幕中央显示 "time to sleep" 文本
    auto sleepLabel = Label::createWithTTF("time to sleep", "fonts/Marker Felt.ttf", 48);
    sleepLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));
    nightLayer->addChild(sleepLabel, 6); // 确保文本的 z-order 比夜晚层高

    // 调度切换到白天的方法
    this->scheduleOnce([this, nightLayer](float dt) {
        nightLayer->removeFromParent(); // 移除夜晚层
        switchToDay(dt);
        }, 5.0f, "switchToDayKey"); // 20秒后切换到白天
}

void MapScene::switchToDay(float dt)
{
    if (_weather != nullptr)
    {
        _weather->stopRainAnimation(); // 停止下雨动画
        _weather->randomDayWeather();
        if (_weather->getWeather() == Weather::WeatherType::RAINY)
        {
            _weather->createRainAnimation();
            _weather->createRainDropAnimation(Vec2(0, 0));
        }
    }
    // 调度切换到夜晚的方法
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(MapScene::switchToNight), 5.0f); // 120秒后切换到夜晚
}

void MapScene::saveMapState()
{
    // 创建一个 ValueMap 来保存地图状态
    ValueMap mapState;

    // 保存作物状态
    ValueVector cropsState;
    for (const auto& child : _cropLayer->getChildren())
    {
        auto crop = dynamic_cast<Crops*>(child);
        if (crop)
        {
            ValueMap cropState;
            cropState["type"] = static_cast<int>(crop->getType());
            cropState["x"] = crop->getPositionX();
            cropState["y"] = crop->getPositionY();
            cropsState.push_back(Value(cropState));
        }
    }
    mapState["crops"] = cropsState;

    // 保存动物状态
    ValueVector animalsState;
    for (const auto& child : _animalLayer->getChildren())
    {
        auto animal = dynamic_cast<Animal*>(child);
        if (animal)
        {
            ValueMap animalState;
            animalState["type"] = static_cast<int>(animal->getType());
            animalState["x"] = animal->getPositionX();
            animalState["y"] = animal->getPositionY();
            animalsState.push_back(Value(animalState));
        }
    }
    mapState["animals"] = animalsState;

    // 将地图状态保存到文件
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "mapState.plist";
    if (FileUtils::getInstance()->writeToFile(mapState, filePath))
    {
        CCLOG("Map state saved successfully to %s", filePath.c_str());
    }
    else
    {
        CCLOG("Failed to save map state to %s", filePath.c_str());
    }
}

void MapScene::loadMapState()
{
    // 获取文件路径
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "mapState.plist";

    // 从文件中读取地图状态
    ValueMap mapState = FileUtils::getInstance()->getValueMapFromFile(filePath);

    // 加载作物状态
    if (mapState.find("crops") != mapState.end() && mapState["crops"].getType() == Value::Type::VECTOR)
    {
        ValueVector cropsState = mapState["crops"].asValueVector();
        for (const auto& cropValue : cropsState)
        {
            if (cropValue.getType() == Value::Type::MAP)
            {
                ValueMap cropState = cropValue.asValueMap();
                auto crop = Crops::create(static_cast<Crops::CropType>(cropState["type"].asInt()));
                if (crop)
                {
                    crop->setPosition(Vec2(cropState["x"].asFloat(), cropState["y"].asFloat()));
                    _cropLayer->addChild(crop);
                }
            }
        }
    }

    // 加载动物状态
    if (mapState.find("animals") != mapState.end() && mapState["animals"].getType() == Value::Type::VECTOR)
    {
        ValueVector animalsState = mapState["animals"].asValueVector();
        for (const auto& animalValue : animalsState)
        {
            if (animalValue.getType() == Value::Type::MAP)
            {
                ValueMap animalState = animalValue.asValueMap();
                auto animal = Animal::create(static_cast<Animal::AnimalType>(animalState["type"].asInt()));
                if (animal)
                {
                    animal->setPosition(Vec2(animalState["x"].asFloat(), animalState["y"].asFloat()));
                    _animalLayer->addChild(animal);
                }
            }
        }
    }
}