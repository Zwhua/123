#ifndef __WELCOME_SCENE_H__
#define __WELCOME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h" // 包含 Cocos2d-x UI 头文件

class WelcomeScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // implement the "static create()" method manually
    CREATE_FUNC(WelcomeScene);
};

#endif // __WELCOME_SCENE_H__