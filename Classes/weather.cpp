#include "weather.h"

USING_NS_CC;

Weather* Weather::create()
{
    Weather* pRet = new(std::nothrow) Weather();
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

bool Weather::init()
{
    if (!Node::init())
    {
        return false;
    }

    // ��ʼ������Ϊ����
    currentWeather = WeatherType::SUNNY;
    weatherSprite = Sprite::create("sunny.png");
    if (weatherSprite != nullptr)
    {
        const auto visibleSize = Director::getInstance()->getVisibleSize();
        const auto origin = Director::getInstance()->getVisibleOrigin();
        weatherSprite->setPosition(Vec2(origin.x + visibleSize.width - weatherSprite->getContentSize().width / 2,
            origin.y + visibleSize.height - weatherSprite->getContentSize().height / 2));
        this->addChild(weatherSprite);
    }

    // ��ʼ����νڵ�
    rainDropNode = Node::create();
    this->addChild(rainDropNode, 3); // ȷ����νڵ�� z-order

    return true;
}

void Weather::setWeather(WeatherType type)
{
    if (currentWeather != type)
    {
        currentWeather = type;
        updateWeatherSprite();
    }
}

Weather::WeatherType Weather::getWeather() const
{
    return currentWeather;
}

void Weather::randomDayWeather()
{
    int randomValue = rand() % 3; // ����0��2֮��������
    switch (randomValue)
    {
    case 0:
        setWeather(WeatherType::SUNNY);
        break;
    case 1:
        setWeather(WeatherType::RAINY);
        break;
    case 2:
        setWeather(WeatherType::CLOUDY);
        break;
    default:
        setWeather(WeatherType::SUNNY);
        break;
    }
}

void Weather::setNightWeather()
{
    setWeather(WeatherType::MOON);
}

void Weather::updateWeatherSprite()
{
    if (weatherSprite != nullptr)
    {
        this->removeChild(weatherSprite);
    }

    switch (currentWeather)
    {
    case WeatherType::SUNNY:
        weatherSprite = Sprite::create("sunny.png");
        break;
    case WeatherType::RAINY:
        weatherSprite = Sprite::create("rainy.png");
        break;
    case WeatherType::CLOUDY:
        weatherSprite = Sprite::create("cloudy.png");
        break;
    case WeatherType::MOON:
        weatherSprite = Sprite::create("moon.png");
        break;
    default:
        weatherSprite = nullptr;
        break;
    }

    if (weatherSprite != nullptr)
    {
        const auto visibleSize = Director::getInstance()->getVisibleSize();
        const auto origin = Director::getInstance()->getVisibleOrigin();
        weatherSprite->setAnchorPoint(Vec2(1.0f, 1.0f)); // ����ê��Ϊ���Ͻ�
        weatherSprite->setPosition(Vec2(origin.x + visibleSize.width-10,
            origin.y + visibleSize.height));
        weatherSprite->setScale(2.0f); // �Ŵ�ͼ��
        this->addChild(weatherSprite);
    }
}

void Weather::createRainAnimation()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    // ����������߾��飬����������Ļ
    float rainScale = 0.4f; // ��С
    float rainWidth = 0.4f * Sprite::create("rain1.png")->getContentSize().width;
    float rainHeight = 0.4f * Sprite::create("rain1.png")->getContentSize().height;

    // ���Ӽ�࣬ʹ���߾���ֲ�ϡ��һЩ
    float xSpacing = rainWidth * 1.5f;
    float ySpacing = rainHeight * 1.5f;

    for (float x = 0; x < visibleSize.width; x += xSpacing)
    {
        for (float y = visibleSize.height; y > -ySpacing; y -= ySpacing)
        {
            auto rainLine = Sprite::create("rain1.png");
            if (rainLine != nullptr)
            {
                rainLine->setAnchorPoint(Vec2(1.0f, 1.0f)); // ����ê��Ϊ���Ͻ�
                rainLine->setPosition(Vec2(origin.x + x + rainWidth, origin.y + y));
                rainLine->setScale(rainScale); // ��Сͼ��
                this->addChild(rainLine);
                rainLines.pushBack(rainLine); // �����߾�����ӵ��б���

                // ���������Ϸ������·����ƶ�����
                auto moveAction = MoveTo::create(1.0f, Vec2(origin.x + x, origin.y + y - rainHeight));
                auto playRainDropAnimation = CallFunc::create([this, rainLine]() {
                    createRainDropAnimation(rainLine->getPosition());
                    });
                auto resetPosition = CallFunc::create([rainLine, origin, x, y, rainWidth, rainHeight]() {
                    rainLine->setPosition(Vec2(origin.x + x + rainWidth, origin.y + y));
                    });
                auto sequence = Sequence::create(moveAction, playRainDropAnimation, resetPosition, nullptr);
                auto repeatAction = RepeatForever::create(sequence);
                rainLine->runAction(repeatAction);
            }
        }
    }
}

void Weather::createRainDropAnimation(const Vec2& position)
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    if (rainDropNode == nullptr)
    {
        rainDropNode = Node::create();
        this->addChild(rainDropNode, 5); // ȷ����νڵ�� z-order ���� Toolbar
    }

    // ���������ض���
    float rainDropScale = 0.4f; // ��С��0.4��
    auto rainDrop = Sprite::create("rain2.png");
    if (rainDrop != nullptr)
    {
        rainDrop->setPosition(position);
        rainDrop->setScale(rainDropScale);
        rainDropNode->addChild(rainDrop);

        // ���������ض���֡
        Vector<SpriteFrame*> animFrames(3);
        animFrames.pushBack(SpriteFrame::create("rain2.png", Rect(0, 0, rainDrop->getContentSize().width, rainDrop->getContentSize().height)));
        animFrames.pushBack(SpriteFrame::create("rain3.png", Rect(0, 0, rainDrop->getContentSize().width, rainDrop->getContentSize().height)));
        animFrames.pushBack(SpriteFrame::create("rain4.png", Rect(0, 0, rainDrop->getContentSize().width, rainDrop->getContentSize().height)));

        // ��������
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        auto animate = Animate::create(animation);

        // ���������ض�������
        auto fadeOut = FadeOut::create(0.5f);
        auto removeSelf = RemoveSelf::create();
        auto sequence = Sequence::create(animate, fadeOut, removeSelf, nullptr);
        rainDrop->runAction(sequence);
    }
}

void Weather::stopRainAnimation()
{
    // ֹͣ�������߾���Ķ������Ƴ�����
    for (auto rainLine : rainLines)
    {
        rainLine->stopAllActions();
        this->removeChild(rainLine);
    }
    rainLines.clear();

    // �Ƴ�������ξ���
    if (rainDropNode != nullptr)
    {
        rainDropNode->removeAllChildren();
    }
}