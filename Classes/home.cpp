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
    // ����һ��������Ϊ���˹�
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

    // ���� home.png ����������
    auto sprite = Sprite::create(imagePath);
    if (sprite == nullptr) {
        CCLOG("Error while loading: %s", imagePath.c_str());
        return false;
    }

    // ��ȡ��Ļ��С����������������Ļ����
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    sprite->setPosition(Vec2(origin.x + visibleSize.width / 2,
        origin.y + visibleSize.height / 2));

    // ���ñ��������ű���
    sprite->setScale(3.2); // ��ͼƬ�Ŵ�

    // ��������ӵ�������
    this->addChild(sprite, 0);

    // �������ﾫ��
    _hero = Hero::create();
    if (_hero != nullptr)
    {
        _hero->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��Ϊ����
        _hero->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        _hero->setScale(2.0); // ������Ŵ�����
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

    // ���������¼�������
    addMouseListener();

    // ���ȸ��·���
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

    // �����λ���Ƿ����ض�������
    Rect sleepArea = Rect(800, 580, 120, 120); // ��������Ϊ (800, 580) �� (920, 700)
    if (sleepArea.containsPoint(clickPosition))
    {
        // ����˯���¼�
        sleep();
    }
};
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void HomeScene::sleep()
{
    // ����һ����ɫ�Ĳ㸲����Ļ
    auto blackLayer = LayerColor::create(Color4B(0, 0, 0, 255));
    this->addChild(blackLayer, 3);

    // ����һ����ǩ��ʾ "SLEEP..."
    auto sleepLabel = Label::createWithTTF("SLEEP...", "fonts/Marker Felt.ttf", 38);
    sleepLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
    blackLayer->addChild(sleepLabel);

    // �ӳ� 5 ����Ƴ���ɫ��ͱ�ǩ
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

    // ���µ�ͼλ�û�����λ��
    _hero->setPosition(heroPosition);
}
