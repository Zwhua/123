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

    // ѡ����Ʒ
    void selectItem(const std::string& itemName);

    // ��ȡ��ǰѡ�����Ϊ
    const std::string& getCurrentAction() const;

private:

    // ��ʼ��������
    void initBackpackLayer();

    // ��ӱ�����Ʒ����
    void addBackpackItems();

    // ������͵�ǰ����
    cocos2d::Sprite* backpackLayer; // �޸�����Ϊ Sprite*
    std::string currentAction;

    std::function<void(const std::string&, Animal::AnimalType)> _selectAnimalCallback;
    std::function<void(const std::string&)> _updateToolbarCallback;
    bool _isBackpackOpen;
};

#endif // ANIMALBACKPACK_H#pragma once
