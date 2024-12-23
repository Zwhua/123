#include "Tool.h"
#include "Hero.h"


USING_NS_CC;

bool Tool::init()
{
    if (!Node::init())
    {
        return false;
    }

    return true;
}
Tool* Tool::create()
{
    Tool* pRet = new(std::nothrow) Tool();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

void Tool::harvestCropsAroundHero(Hero* hero, cocos2d::Node* cropLayer)
{

    if (hero == nullptr || cropLayer == nullptr)
    {
        return;
    }

    // 获取Hero的世界坐标
    Vec2 heroPosition = hero->getPosition();

    // 定义收割范围的半径
    float harvestRadius = 50.0f;

    // 使用索引遍历cropLayer上的所有子节点
    auto& children = cropLayer->getChildren();
    for (ssize_t i = children.size() - 1; i >= 0; --i)
    {
        auto crop = dynamic_cast<Crops*>(children.at(i));
        if (crop)
        {
            // 获取作物的世界坐标
            Vec2 cropWorldPosition = crop->getParent()->convertToWorldSpace(crop->getPosition());;

            // 计算作物与Hero之间的距离
            float distance = heroPosition.distance(cropWorldPosition);

            // 如果距离在收割范围内，则移除作物
            if (distance <= harvestRadius)
            {
                crop->removeFromParent();
                CCLOG("Crop at position (%f, %f) harvested", cropWorldPosition.x, cropWorldPosition.y);
            }
        }
    }
}

void Tool::handleFishingEvent()
{
    CCLOG("start fishing");
    // 获取屏幕大小
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建一个半透明的黑色层，覆盖整个屏幕
    auto darkLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    if (darkLayer == nullptr) {
        CCLOG("Failed to create darkLayer");
        return;
    }
    this->addChild(darkLayer, 10);
    CCLOG("darkLayer created successfully");

    // 在屏幕中央显示 "fishing" 文本
    auto fishingLabel = Label::createWithTTF("Fishing", "fonts/Marker Felt.ttf", 48);
    fishingLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    darkLayer->addChild(fishingLabel, 11);

    // 调度 2 秒后显示钓鱼结果
    this->scheduleOnce([this, darkLayer](float dt) {
        darkLayer->removeFromParent(); // 移除黑色层
        showFishingResult();
        }, 2.0f, "showFishingResultKey");
}

void Tool::showFishingResult()
{
    // 获取屏幕大小
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建一个半透明的黑色层，覆盖整个屏幕
    auto resultLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(resultLayer, 10);
        

        // 随机选择钓鱼结果
    int randomValue = rand() % 4; // 生成0到3之间的随机数
    std::string fishImage;
    std::string resultText;
    if (randomValue == 0)
    {
        // 钓不到鱼
        resultText = "fail to fish...";
        auto noFishLabel = Label::createWithTTF(resultText, "fonts/Marker Felt.ttf", 48);
        noFishLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        resultLayer->addChild(noFishLabel, 11);
    }
    else
    {
        // 钓到鱼
        resultText = "successful!";
        switch (randomValue)
        {
        case 1:
            fishImage = "fish1.png";
            break;
        case 2:
            fishImage = "fish2.png";
            break;
        case 3:
            fishImage = "fish3.png";
            break;
        }

        // 在屏幕中央显示钓到的鱼的照片
        auto fishSprite = Sprite::create(fishImage);
        if (fishSprite)
        {
            // 获取鱼的图片的尺寸
            auto fishSize = fishSprite->getContentSize();

            // 计算去掉周围一圈后的矩形区域
            float borderWidth = 1.0f; // 假设要去掉的边框宽度为1像素
            Rect innerRect(borderWidth, borderWidth, fishSize.width - 2 * borderWidth, fishSize.height - 2 * borderWidth);

            // 设置纹理矩形，只显示去掉周围一圈后的部分
            fishSprite->setTextureRect(innerRect);

            fishSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            fishSprite->setScale(12.0f); // 将鱼的图片放大12倍
            resultLayer->addChild(fishSprite, 11);
        }

        // 显示 "successful!" 文本
        auto successLabel = Label::createWithTTF(resultText, "fonts/Marker Felt.ttf", 48);
        successLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
        resultLayer->addChild(successLabel, 11);
    }

    // 调度 3 秒后移除结果层
    this->scheduleOnce([resultLayer](float dt) {
        resultLayer->removeFromParent();
        }, 3.0f, "removeResultLayerKey");
}