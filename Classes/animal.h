#ifndef ANIMAL_H
#define ANIMAL_H

#include "cocos2d.h"

class Animal : public cocos2d::Sprite
{
public:
    enum class AnimalType
    {
        animalA,
        animalB,
    };

    static Animal* create(AnimalType type);

    bool init(AnimalType type);

    void updateAnimalType(AnimalType type);
    void updateAnimal(float dt);
    AnimalType getType() const { return _type; }

private:
    AnimalType _type;
    int _state;
    std::vector<std::string> _animalImages;
    cocos2d::Sprite* _sprite;
};

#endif // ANIMAL_H

#pragma once
