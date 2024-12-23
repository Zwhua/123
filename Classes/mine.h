#ifndef __MINE_SCENE_H__
#define __MINE_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"

class mine : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(mine);

    void increaseMiningSkill(int value); // 添加增加挖矿经验值的方法
    void saveMiningSkill(); // 保存挖矿技能

private:
    cocos2d::TMXTiledMap* map3;
    Hero* _hero;
    bool _isMovingUp;
    bool _isMovingDown;
    bool _isMovingLeft;
    bool _isMovingRight;
    Hero::Direction _lastDirection;
    cocos2d::Vector<cocos2d::Sprite*> _rocks;

    int _money; // 金钱数量
    int _miningSkill; // 挖矿技能
    cocos2d::Label* _miningSkillLabel; // 显示挖矿技能的标签

    void addKeyboardListener();
    void addMouseListener();
    void createRocks();
    void update(float dt);
    void saveMoney(); // 保存金钱数量
    void loadMoney(); // 加载金钱数量
    void loadMiningSkill(); // 加载挖矿技能
};
#endif // __MINE_SCENE_H__