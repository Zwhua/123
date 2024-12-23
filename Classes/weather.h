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
    void randomDayWeather(); // ���ѡ���������
    void setNightWeather();  // ����ҹ������
    void createRainAnimation();
    void createRainDropAnimation(const cocos2d::Vec2& position);
    void stopRainAnimation(); // ֹͣ���궯��

private:
    WeatherType currentWeather;
    cocos2d::Sprite* weatherSprite;
    cocos2d::Node* rainDropNode;
    cocos2d::Vector<cocos2d::Sprite*> rainLines; // �洢�������߾���

    void updateWeatherSprite();

};

#endif // __WEATHER_H__