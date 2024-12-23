#ifndef ANIMALBACKPACK_H
#define ANIMALBACKPACK_H

#include "cocos2d.h"
#include "Animal.h"

class AnimalBackpack : public cocos2d::Node
{
public:
    AnimalBackpack();
    ~AnimalBackpack();
    static AnimalBackpack* create();
    void setSelectAnimalCallback(const std::function<void(const std::string&, Animal::AnimalType)>& callback);

    void setUpdateToolbarCallback(const std::function<void(const std::string&)>& callback);

    void showBackpackLayer(cocos2d::Node* parent);
    void hideBackpackLayer();
    bool isBackpackOpen() const;

    // 选择物品
    void selectItem(const std::string& itemName);

    // 获取当前选择的行为
    const std::string& getCurrentAction() const;

private:

    // 初始化背包层
    void initBackpackLayer();

    // 添加背包物品到层
    void addBackpackItems();

    // 背包层和当前动作
    cocos2d::Sprite* backpackLayer; // 修改类型为 Sprite*
    std::string currentAction;

    std::function<void(const std::string&, Animal::AnimalType)> _selectAnimalCallback;
    std::function<void(const std::string&)> _updateToolbarCallback;
    bool _isBackpackOpen;
};

#endif // ANIMALBACKPACK_H#pragma once
