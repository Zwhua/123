#include "town.h"
#include "SimpleAudioEngine.h"
#include "Hero.h"
#include "people.h"
#include "talk.h"
#include "adventure.h"
#include "store.h"
#include "mapscene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// 场景的初始化
Scene* townScene::createScene(bool fromAdventure, bool fromStore)
{
    auto scene = Scene::create();
    auto layer = townScene::create(fromAdventure, fromStore);
    scene->addChild(layer);
    return scene;
}

bool townScene::init(bool fromAdventure, bool fromStore)
{
    if (!Scene::init())
    {
        CCLOG("Error: Failed to initialize Scene");
        return false;
    }

    // 获取窗口的大小
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 获取完整的文件路径
    std::string mapPath1 = FileUtils::getInstance()->fullPathForFilename("town.tmx");
    if (mapPath1.empty())
    {
        CCLOG("Error: TMX file not found: town.tmx");
        return false;
    }

    // 加载 Tiled 地图
    map1 = TMXTiledMap::create(mapPath1);
    if (map1 == nullptr)
    {
        CCLOG("Error while loading: %s", mapPath1.c_str());
        return false;
    }
    else
    {
        CCLOG("Map loaded: %s", mapPath1.c_str());
        map1->setAnchorPoint(Vec2(0, 0)); // 设置锚点为左下角
        map1->setPosition(Vec2(0, 0)); // 设置地图的位置

        // 放大视图
        float scaleFactor = 1.3f; // 设置缩放因子，例如放大2倍
        map1->setScale(scaleFactor);
        this->addChild(map1, 0, 0); // 确保地图的 z-order 为 0
    }

    // 创建人物精灵
    createHero(fromAdventure, fromStore);

    // 创建一个单独的层来添加 Alex 精灵
    auto Layer = Node::create();
    auto Alex = Sprite::create("Alex.png");
    Alex->setScale(0.7);
    auto Bear = Sprite::create("Bear.png");
    Bear->setScale(0.5);
    auto Baby = Sprite::create("Baby.png");
    Baby->setScale(0.8);
    auto Emily = Sprite::create("Emily.png");
    Emily->setScale(0.7);
    auto George = Sprite::create("George.png");
    George->setScale(1.15);
    auto Gunther = Sprite::create("Gunther.png");
    Gunther->setScale(1.1);
    auto Jodi = Sprite::create("Jodi.png");
    Jodi->setScale(1.2);
    auto Linus = Sprite::create("Linus.png");
    auto Sam = Sprite::create("Sam.png");

    //设置每个精灵的位置
    Alex->setPosition(Vec2(735, 513));
    Bear->setPosition(Vec2(418, 410));
    Baby->setPosition(Vec2(210, 300));
    Emily->setPosition(Vec2(80, 35));
    George->setPosition(Vec2(470, 90));
    Gunther->setPosition(Vec2(595, 630));
    Jodi->setPosition(Vec2(180, 300));
    Linus->setPosition(Vec2(693, 147));
    Sam->setPosition(Vec2(535, 480));

    //将每个精灵添加到层中
    Layer->addChild(Alex);
    Layer->addChild(Bear);
    Layer->addChild(Baby);
    Layer->addChild(Emily);
    Layer->addChild(George);
    Layer->addChild(Gunther);
    Layer->addChild(Jodi);
    Layer->addChild(Linus);
    Layer->addChild(Sam);
    map1->addChild(Layer, 2);

    // 添加 Jodi 的鼠标点击事件监听器
    auto jodiListener = EventListenerMouse::create();
    jodiListener->onMouseDown = [this, Jodi, visibleSize](Event* event)
        {
            //点击出现坐标
            EventMouse* e = dynamic_cast<EventMouse*>(event);

            Vec2 heroPosition = _hero->getPosition();
            Vec2 jodiPosition = Vec2(237, 370);
            float distance = heroPosition.getDistance(jodiPosition);

            // 检查主人公是否在 Jodi 附近
            if (distance <= 40.0f) // 距离阈值，可以根据需要调整
            {
                // 显示自定义对话框
                std::vector<std::string> messages = {
                    "Hello! I am Jodi.",
                    "Welcome to our village.",
                    "Feel free to explore around."
                };

                auto dialog = Dialog::create();
                dialog->setTitle("Jodi", 40);
                dialog->setContent(messages, 32);
                dialog->addDeleteButton("buy_normal.png", "buy_selected.png", Vec2(0, 30));
                dialog->addButton("buy_normal.png", "buy_selected.png", 1, Vec2(100, 30));
                dialog->addConfessButton("buy_normal.png", "buy_selected.png", Vec2(200, 30));
                dialog->addTaskButton("buy_normal.png", "buy_selected.png", Vec2(300, 30));
                dialog->setCallbackFunc(this, callfuncN_selector(townScene::dialogCallback));
                dialog->setTag(10); // 设置标签
                this->addChild(dialog, 10);
            }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(jodiListener, map1);

    // 添加鼠标点击事件监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [this, fromAdventure](Event* event)
        {
            EventMouse* e = dynamic_cast<EventMouse*>(event);
            Vec2 heroPosition = _hero->getPosition();
            const auto mapSize = map1->getContentSize();
            const auto visibleSize = Director::getInstance()->getVisibleSize();
            //输出当前位置
			CCLOG("Mouse clicked at: (%f, %f)", e->getLocationInView().x, e->getLocationInView().y);

            // 检查主角是否在地图的最右边正中心
            if (heroPosition.x >= 1000  &&
                heroPosition.y >= (500 && heroPosition.y <= 550))
            {
                // 切换到 adventure 场景
                auto adventureScene = adventure::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, adventureScene));
            }

            // 检查主角是否在商店位置
            Vec2 targetPosition = Vec2(775, 620);
            if (heroPosition.distance(targetPosition) < 50.0f) // 距离阈值，可以根据需要调整
            {
                // 切换到 store 场景
                auto storeScene = Store::createScene(fromAdventure); // 传递 fromAdventure 参数
                Director::getInstance()->replaceScene(TransitionFade::create(1.0f, storeScene));
            }

            //检查主角是否在地图的最左边正中心
			if (heroPosition.x <= 50 &&
				heroPosition.y >= (500 && heroPosition.y <= 550))
			{
				// 切换到 mapscene 场景
				auto mapScene = MapScene::createScene();
				Director::getInstance()->replaceScene(TransitionFade::create(1.0f, mapScene));
			}
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // 初始化上一次移动方向
    _lastDirection = Hero::Direction::DOWN;

    // 添加键盘事件监听器
    addKeyboardListener();

    // 调度更新方法
    this->schedule(CC_SCHEDULE_SELECTOR(townScene::updateMapPosition), 0.001f);

    // 播放背景音乐
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("Mines.mp3", true);

    return true;
}

townScene* townScene::create(bool fromAdventure, bool fromStore)
{
    townScene* pRet = new(std::nothrow) townScene();
    if (pRet && pRet->init(fromAdventure, fromStore))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}

void townScene::createHero(bool fromAdventure, bool fromStore)
{
    // 创建一个精灵作为主人公
    _hero = Hero::create();
    _hero->setScale(1.0f);
    if (_hero == nullptr)
    {
        CCLOG("Error while loading: %s", "Hero");
        return;
    }
    else
    {
        const auto visibleSize = Director::getInstance()->getVisibleSize(); // 获取屏幕的大小
        const auto mapSize = map1->getContentSize();

        if (fromStore)
        {
            // 设置人物位置为商店位置
            _hero->setPosition(Vec2(775, 600));
        }
        else if (fromAdventure)
        {
            // 设置人物位置为地图的正右边
            _hero->setPosition(Vec2(mapSize.width *1.3, 450));
        }
        else
        {
            // 设置人物初始位置
            _hero->setPosition(Vec2(0, 450));
        }

        this->addChild(_hero, 1);
    }
}

void townScene::addKeyboardListener()
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

void townScene::updateMapPosition(float dt)
{
    if (_hero == nullptr) {
        CCLOG("Error: Hero is not initialized");
        return;
    }

    Vec2 heroPosition = _hero->getPosition();
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto mapSize = map1->getContentSize();

    bool heroMoved = false;

    float moveSpeed = 100.0f * dt;

    if (_isMovingUp)
    {
        if (heroPosition.y + moveSpeed <= mapSize.height * map1->getScaleY())
        {
            heroPosition.y += moveSpeed;
            heroMoved = true;
        }
    }
    if (_isMovingDown)
    {
        if (heroPosition.y - moveSpeed >= 0)
        {
            heroPosition.y -= moveSpeed;
            heroMoved = true;
        }
    }
    if (_isMovingLeft)
    {
        if (heroPosition.x - moveSpeed >= 0)
        {
            heroPosition.x -= moveSpeed;
            heroMoved = true;
        }
    }
    if (_isMovingRight)
    {
        if (heroPosition.x + moveSpeed <= mapSize.width * map1->getScaleX())
        {
            heroPosition.x += moveSpeed;
            heroMoved = true;
        }
    }

    // 更新人物位置
    if (heroMoved)
    {
        _hero->setPosition(heroPosition);

        // 计算地图的新位置，使人物始终在视图的中心
        float x = std::max(heroPosition.x, visibleSize.width / 2);
        float y = std::max(heroPosition.y, visibleSize.height / 2);

        // 限制地图的移动范围
        x = std::min(x, (mapSize.width * map1->getScaleX()) - visibleSize.width / 2);
        y = std::min(y, (mapSize.height * map1->getScaleY()) - visibleSize.height / 2);

        // 设置地图的新位置
        Vec2 actualPosition = Vec2(x, y);

        // 计算地图的新位置，使人物始终在视图的中心
        Vec2 centerOfView = Vec2(visibleSize.width / 2, visibleSize.height / 2);
        Vec2 viewPoint = centerOfView - actualPosition;

        // 设置地图的新位置
        map1->setPosition(viewPoint);

        // 检查任务完成情况
        if (heroPosition.distance(Vec2(230, 370)) < 100.0f) {
            auto dialog = dynamic_cast<Dialog*>(this->getChildByTag(10));// 获取对话框
            if (dialog) {
                dialog->setTaskCompleted(true);
                CCLOG("Task completed!");
            }
        }
    }
}

Vec2 townScene::tileCoordForPosition(const Vec2& position)
{
    const auto mapSize = map1->getMapSize();
    const auto tileSize = map1->getTileSize();
    int x = position.x / tileSize.width;
    int y = (mapSize.height - position.y / tileSize.height) / tileSize.height;
    return Vec2(x, y);
}

void townScene::menuCloseCallback(Ref* pSender)
{
    // 关闭当前场景并返回到上一个场景
    Director::getInstance()->popScene();
}

void townScene::dialogCallback(Node* sender)
{
    // 处理对话框按钮点击事件
    CCLOG("Dialog button clicked, tag: %d", sender->getTag());
}
