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
    _state = 1; // ��ʼ״̬Ϊ1

    // ���ݶ������ͳ�ʼ��ͼƬ�ļ�������
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

    // ������ʼ����
    _sprite = Sprite::create(_animalImages[_state - 1]);
    this->addChild(_sprite);

    // ���ȸ��º���
    this->schedule(CC_SCHEDULE_SELECTOR(Animal::updateAnimal), 1.0f);
    return true;
}

void Animal::updateAnimalType(AnimalType type)
{
    _type = type;
    _state = 1; // ����״̬Ϊ1
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
    // ���¶���״̬���߼�
    _state++;
    if (_state > _animalImages.size()) {
        _state = _animalImages.size(); // ͣ������״̬
    }
    _sprite->setTexture(_animalImages[_state - 1]);
}
