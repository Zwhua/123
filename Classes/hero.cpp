#include "Hero.h"

USING_NS_CC;

Hero* Hero::create()
{// 加载图片到 SpriteFrameCache
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("stand.png", Rect(0, 0, 64, 64)), "stand.png");
    Hero* hero = new (std::nothrow) Hero();
    if (hero && hero->initWithSpriteFrameName("stand.png"))
    {
        hero->autorelease();
        hero->createAnimation();
        return hero;
    }
    CC_SAFE_DELETE(hero);
    return nullptr;
}

void Hero::createAnimation()
{
    // 加载图片到 SpriteFrameCache
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("walk1.png", Rect(0, 0, 64, 64)), "walk1.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("walk2.png", Rect(0, 0, 64, 64)), "walk2.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("stand.png", Rect(0, 0, 64, 64)), "stand.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("right1.png", Rect(0, 0, 64, 64)), "right1.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("right2.png", Rect(0, 0, 64, 64)), "right2.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("right3.png", Rect(0, 0, 64, 64)), "right3.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("left1.png", Rect(0, 0, 64, 64)), "left1.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("left2.png", Rect(0, 0, 64, 64)), "left2.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("left3.png", Rect(0, 0, 64, 64)), "left3.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("back1.png", Rect(0, 0, 64, 64)), "back1.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("back2.png", Rect(0, 0, 64, 64)), "back2.png");
    SpriteFrameCache::getInstance()->addSpriteFrame(SpriteFrame::create("back3.png", Rect(0, 0, 64, 64)), "back3.png");

    // 创建动画帧
    Vector<SpriteFrame*> walkFrames;
    walkFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("walk1.png"));
    walkFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("stand.png"));
    walkFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("walk2.png"));

    // 创建行走动画
    cocos2d::Animation* walkAnimation = Animation::createWithSpriteFrames(walkFrames, 0.1f);
    AnimationCache::getInstance()->addAnimation(walkAnimation, "walk");

    // 创建向右走的动画帧
    Vector<SpriteFrame*> walkRightFrames;
    walkRightFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("right2.png"));
    walkRightFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("right1.png"));
    walkRightFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("right3.png"));

    // 创建向右走的动画
    cocos2d::Animation* walkRightAnimation = Animation::createWithSpriteFrames(walkRightFrames, 0.1f);
    AnimationCache::getInstance()->addAnimation(walkRightAnimation, "walkRight");
    CCLOG("Walk right animation created successfully");

    // 创建向左走的动画帧
    Vector<SpriteFrame*> walkLeftFrames;
    walkLeftFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("left2.png"));
    walkLeftFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("left1.png"));
    walkLeftFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("left3.png"));

    // 创建向左走的动画
    cocos2d::Animation* walkLeftAnimation = Animation::createWithSpriteFrames(walkLeftFrames, 0.1f);
    AnimationCache::getInstance()->addAnimation(walkLeftAnimation, "walkLeft");
    CCLOG("Walk left animation created successfully");

    // 创建向后走的动画帧
    Vector<SpriteFrame*> walkBackFrames;
    walkBackFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("back2.png"));
    walkBackFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("back1.png"));
    walkBackFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("back3.png"));

    // 创建向后走的动画
    cocos2d::Animation* walkBackAnimation = Animation::createWithSpriteFrames(walkBackFrames, 0.1f);
    AnimationCache::getInstance()->addAnimation(walkBackAnimation, "walkBack");
    CCLOG("Walk back animation created successfully");
}

void Hero::playWalkAnimation(Direction direction)
{
    std::string animationName;
    switch (direction)
    {
    case Direction::RIGHT:
        animationName = "walkRight";
        break;
    case Direction::LEFT:
        animationName = "walkLeft";
        break;
    case Direction::UP:
        animationName = "walkBack";
        break;
    default:
        animationName = "walk";
		break;
    }

    auto animation = AnimationCache::getInstance()->getAnimation(animationName);
    if (animation)
    {
        auto animate = Animate::create(animation);
        this->runAction(RepeatForever::create(animate));
        CCLOG("Playing walk animation: %s", animationName.c_str());
    }
}

void Hero::stopWalkAnimation(Direction direction)
{
    this->stopAllActions();
    switch (direction)
    {
    case Direction::RIGHT:
        this->setSpriteFrame("right1.png");
        break;
    case Direction::LEFT:
        this->setSpriteFrame("left1.png");
        break;
    case Direction::UP:
        this->setSpriteFrame("back1.png");
        break;
    default:
        this->setSpriteFrame("stand.png");
        break;
    }
    CCLOG("Stopped walk animation and set to stand frame");
}

void Hero::moveUp(float dt)
{
    this->setPositionY(this->getPositionY() + 50.0f * dt);
    CCLOG("Moved up");
}

void Hero::moveDown(float dt)
{
    this->setPositionY(this->getPositionY() - 50.0f * dt);
    CCLOG("Moved down");
}

void Hero::moveLeft(float dt)
{
    this->setPositionX(this->getPositionX() - 50.0f * dt);
    CCLOG("Moved left");
}

void Hero::moveRight(float dt)
{
    this->setPositionX(this->getPositionX() + 50.0f * dt);
    CCLOG("Moved right");
}