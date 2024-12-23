#include "Animal.h"

USING_NS_CC;

Animal* Animal::create(AnimalType type)
{
    Animal* animal = new (std::nothrow) Animal();
    if (animal && animal->init(type))
    {
        animal->autorelease();
        return animal;
    }
    else
    {
        delete animal;
        return nullptr;
    }
}

bool Animal::init(AnimalType type)
{
    if (!Node::init())
    {
        return false;
    }

    _type = type;
    _state = 1; // 初始状态为1

    // 根据动物类型初始化图片文件名数组
    switch (_type)
    {
    case AnimalType::animalA:
        _animalImages = { "animalA1.png", "animalA2.png", "animalA3.png" };
        break;
    case AnimalType::animalB:
        _animalImages = { "animalB.png", "animalB2.png", "animalB3.png" };
        break;
    default:
        break;
    }

    // 创建初始精灵
    _sprite = Sprite::create(_animalImages[_state - 1]);
    this->addChild(_sprite);

    // 调度更新函数
    this->schedule(CC_SCHEDULE_SELECTOR(Animal::updateAnimal), 1.0f);
    return true;
}

void Animal::updateAnimalType(AnimalType type)
{
    _type = type;
    _state = 1; // 重置状态为1
    switch (_type)
    {
    case AnimalType::animalA:
        _animalImages = { "animalA1.png", "animalA2.png", "animalA3.png" };
        break;
    case AnimalType::animalB:
        _animalImages = { "animalB.png", "animalB2.png", "animalB3.png" };
        break;

    default:
        break;
    }
    if (_sprite)
    {
        _sprite->setTexture(_animalImages[_state - 1]);
    }
}

void Animal::updateAnimal(float dt)
{
    // 更新动物状态的逻辑
    _state++;
    if (_state > _animalImages.size()) {
        _state = _animalImages.size(); // 停在最终状态
    }
    _sprite->setTexture(_animalImages[_state - 1]);
}
