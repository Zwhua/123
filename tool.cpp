#include "Tool.h"

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
            Vec2 cropPosition = crop->getPosition();

            // 计算作物与Hero之间的距离
            float distance = heroPosition.distance(cropPosition);

            // 如果距离在收割范围内，则移除作物
            if (distance <= harvestRadius)
            {
                crop->removeFromParent();
                CCLOG("Crop at position (%f, %f) harvested", cropPosition.x, cropPosition.y);
            }
        }
    }
}
