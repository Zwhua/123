#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"
#include "hero.h"

class MapScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MapScene);

private:
    void createHero();
    void addKeyboardListener();
    void updateMapPosition(float dt);
    void updateMapPosition();
    bool checkCollision(const cocos2d::Vec2& position);
    cocos2d::Vec2 tileCoordForPosition(const cocos2d::Vec2& position);

    Hero* _hero;
    cocos2d::TMXTiledMap* _map;
    cocos2d::TMXLayer* _collidableLayer;

    bool _isMovingUp = false;
    bool _isMovingDown = false;
    bool _isMovingLeft = false;
    bool _isMovingRight = false;
    Hero::Direction _lastDirection;
};

#endif // __MAP_SCENE_H__
