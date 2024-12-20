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

    // ��ȡHero����������
    Vec2 heroPosition = hero->getPosition();

    // �����ոΧ�İ뾶
    float harvestRadius = 50.0f;

    // ʹ����������cropLayer�ϵ������ӽڵ�
    auto& children = cropLayer->getChildren();
    for (ssize_t i = children.size() - 1; i >= 0; --i)
    {
        auto crop = dynamic_cast<Crops*>(children.at(i));
        if (crop)
        {
            // ��ȡ�������������
            Vec2 cropPosition = crop->getPosition();

            // ����������Hero֮��ľ���
            float distance = heroPosition.distance(cropPosition);

            // ����������ոΧ�ڣ����Ƴ�����
            if (distance <= harvestRadius)
            {
                crop->removeFromParent();
                CCLOG("Crop at position (%f, %f) harvested", cropPosition.x, cropPosition.y);
            }
        }
    }
}
