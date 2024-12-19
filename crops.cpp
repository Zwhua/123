#include "Crops.h"

USING_NS_CC;

Crops* Crops::create(CropType type)
{
    Crops* pRet = new(std::nothrow) Crops();
    if (pRet && pRet->init(type))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        return nullptr;
    }
}

bool Crops::init(CropType type)
{
	//this->setAnchorPoint(Vec2(0.5f, 0.5f));
    if (!Node::init())
    {
        return false;
    }

    _type = type;
    _state = 1; // ��ʼ״̬Ϊ1

    // �����������ͳ�ʼ��ͼƬ�ļ�������
    switch (_type)
    {
    case CropType::pumpkin:
        _cropImages = { "pumpkin1.png", "pumpkin2.png", "pumpkin3.png", "pumpkin4.png" };
        break;
        // ������Ӹ�������
    default:
        break;
    }

    // ������ʼ����
    _sprite = Sprite::create(_cropImages[_state - 1]);
    this->addChild(_sprite);

    // ���ȸ��º���
    this->schedule(CC_SCHEDULE_SELECTOR(Crops::updateCrop), 5.0f); // ÿ5�����һ��
    return true;
}

void Crops::updateCrop(float dt)
{
    if (_state < 4)
    {
        _state++;
        _sprite->setTexture(_cropImages[_state - 1]);
    }
}