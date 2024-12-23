#ifndef __MAP_SCENE_H__
#define __MAP_SCENE_H__

#include "cocos2d.h"
#include "Hero.h"
#include "Toolbar.h"
#include "Crops.h"
#include "Tool.h"
#include "Weather.h"
#include "BackpackSystem.h"
#include "Animal.h"
#include "AnimalBackpack.h"

class MapScene : public cocos2d::Scene
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MapScene);

    void updateCurrentCropType(Crops::CropType cropType);
    void updateCurrentAnimalType(Animal::AnimalType animalType);

    void toggleBackpackLayer(); // ���� toggleBackpackLayer ����
    void toggleAnimalBackpackLayer();

    Crops* crop;
    Toolbar* _toolbar;
    Hero* _hero;
    Hero::Direction _lastDirection;
    Weather* _weather;
    cocos2d::TMXTiledMap* _map;
    cocos2d::Node* _cropLayer;
    cocos2d::Node* _animalLayer;
    BackpackSystem* _backpackSystem;
    Crops::CropType _currentCropType;

    AnimalBackpack* _animalBackpackSystem;
    Animal::AnimalType _currentAnimalType;


private:
    void addKeyboardListener();
    void addTouchListener();
    void updateMapPosition(float dt);
    void switchToNight(float dt); // �л���ҹ��ķ���
    void switchToDay(float dt);   // �л�������ķ���
    void loadMapState();
	void saveMapState();



    bool _isMovingUp = false;
    bool _isMovingDown = false;
    bool _isMovingLeft = false;
    bool _isMovingRight = false;
    bool _isBackpackOpen = false; // ��־���������ٱ������״̬
    bool _isAnimalBackpackOpen = false;
   
};

#endif // __MAP_SCENE_H__