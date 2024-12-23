#ifndef __WEATHER_H__
#define __WEATHER_H__

#include "cocos2d.h"

class Weather : public cocos2d::Node
{
public:
    enum class WeatherType
    {
        SUNNY,
        RAINY,
        CLOUDY,
        MOON
    };

    static Weather* create();
    virtual bool init();

    void setWeather(WeatherType type);
    WeatherType getWeather() const;
    void randomDayWeather(); // 随机选择白天天气
    void setNightWeather();  // 设置夜晚天气
    void createRainAnimation();
    void createRainDropAnimation(const cocos2d::Vec2& position);
    void stopRainAnimation(); // 停止下雨动画

private:
    WeatherType currentWeather;
    cocos2d::Sprite* weatherSprite;
    cocos2d::Node* rainDropNode;
    cocos2d::Vector<cocos2d::Sprite*> rainLines; // 存储所有雨线精灵

    void updateWeatherSprite();

};

#endif // __WEATHER_H__