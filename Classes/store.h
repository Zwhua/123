#ifndef __STORE_H__
#define __STORE_H__

#include "cocos2d.h"

class Store : public cocos2d::Layer
{
public:
    virtual bool init(bool fromAdventure);
    static cocos2d::Scene* createScene(bool fromAdventure);
    static Store* create(bool fromAdventure); // 修改为带有参数的声明

private:
    cocos2d::LayerColor* _storeLayer;
    bool _fromAdventure;
    cocos2d::Sprite* _backgroundSprite;
    int _money;
    int _miningSkill; // 挖矿经验值
    cocos2d::Label* _moneyLabel;
    cocos2d::Label* _miningSkillLabel; // 显示挖矿经验值的标签

    void drawGrid();
    void updateMoneyLabel();
    void updateMiningSkillLabel(); // 更新挖矿经验值标签
    void loadMoney();
    void saveMoney();
    void loadMiningSkill(); // 加载挖矿经验值
    void showStore();
    void closeStore(cocos2d::Ref* sender);
    void buyItem(cocos2d::Ref* sender);
    void increaseMiningSkill();
};

#endif // __STORE_H__
