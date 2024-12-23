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

    // ��ȡ���ڵĴ�С
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    // ��ȡ�������ļ�·��
    std::string mapPath = FileUtils::getInstance()->fullPathForFilename("last.tmx");
    if (mapPath.empty())
    {
        CCLOG("Error: TMX file not found: last.tmx");
        return false;
    }

    // ���� Tiled ��ͼ
    _map = TMXTiledMap::create(mapPath);
    if (_map == nullptr)
    {
        return false;
    }
    else
	{
		CCLOG("Map loaded successfully");
        _map->setAnchorPoint(Vec2(0, 0)); // ����ê��Ϊ (0, 0)
        _map->setPosition(Vec2(0, 0)); // ����λ��Ϊ (0, 0)
        this->addChild(_map, 0, 0); // ȷ����ͼ�� z-order Ϊ 0
    }

    // ���������
    _cropLayer = Node::create();
    if (_cropLayer != nullptr) {
        _cropLayer->setAnchorPoint(Vec2(0, 0)); // ����ê��Ϊ (0, 0)

        _cropLayer->setPosition(Vec2(0, 0)); // ����λ��Ϊ (0, 0)
        _map->addChild(_cropLayer, 1); // ���������ӵ���ͼ�У�ȷ�������� z-order Ϊ 1
    }
	else
	{
		CCLOG("Error: Failed to create crop layer");
	}

    // ���������
    _animalLayer = Node::create();
    if (_animalLayer != nullptr) {
        _animalLayer->setAnchorPoint(Vec2(0, 0)); // ����ê��Ϊ (0, 0)
        _animalLayer->setPosition(Vec2(0, 0)); // ����λ��Ϊ (0, 0)
        _map->addChild(_animalLayer, 1); // ���������ӵ���ͼ�У�ȷ�������� z-order Ϊ 1
    }
    else
    {
        CCLOG("Error: Failed to create animal layer");
    }

    // ���ص�ͼ״̬
    loadMapState();

    // �������ﾫ��
    _hero = Hero::create();
    if (_hero != nullptr)
    {
        _hero->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��Ϊ����
        _hero->setPosition(Vec2(615, 685));
        this->addChild(_hero, 2, 1); // ȷ������� z-order Ϊ 2
    }
    else
    {
        CCLOG("Error: Failed to create Hero");
    }

    // ��ʼ����һ���ƶ�����
    _lastDirection = Hero::Direction::DOWN;

    // ��Ӽ����¼�������
    addKeyboardListener();

    // ��Ӵ����¼�������
    addTouchListener();

    // ���ȸ��·���
    this->schedule(CC_SCHEDULE_SELECTOR(MapScene::updateMapPosition), 0.01f);

    // ��������������ӵ�������
    _weather = Weather::create();
    if (_weather != nullptr)
    {
        this->addChild(_weather, 3); // ȷ������ͼ��� z-order Ϊ 3
    }

    // ���ó�ʼ����
    _weather->randomDayWeather();

    // �����л���ҹ��ķ���
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(MapScene::switchToNight), 10.0f); // 5���Ӻ��л���ҹ��

    _currentCropType = Crops::CropType::pumpkin; // ��ʼ��Ϊĳ����������

    // �� MapScene::init() �����г�ʼ�� _backpackSystem
    _backpackSystem = BackpackSystem::create();
    if (_backpackSystem != nullptr) {
        this->addChild(_backpackSystem, 4); // ȷ������ϵͳ�� z-order Ϊ 4
        _backpackSystem->setSelectCropCallback([this](const std::string& cropImage, Crops::CropType cropType) {
            CCLOG("Selected crop image: %s", cropImage.c_str());
            // ���¹��߿��е�ͼƬ
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
            // ���µ�ǰ��������
            _currentCropType = cropType;
            });
    }

    // �� MapScene::init() �����г�ʼ�� _animalBackpackSystem
    _animalBackpackSystem = AnimalBackpack::create();
    if (_animalBackpackSystem != nullptr) {
        this->addChild(_animalBackpackSystem, 4); // ȷ������ϵͳ�� z-order Ϊ 4
        _animalBackpackSystem->setSelectAnimalCallback([this](const std::string& animalImage, Animal::AnimalType animalType) {
            CCLOG("Selected animal image: %s", animalImage.c_str());
            // ���¹��߿��е�ͼƬ
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
            // ���µ�ǰ��������
            _currentAnimalType = animalType;
            });
    }

    // ��������������ʼ����Ա����
    _toolbar = dynamic_cast<Toolbar*>(Toolbar::create());
    if (_toolbar != nullptr) {
        this->addChild(_toolbar, 3); // ȷ���������� z-order Ϊ 3
        _toolbar->setUpdateCropTypeCallback([this](Crops::CropType cropType) {
            this->updateCurrentCropType(cropType);
            });
    }
    else {
        CCLOG("Error: Failed to create Toolbar");
        return false;
    }

    // ���ű�������
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Cloud Country.mp3", true);

    return true;
}

void MapScene::updateCurrentCropType(Crops::CropType cropType) {
    _currentCropType = cropType;
}

// ��Ӹ��µ�ǰ�������͵Ļص�����
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
        // ��ȡ������λ��
        Vec2 touchLocation = touch->getLocation();
        CCLOG("Touch location: (%f, %f)", touchLocation.x, touchLocation.y);

        // ������ת�� townScene �ľ�������
        Rect townArea = Rect(900, 500, 200, 200); // ����Ը�����Ҫ���������������

        if (townArea.containsPoint(touchLocation))
        {
            // �л��� townScene
            auto townScene = townScene::createScene(false, false);
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, townScene));
            return true; // ȷ�������ת����ʱ��������
        }

        Rect homeArea = Rect(540.000488, 585.000244, 115.999390, 145.000549); // �ؼҵ�����Ϊ (547.000488, 642.000244) �� (653.999878, 766.000061)
        if (homeArea.containsPoint(touchLocation))
        {
            // �����ͼ״̬
            saveMapState();

            // �л��� HomeScene
            auto homeScene = HomeScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, homeScene));
            return true; // ȷ������ؼ�����ʱ��������
        }

        // ���������򿪣��򲻽��в���
        if (_backpackSystem->isBackpackOpen()) {
            log("Backpack is open, ignoring touch");
            return false;
        }

        // ������ﱳ����򿪣��򲻽��в���
        if (_animalBackpackSystem->isBackpackOpen()) {
            log("Animal backpack is open, ignoring touch");
            return false;
        }

        // ��鴥�����Ƿ��ڹ��߿�Χ��
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

        // ����������ڹ��߿�Χ�ڣ��򲻽��в���
        if (isTouchOnToolbar)
        {
            return false;
        }

        // ���������ֲ˵�����
        Rect plantingArea = Rect(100, 20, 575, 400); // �����ֲ˵�����

        // ��ȡ�������������
        Vec2 heroPosition = _hero->getPosition();

        // ��������Ƿ��������ֲ˵�������
        bool isInPlantingArea = plantingArea.containsPoint(heroPosition);

        // ����ѡ�еĹ��߿�ִ����Ӧ�Ĳ���
        int selectedTool = _toolbar->getSelectedTool();
        if (selectedTool == 0)
        {
            if (isInPlantingArea)
            {
                CCLOG("Planting area");
                // ���������������ת��Ϊ��ͼ����
                Vec2 mapLocation = _map->convertToNodeSpace(heroPosition);

                // �����ĸ���ͬ��ƫ����
                Vec2 offsetUp(-15, 5);    // ����ƫ��
                Vec2 offsetDown(-15, -24); // ����ƫ��
                Vec2 offsetLeft(-32, -10); // ����ƫ��
                Vec2 offsetRight(0, -10); // ����ƫ��

                // ��������ĵ�ǰ����ѡ����Ӧ��ƫ����
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
                    offset = Vec2(0, 0); // Ĭ�ϲ�ƫ��
                    break;
                }

                // ���������λ�ã�ʹ����ֲ����������ķ���
                Vec2 cropPosition = mapLocation + offset;

                // ��������ʼ���������
                crop = Crops::create(_currentCropType);

                if (crop != nullptr)
                {
                    crop->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��Ϊ����
                    // ��������λ��Ϊ�������λ��
                    crop->setPosition(cropPosition);
                    _cropLayer->addChild(crop);// ��������ӵ������
                    // ��������������������
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
            // �������߶���
            Tool* tool1 = Tool::create();
            if (tool1 != nullptr)
            {
                tool1->harvestCropsAroundHero(_hero, _cropLayer);//�����ո��¼�
            }
        }
        else if (selectedTool == 2)
        {
            cocos2d::Rect fishingArea = cocos2d::Rect(114.000488, 375.000244, 400.999390, 325.000549); // ��������
            // �����λ���Ƿ��ڵ���������
            if (fishingArea.containsPoint(touchLocation))
            {
                // �������߶���
                Tool* tool2 = Tool::create();
                if (tool2 != nullptr)
                {
                    this->addChild(tool2); // ��������ӵ�������
                    CCLOG("fishing tool ready");
                    tool2->handleFishingEvent();
                }
            }
        }
        else if (selectedTool == 3)
        {
            // ��ȡ�������������
            Vec2 heroPosition = _hero->getPosition();

            // ���������������ת��Ϊ��ͼ����
            Vec2 mapLocation = _map->convertToNodeSpace(heroPosition);

            // �����ĸ���ͬ��ƫ����
            Vec2 offsetUp(-15, 5);    // ����ƫ��
            Vec2 offsetDown(-15, -24); // ����ƫ��
            Vec2 offsetLeft(-32, -10); // ����ƫ��
            Vec2 offsetRight(0, -10); // ����ƫ��

            // ��������ĵ�ǰ����ѡ����Ӧ��ƫ����
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
                offset = Vec2(0, 0); // Ĭ�ϲ�ƫ��
                break;
            }

            // ���������λ�ã�ʹ���������������ķ���
            Vec2 animalPosition = mapLocation + offset;

            // ��������ʼ���������
            auto animal = Animal::create(_currentAnimalType);

            if (animal != nullptr)
            {
                animal->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��Ϊ����
                // ���ö���λ��Ϊ�������λ��
                animal->setPosition(animalPosition);
                _cropLayer->addChild(animal); // ��������ӵ������
                // ��������������������
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

    // ���Ƶ�ͼλ�ã���ֹ��ʾ������ͼ�߽�
    mapPosition.x = MIN(mapPosition.x, 0);
    mapPosition.y = MIN(mapPosition.y, 0);
    mapPosition.x = MAX(mapPosition.x, visibleSize.width - mapSize.width);
    mapPosition.y = MAX(mapPosition.y, visibleSize.height - mapSize.height);

    // ���µ�ͼλ�û�����λ��
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

    // ����һ����͸���ĺ�ɫ�㣬����������Ļ
    auto nightLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    if (nightLayer == nullptr) {
        CCLOG("Failed to create nightLayer");
        return;
    }
    this->addChild(nightLayer, 5); // ȷ��ҹ���� z-order Ϊ 5

    // ����Ļ������ʾ "time to sleep" �ı�
    auto sleepLabel = Label::createWithTTF("time to sleep", "fonts/Marker Felt.ttf", 48);
    sleepLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
        Director::getInstance()->getVisibleSize().height / 2));
    nightLayer->addChild(sleepLabel, 6); // ȷ���ı��� z-order ��ҹ����

    // �����л�������ķ���
    this->scheduleOnce([this, nightLayer](float dt) {
        nightLayer->removeFromParent(); // �Ƴ�ҹ���
        switchToDay(dt);
        }, 5.0f, "switchToDayKey"); // 20����л�������
}

void MapScene::switchToDay(float dt)
{
    if (_weather != nullptr)
    {
        _weather->stopRainAnimation(); // ֹͣ���궯��
        _weather->randomDayWeather();
        if (_weather->getWeather() == Weather::WeatherType::RAINY)
        {
            _weather->createRainAnimation();
            _weather->createRainDropAnimation(Vec2(0, 0));
        }
    }
    // �����л���ҹ��ķ���
    this->scheduleOnce(CC_SCHEDULE_SELECTOR(MapScene::switchToNight), 5.0f); // 120����л���ҹ��
}

void MapScene::saveMapState()
{
    // ����һ�� ValueMap �������ͼ״̬
    ValueMap mapState;

    // ��������״̬
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

    // ���涯��״̬
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

    // ����ͼ״̬���浽�ļ�
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
    // ��ȡ�ļ�·��
    std::string filePath = FileUtils::getInstance()->getWritablePath() + "mapState.plist";

    // ���ļ��ж�ȡ��ͼ״̬
    ValueMap mapState = FileUtils::getInstance()->getValueMapFromFile(filePath);

    // ��������״̬
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

    // ���ض���״̬
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