#ifndef __TOOL_H__
#define __TOOL_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Crops.h"
#include "ui/CocosGUI.h"

class Tool : public cocos2d::Node
{
public:
    virtual bool init();
    static Tool* create();
    void harvestCropsAroundHero(Hero* hero, cocos2d::Node* cropLayer);
    void handleFishingEvent();

private:
    void showFishingResult();
    cocos2d::Rect fishingArea;
};

#endif // __TOOL_H__