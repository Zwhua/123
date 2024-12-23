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
    cocos2d::TMXTiledMap* map1; // 声明 map1 变量
    Hero* _hero; // 声明 _hero 变量
    People* _people; // 声明 _people 变量
    int currentPage = 0; // 声明 currentPage 变量
    void dialogCallback(Node* sender);

    void createHero(bool fromAdventure, bool fromStore); // 声明 createHero 方法
    void addKeyboardListener(); // 声明 addKeyboardListener 方法
    void updateMapPosition(float dt); // 声明 updateMapPosition 方法
    cocos2d::Vec2 tileCoordForPosition(const cocos2d::Vec2& position); // 声明 tileCoordForPosition 方法

    Hero::Direction _lastDirection; // 声明 _lastDirection 变量

    bool _isMovingUp; // 声明 _isMovingUp 变量
    bool _isMovingDown; // 声明 _isMovingDown 变量
    bool _isMovingLeft; // 声明 _isMovingLeft 变量
    bool _isMovingRight; // 声明 _isMovingRight 变量

    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // 声明 onKeyPressed 方法
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event); // 声明 onKeyReleased 方法
};

#endif // __TOWN_SCENE_H__