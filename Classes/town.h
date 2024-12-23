#ifndef __TOWN_SCENE_H__
#define __TOWN_SCENE_H__

#include "cocos2d.h"
#include "Hero.h" 
#include "people.h"

class townScene : public cocos2d::Scene
{
public:
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    void showCustomDialog(const std::vector<std::string>& messages, const std::string& title);

    // implement the "static create()" method manually
    static cocos2d::Scene* createScene(bool fromAdventure = false, bool fromStore = false);
    virtual bool init(bool fromAdventure, bool fromStore);
    static townScene* create(bool fromAdventure = false, bool fromStore = false);

private:
    cocos2d::TMXTiledMap* map1; // ���� map1 ����
    Hero* _hero; // ���� _hero ����
    People* _people; // ���� _people ����
    int currentPage = 0; // ���� currentPage ����
    void dialogCallback(Node* sender);

    void createHero(bool fromAdventure, bool fromStore); // ���� createHero ����
    void addKeyboardListener(); // ���� addKeyboardListener ����
    void updateMapPosition(float dt); // ���� updateMapPosition ����
    cocos2d::Vec2 tileCoordForPosition(const cocos2d::Vec2& position); // ���� tileCoordForPosition ����

    Hero::Direction _lastDirection; // ���� _lastDirection ����

    bool _isMovingUp; // ���� _isMovingUp ����
    bool _isMovingDown; // ���� _isMovingDown ����
    bool _isMovingLeft; // ���� _isMovingLeft ����
    bool _isMovingRight; // ���� _isMovingRight ����

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // ���� onKeyPressed ����
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // ���� onKeyReleased ����
};

#endif // __TOWN_SCENE_H__