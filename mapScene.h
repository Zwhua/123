#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"
#include "hero.h"
#include "Toolbar.h"
#include "Crops.h"

class MapScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MapScene);
    Crops* crop;

private:
    void createHero();
    void addKeyboardListener();
    void addTouchListener();
    void updateMapPosition(float dt);
    cocos2d::Vec2 tileCoordForPosition(const cocos2d::Vec2& position);
    void createToolbar();


    Hero* _hero;
    cocos2d::TMXTiledMap* _map;
    cocos2d::Node* _cropLayer;
 
    bool _isMovingUp = false;
    bool _isMovingDown = false;
    bool _isMovingLeft = false;
    bool _isMovingRight = false;
    Hero::Direction _lastDirection;
};

#endif // __MAP_SCENE_H__