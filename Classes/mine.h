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

    void increaseMiningSkill(int value); // ��������ڿ���ֵ�ķ���
    void saveMiningSkill(); // �����ڿ���

private:
    cocos2d::TMXTiledMap* map3;
    Hero* _hero;
    bool _isMovingUp;
    bool _isMovingDown;
    bool _isMovingLeft;
    bool _isMovingRight;
    Hero::Direction _lastDirection;
    cocos2d::Vector<cocos2d::Sprite*> _rocks;

    int _money; // ��Ǯ����
    int _miningSkill; // �ڿ���
    cocos2d::Label* _miningSkillLabel; // ��ʾ�ڿ��ܵı�ǩ

    void addKeyboardListener();
    void addMouseListener();
    void createRocks();
    void update(float dt);
    void saveMoney(); // �����Ǯ����
    void loadMoney(); // ���ؽ�Ǯ����
    void loadMiningSkill(); // �����ڿ���
};
#endif // __MINE_SCENE_H__