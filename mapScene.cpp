#include "MapScene.h"
#include "Hero.h"
#include "Crops.h"
#include "Toolbar.h"
#include "Tool.h"
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

    // �������ﾫ��
    _hero = Hero::create();
    if (_hero != nullptr)
    {
        _hero->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��Ϊ����
        _hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
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

    // ��������������ʼ����Ա����
    _toolbar = dynamic_cast<Toolbar*>(Toolbar::create());
    if (_toolbar != nullptr)
    {
        this->addChild(_toolbar, 3); // ȷ���������� z-order Ϊ 3
    }
    else
    {
        CCLOG("Error: Failed to create Toolbar");
        return false;
    }


    return true;
}

void MapScene::createHero()
{
    // ����һ��������Ϊ���˹�
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
        // ��ȡ������λ��
        Vec2 touchLocation = touch->getLocation();

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
            log("Touch on toolbar");
            return false;
        }

        // ����ѡ�еĹ��߿�ִ����Ӧ�Ĳ���
        int selectedTool = _toolbar->getSelectedTool();
        if (selectedTool == 0)
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

            // ���������λ�ã�ʹ����ֲ����������ķ���
            Vec2 cropPosition = mapLocation + offset;

            // ��������ʼ���������
            crop = Crops::create(Crops::CropType::pumpkin);

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
		else if (selectedTool == 1)
		{
			// �������߶���
			Tool* tool = Tool::create();
			if (tool != nullptr)
			{
				// �ո�������Χ������
				tool->harvestCropsAroundHero(_hero, _cropLayer);
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