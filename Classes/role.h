#ifndef __ROLE_SCENE_H__
#define __ROLE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class CharacterRole : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(CharacterRole);
};

#endif // __ROLE_SCENE_H__
