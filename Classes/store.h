#ifndef __STORE_H__
#define __STORE_H__

#include "cocos2d.h"

class Store : public cocos2d::Layer
{
public:
    virtual bool init(bool fromAdventure);
    static cocos2d::Scene* createScene(bool fromAdventure);
    static Store* create(bool fromAdventure); // �޸�Ϊ���в���������

private:
    cocos2d::LayerColor* _storeLayer;
    bool _fromAdventure;
    cocos2d::Sprite* _backgroundSprite;
    int _money;
    int _miningSkill; // �ڿ���ֵ
    cocos2d::Label* _moneyLabel;
    cocos2d::Label* _miningSkillLabel; // ��ʾ�ڿ���ֵ�ı�ǩ

    void drawGrid();
    void updateMoneyLabel();
    void updateMiningSkillLabel(); // �����ڿ���ֵ��ǩ
    void loadMoney();
    void saveMoney();
    void loadMiningSkill(); // �����ڿ���ֵ
    void showStore();
    void closeStore(cocos2d::Ref* sender);
    void buyItem(cocos2d::Ref* sender);
    void increaseMiningSkill();
};

#endif // __STORE_H__
