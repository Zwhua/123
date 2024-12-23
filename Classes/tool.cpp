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
            Vec2 cropWorldPosition = crop->getParent()->convertToWorldSpace(crop->getPosition());;

            // ����������Hero֮��ľ���
            float distance = heroPosition.distance(cropWorldPosition);

            // ����������ոΧ�ڣ����Ƴ�����
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
    // ��ȡ��Ļ��С
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    // ����һ����͸���ĺ�ɫ�㣬����������Ļ
    auto darkLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    if (darkLayer == nullptr) {
        CCLOG("Failed to create darkLayer");
        return;
    }
    this->addChild(darkLayer, 10);
    CCLOG("darkLayer created successfully");

    // ����Ļ������ʾ "fishing" �ı�
    auto fishingLabel = Label::createWithTTF("Fishing", "fonts/Marker Felt.ttf", 48);
    fishingLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
    darkLayer->addChild(fishingLabel, 11);

    // ���� 2 �����ʾ������
    this->scheduleOnce([this, darkLayer](float dt) {
        darkLayer->removeFromParent(); // �Ƴ���ɫ��
        showFishingResult();
        }, 2.0f, "showFishingResultKey");
}

void Tool::showFishingResult()
{
    // ��ȡ��Ļ��С
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    // ����һ����͸���ĺ�ɫ�㣬����������Ļ
    auto resultLayer = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(resultLayer, 10);
        

        // ���ѡ�������
    int randomValue = rand() % 4; // ����0��3֮��������
    std::string fishImage;
    std::string resultText;
    if (randomValue == 0)
    {
        // ��������
        resultText = "fail to fish...";
        auto noFishLabel = Label::createWithTTF(resultText, "fonts/Marker Felt.ttf", 48);
        noFishLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
        resultLayer->addChild(noFishLabel, 11);
    }
    else
    {
        // ������
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

        // ����Ļ������ʾ�����������Ƭ
        auto fishSprite = Sprite::create(fishImage);
        if (fishSprite)
        {
            // ��ȡ���ͼƬ�ĳߴ�
            auto fishSize = fishSprite->getContentSize();

            // ����ȥ����ΧһȦ��ľ�������
            float borderWidth = 1.0f; // ����Ҫȥ���ı߿���Ϊ1����
            Rect innerRect(borderWidth, borderWidth, fishSize.width - 2 * borderWidth, fishSize.height - 2 * borderWidth);

            // ����������Σ�ֻ��ʾȥ����ΧһȦ��Ĳ���
            fishSprite->setTextureRect(innerRect);

            fishSprite->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
            fishSprite->setScale(12.0f); // �����ͼƬ�Ŵ�12��
            resultLayer->addChild(fishSprite, 11);
        }

        // ��ʾ "successful!" �ı�
        auto successLabel = Label::createWithTTF(resultText, "fonts/Marker Felt.ttf", 48);
        successLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2 - 100));
        resultLayer->addChild(successLabel, 11);
    }

    // ���� 3 ����Ƴ������
    this->scheduleOnce([resultLayer](float dt) {
        resultLayer->removeFromParent();
        }, 3.0f, "removeResultLayerKey");
}