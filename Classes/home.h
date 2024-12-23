#include "Crops.h"
#include "Hero.h"

#ifndef HOME_H
#define HOME_H

#include <string>

class HomeScene : public cocos2d::Scene {
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(HomeScene);
    Hero* _hero;
    Hero::Direction _lastDirection;

private:
	void createHero();  
    std::string imagePath = "home.png";
    void addKeyboardListener();
    void updateMapPosition(float dt);
    void HomeScene::addMouseListener();
    void HomeScene::sleep();

    bool _isMovingUp = false;
    bool _isMovingDown = false;
    bool _isMovingLeft = false;
    bool _isMovingRight = false;
};

#endif // HOME_H