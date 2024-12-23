#include "store.h"
#include "SimpleAudioEngine.h"
#include "town.h"
#include "mine.h" 

USING_NS_CC;

Scene* Store::createScene(bool fromAdventure)
{
    auto scene = Scene::create();
    auto layer = Store::create(fromAdventure); 
    scene->addChild(layer);
    return scene;
}

bool Store::init(bool fromAdventure)
{
    if (!Layer::init())
    {
        return false;
    }

    _fromAdventure = fromAdventure;

    // 获取可见区域大小
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建商店界面层
    _storeLayer = LayerColor::create(Color4B(0, 0, 0, 180));
    _storeLayer->setContentSize(Size(visibleSize.width / 2, visibleSize.height / 2));
    _storeLayer->setPosition(Vec2(visibleSize.width / 4 + origin.x, visibleSize.height / 4 + origin.y));
    this->addChild(_storeLayer, 1);

    // 添加商店背景图片
    _backgroundSprite = Sprite::create("store.png");
    _backgroundSprite->setScale(3.0f);
    _backgroundSprite->setPosition(Vec2(_storeLayer->getContentSize().width / 2, _storeLayer->getContentSize().height / 2));
    _storeLayer->addChild(_backgroundSprite, 0);

    // 画出商店格子
    drawGrid();

    // 添加商店标题标签
    auto storeTitleLabel = Label::createWithTTF("Store", "fonts/Marker Felt.ttf", 60);
    storeTitleLabel->setColor(Color3B::BLACK);
    storeTitleLabel->setPosition(Vec2(-140, _storeLayer->getContentSize().height));
    _storeLayer->addChild(storeTitleLabel, 1);

    // 添加关闭按钮
    auto closeButton = MenuItemImage::create(
        "exit_normal.png",
        "exit_selected.png",
        CC_CALLBACK_1(Store::closeStore, this));
    closeButton->setPosition(Vec2(_storeLayer->getContentSize().width + 100, _storeLayer->getContentSize().height + 130));

    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    _storeLayer->addChild(menu, 1);

    // 添加显示金钱数量的标签
    _moneyLabel = Label::createWithTTF("Money: " + std::to_string(_money), "fonts/Marker Felt.ttf", 32);
    _moneyLabel->setPosition(Vec2(-200, _storeLayer->getContentSize().height + 150));
    _moneyLabel->setAnchorPoint(Vec2(0, 0.5));
    _storeLayer->addChild(_moneyLabel, 1);

    // 添加显示挖矿经验值的标签
    _miningSkillLabel = Label::createWithTTF("Mining Skill: " + std::to_string(_miningSkill), "fonts/Marker Felt.ttf", 32);
    _miningSkillLabel->setPosition(Vec2(-200, _storeLayer->getContentSize().height + 100));
    _miningSkillLabel->setAnchorPoint(Vec2(0, 0.5));
    _storeLayer->addChild(_miningSkillLabel, 1);

    // 加载金钱数量
    loadMoney();

    // 加载挖矿经验值
    loadMiningSkill();

    // 添加商品图片
    auto item1Sprite = Sprite::create("item1.png");
    item1Sprite->setScale(4.0f);
    item1Sprite->setPosition(Vec2(50, 350));
    _storeLayer->addChild(item1Sprite, 1);

    // 添加购买按钮
    auto buyButton1 = MenuItemImage::create(
        "buy_normal.png",
        "buy_selected.png",
        CC_CALLBACK_1(Store::buyItem, this));
    buyButton1->setScale(1.5f);
    buyButton1->setPosition(Vec2(50, 300));

    // 添加价格标签
    auto priceLabel = Label::createWithTTF("100", "fonts/Marker Felt.ttf", 14);
    priceLabel->setColor(Color3B::BLACK);
    priceLabel->setPosition(Vec2(buyButton1->getContentSize().width / 2, buyButton1->getContentSize().height / 2));
    priceLabel->setAnchorPoint(Vec2(0.5, 0.5));
    buyButton1->addChild(priceLabel);

    auto buyMenu = Menu::create(buyButton1, nullptr);
    buyMenu->setPosition(Vec2::ZERO);
    _storeLayer->addChild(buyMenu, 1);

    return true;
}

void Store::showStore()
{
    _storeLayer->setVisible(true);
}

void Store::closeStore(Ref* sender)
{
    // 关闭商店界面并返回到 town 场景
    auto townScene = townScene::createScene(_fromAdventure, true);
    Director::getInstance()->replaceScene(TransitionFade::create(1.0f, townScene));
}

Store* Store::create(bool fromAdventure) // 修改为带有参数的定义
{
    Store* pRet = new(std::nothrow) Store();
    if (pRet && pRet->init(fromAdventure))
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

void Store::drawGrid()
{
    auto drawNode = DrawNode::create();
    float cellWidth = _storeLayer->getContentSize().width / 5;
    float cellHeight = _storeLayer->getContentSize().height / 5;

    for (int i = 0; i <= 5; ++i)
    {
        // 画垂直线
        drawNode->drawLine(Vec2(i * cellWidth, 0), Vec2(i * cellWidth, _storeLayer->getContentSize().height), Color4F::BLACK);
        // 画水平线
        drawNode->drawLine(Vec2(0, i * cellHeight), Vec2(_storeLayer->getContentSize().width, i * cellHeight), Color4F::BLACK);
    }

    _storeLayer->addChild(drawNode, 1);
}

void Store::updateMoneyLabel()
{
    _moneyLabel->setString("Money: " + std::to_string(_money));
}

void Store::updateMiningSkillLabel()
{
    _miningSkillLabel->setString("Mining Skill: " + std::to_string(_miningSkill));
}

void Store::loadMoney()
{
    _money = UserDefault::getInstance()->getIntegerForKey("money", 0);
    updateMoneyLabel();
}

void Store::saveMoney()
{
    UserDefault::getInstance()->setIntegerForKey("money", _money);
    UserDefault::getInstance()->flush();
}

void Store::loadMiningSkill()
{
    _miningSkill = UserDefault::getInstance()->getIntegerForKey("miningSkill", 0);
    updateMiningSkillLabel();
}

void Store::buyItem(Ref* sender)
{
    if (_money >= 100)
    {
        _money -= 100;
        updateMoneyLabel();
        saveMoney();
        increaseMiningSkill(); // 增加挖矿经验值
        CCLOG("Item purchased!");
    }
    else
    {
        CCLOG("Not enough money to purchase the item.");
    }
}

void Store::increaseMiningSkill()
{
    // 获取当前场景
    auto currentScene = Director::getInstance()->getRunningScene();
    if (currentScene)
    {
        // 获取 mine 场景中的挖矿经验值标签
        auto mineLayer = dynamic_cast<mine*>(currentScene->getChildByName("mineLayer"));
        if (mineLayer)
        {
            mineLayer->increaseMiningSkill(10); // 增加挖矿经验值
            mineLayer->saveMiningSkill(); // 保存挖矿技能
        }
    }
    _miningSkill += 10; // 增加挖矿经验值
    updateMiningSkillLabel(); // 更新挖矿经验值标签
    UserDefault::getInstance()->setIntegerForKey("miningSkill", _miningSkill); // 保存挖矿经验值
    UserDefault::getInstance()->flush();
}
