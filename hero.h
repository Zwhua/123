#ifndef __HERO_H__
#define __HERO_H__

#include "cocos2d.h"

class Hero : public cocos2d::Sprite
{
public:
    enum class Direction
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    static Hero* create();
    void createAnimation();
    void playWalkAnimation(Direction direction);
    void stopWalkAnimation(Direction direction);
    void moveUp(float dt);
    void moveDown(float dt);
    void moveLeft(float dt);
    void moveRight(float dt);
    bool checkCollisionWithNewPosition(const cocos2d::Vec2& newHeroPosition, cocos2d::Node* other);
};

#endif // __HERO_H__