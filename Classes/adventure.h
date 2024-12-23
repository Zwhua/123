#ifndef __ADVENTURE_SCENE_H__
#define __ADVENTURE_SCENE_H__

#include "cocos2d.h"
#include "Hero.h" // 确保包含 Hero 头文件

class adventure : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(adventure);

private:
    cocos2d::TMXTiledMap* map2;
    Hero* _hero;
    bool _isMovingUp;
    bool _isMovingDown;
    bool _isMovingLeft;
    bool _isMovingRight;
    Hero::Direction _lastDirection;

    void addKeyboardListener();
    void addMouseListener();
    void update(float dt);
};

#endif // __ADVENTURE_SCENE_H__