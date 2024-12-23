#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__

#include "cocos2d.h"
#include "Hero.h"
#include "BackpackSystem.h"
#include "crops.h"
#include "animalBackpack.h"
#include "animal.h"

class Toolbar : public cocos2d::Node
{
public:
    virtual bool init();
    int getSelectedTool() const;

    // 在 Toolbar 类中添加一个成员变量
    BackpackSystem* _backpackSystem;
    void setUpdateCropTypeCallback(const std::function<void(Crops::CropType)>& callback);
    void setUpdateAnimalTypeCallback(const std::function<void(Animal::AnimalType)>& callback);

	CREATE_FUNC(Toolbar);

private:
    void createToolbar();
    std::function<void(Crops::CropType)> updateCropTypeCallback;
    std::function<void(Animal::AnimalType)> updateAnimalTypeCallback;
    std::function<void()> backpackButtonCallback; // 声明 backpackButtonCallback 成员变量
    int selectedTool = -1;
    cocos2d::Sprite* currentSelectedItem = nullptr;
    AnimalBackpack* _animalBackpackSystem;

};

#endif // __TOOLBAR_