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

    // 初始化天气为晴天
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

    // 初始化雨滴节点
    rainDropNode = Node::create();
    this->addChild(rainDropNode, 3); // 确保雨滴节点的 z-order

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
    int randomValue = rand() % 3; // 生成0到2之间的随机数
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
        weatherSprite->setAnchorPoint(Vec2(1.0f, 1.0f)); // 设置锚点为右上角
        weatherSprite->setPosition(Vec2(origin.x + visibleSize.width-10,
            origin.y + visibleSize.height));
        weatherSprite->setScale(2.0f); // 放大图标
        this->addChild(weatherSprite);
    }
}

void Weather::createRainAnimation()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const auto origin = Director::getInstance()->getVisibleOrigin();

    // 创建多个雨线精灵，覆盖整个屏幕
    float rainScale = 0.4f; // 缩小
    float rainWidth = 0.4f * Sprite::create("rain1.png")->getContentSize().width;
    float rainHeight = 0.4f * Sprite::create("rain1.png")->getContentSize().height;

    // 增加间距，使雨线精灵分布稀疏一些
    float xSpacing = rainWidth * 1.5f;
    float ySpacing = rainHeight * 1.5f;

    for (float x = 0; x < visibleSize.width; x += xSpacing)
    {
        for (float y = visibleSize.height; y > -ySpacing; y -= ySpacing)
        {
            auto rainLine = Sprite::create("rain1.png");
            if (rainLine != nullptr)
            {
                rainLine->setAnchorPoint(Vec2(1.0f, 1.0f)); // 设置锚点为右上角
                rainLine->setPosition(Vec2(origin.x + x + rainWidth, origin.y + y));
                rainLine->setScale(rainScale); // 缩小图标
                this->addChild(rainLine);
                rainLines.pushBack(rainLine); // 将雨线精灵添加到列表中

                // 创建从右上方到左下方的移动动画
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
        this->addChild(rainDropNode, 5); // 确保雨滴节点的 z-order 高于 Toolbar
    }

    // 创建雨滴落地动画
    float rainDropScale = 0.4f; // 缩小到0.4倍
    auto rainDrop = Sprite::create("rain2.png");
    if (rainDrop != nullptr)
    {
        rainDrop->setPosition(position);
        rainDrop->setScale(rainDropScale);
        rainDropNode->addChild(rainDrop);

        // 创建雨滴落地动画帧
        Vector<SpriteFrame*> animFrames(3);
        animFrames.pushBack(SpriteFrame::create("rain2.png", Rect(0, 0, rainDrop->getContentSize().width, rainDrop->getContentSize().height)));
        animFrames.pushBack(SpriteFrame::create("rain3.png", Rect(0, 0, rainDrop->getContentSize().width, rainDrop->getContentSize().height)));
        animFrames.pushBack(SpriteFrame::create("rain4.png", Rect(0, 0, rainDrop->getContentSize().width, rainDrop->getContentSize().height)));

        // 创建动画
        auto animation = Animation::createWithSpriteFrames(animFrames, 0.1f);
        auto animate = Animate::create(animation);

        // 创建雨滴落地动画序列
        auto fadeOut = FadeOut::create(0.5f);
        auto removeSelf = RemoveSelf::create();
        auto sequence = Sequence::create(animate, fadeOut, removeSelf, nullptr);
        rainDrop->runAction(sequence);
    }
}

void Weather::stopRainAnimation()
{
    // 停止所有雨线精灵的动作并移除它们
    for (auto rainLine : rainLines)
    {
        rainLine->stopAllActions();
        this->removeChild(rainLine);
    }
    rainLines.clear();

    // 移除所有雨滴精灵
    if (rainDropNode != nullptr)
    {
        rainDropNode->removeAllChildren();
    }
}