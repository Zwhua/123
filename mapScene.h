#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Toolbar.h"
#include "Crops.h"
#include "Tool.h"

class MapScene : public cocos2d::Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MapScene);

    Crops* crop;
    Toolbar* _toolbar;
    Hero* _hero;
    cocos2d::TMXTiledMap* _map;
    cocos2d::Node* _cropLayer;
    Hero::Direction _lastDirection;

private:
    void createHero();
    void addKeyboardListener();
    void addTouchListener();
    void updateMapPosition(float dt);
  
    bool _isMovingUp = false;
    bool _isMovingDown = false;
    bool _isMovingLeft = false;
    bool _isMovingRight = false;
   
};

#endif // __MAP_SCENE_H__