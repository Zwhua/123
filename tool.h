#ifndef __TOOL_H__
#define __TOOL_H__


#include "cocos2d.h"
#include "Hero.h"
#include "Crops.h"

class Tool : public cocos2d::Node
{
public:

    static Tool* create();
    virtual bool init();

    void harvestCropsAroundHero(Hero* hero, cocos2d::Node* cropLayer);

};

#endif // __TOOL_H__
