#include "mine.h"
#include "SimpleAudioEngine.h"
#include "Hero.h"
#include "adventure.h"
#include "cocos2d.h"

USING_NS_CC;

Scene* mine::createScene()
{
    auto scene = Scene::create();
    auto layer = mine::create();
    layer->setName("mineLayer");
    scene->addChild(layer);
    return scene;
}

bool mine::init()
{
    if (!Scene::init())
    {
        CCLOG("Error: Failed to initialize Scene");
        return false;
    }

    // ���ؽ�Ǯ����
    loadMoney();

    // �����ڿ���
    loadMiningSkill();

    // ��ȡ�������ļ�·��
    std::string mapPath3 = FileUtils::getInstance()->fullPathForFilename("mine.tmx");
    if (mapPath3.empty())
    {
        CCLOG("Error: TMX file not found: mine.tmx");
        return false;
    }

    // ���� Tiled ��ͼ
    map3 = TMXTiledMap::create(mapPath3);
    if (map3 == nullptr)
    {
        CCLOG("Error while loading: %s", mapPath3.c_str());
        return false;
    }
    else
    {
        CCLOG("Map loaded: %s", mapPath3.c_str());
        map3->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��Ϊ����
        // ���õ�ͼλ��Ϊ��Ļ����
        map3->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
            Director::getInstance()->getVisibleSize().height / 2));

        // �Ŵ��ͼ
        float scaleFactor = 3.0f;
        map3->setScale(scaleFactor);
        this->addChild(map3, 0, 0);
    }

    // ��ӽ�ɫ
    _hero = Hero::create();
    _hero->setPosition(Vec2(100, 100));
    _hero->setScale(1.5f);
    this->addChild(_hero, 1, 1);

    // ��ʼ���ƶ���־
    _isMovingUp = false;
    _isMovingDown = false;
    _isMovingLeft = false;
    _isMovingRight = false;

    // ����ʯͷ����
    createRocks();

    // ��Ӽ����¼�������
    addKeyboardListener();

    // �������¼�������
    addMouseListener();

    // ��ʼ���ڿ��ܱ�ǩ
    _miningSkillLabel = Label::createWithTTF("Mining Skill: " + std::to_string(_miningSkill), "fonts/Marker Felt.ttf", 32);
    _miningSkillLabel->setPosition(Vec2(450, 60)); // ����λ�������½�
    _miningSkillLabel->setAnchorPoint(Vec2(0, 0.5));
    this->addChild(_miningSkillLabel, 1);

    // ���ȸ��·���
    this->schedule(CC_SCHEDULE_SELECTOR(mine::update), 0.016f); // Լ 60 FPS

    return true;
}

void mine::createRocks()
{
    // ��������Ӷ��ʯͷ����
    for (int i = 0; i < 10; ++i)
    {
        auto rock = Sprite::create("stone.png");
        rock->setScale(3.0f); // ����ʯͷ��С

        // �������λ��
        float x = 200 + CCRANDOM_0_1() * (650 - 200);
        float y = 200 + CCRANDOM_0_1() * (650 - 200);
        rock->setPosition(Vec2(x, y));

        this->addChild(rock, 1);
        _rocks.pushBack(rock);
    }
}

void mine::addKeyboardListener()
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

void mine::addMouseListener()
{
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = [this](Event* event) {
        EventMouse* e = dynamic_cast<EventMouse*>(event);
        Vec2 clickPosition = e->getLocationInView();

        // ����Ƿ�����ʯͷ
        for (auto rock : _rocks)
        {
            if (rock->getBoundingBox().containsPoint(clickPosition))
            {
                rock->removeFromParent();
                _rocks.eraseObject(rock);
                _money += 20; // ��Ǯ����20
                _miningSkill += 1; // �ڿ�������1
                saveMoney(); // �����Ǯ����
                saveMiningSkill(); // �����ڿ���
                _miningSkillLabel->setString("Mining Skill: " + std::to_string(_miningSkill)); // ���¼�����ʾ
                break;
            }
        }

        // ����Ƿ�����ָ��λ��
        Vec2 returnPosition = Vec2(150, 100);
        if (clickPosition.distance(returnPosition) < 50.0f) // ������ֵ�����Ը�����Ҫ����
        {
            // �л��� adventure ����
            auto adventureScene = adventure::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(1.0f, adventureScene));
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void mine::update(float dt)
{
    if (_hero == nullptr) {
        CCLOG("Error: Hero is not initialized");
        return;
    }

    Vec2 heroPosition = _hero->getPosition();
    float moveSpeed = 100.0f * dt;

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

    // ���½�ɫλ��
    _hero->setPosition(heroPosition);
}

void mine::saveMoney()
{
    UserDefault::getInstance()->setIntegerForKey("money", _money);
    UserDefault::getInstance()->flush();
}

void mine::loadMoney()
{
    _money = UserDefault::getInstance()->getIntegerForKey("money", 0);
}

void mine::saveMiningSkill()
{
    UserDefault::getInstance()->setIntegerForKey("miningSkill", _miningSkill);
    UserDefault::getInstance()->flush();
}

void mine::loadMiningSkill()
{
    _miningSkill = UserDefault::getInstance()->getIntegerForKey("miningSkill", 0);
}

void mine::increaseMiningSkill(int value)
{
    _miningSkill += value;
    _miningSkillLabel->setString("Mining Skill: " + std::to_string(_miningSkill));
    saveMiningSkill();
}
