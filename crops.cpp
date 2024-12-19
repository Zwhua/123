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
    _state = 1; // 初始状态为1

    // 根据作物类型初始化图片文件名数组
    switch (_type)
    {
    case CropType::pumpkin:
        _cropImages = { "pumpkin1.png", "pumpkin2.png", "pumpkin3.png", "pumpkin4.png" };
        break;
        // 可以添加更多种类
    default:
        break;
    }

    // 创建初始精灵
    _sprite = Sprite::create(_cropImages[_state - 1]);
    this->addChild(_sprite);

    // 调度更新函数
    this->schedule(CC_SCHEDULE_SELECTOR(Crops::updateCrop), 5.0f); // 每5秒更新一次
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