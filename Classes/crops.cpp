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
    case CropType::cropA:
        _cropImages = { "cropA1.png", "cropA2.png", "cropA3.png", "cropA4.png" };
        break;
    case CropType::cropB:
        _cropImages = { "cropB1.png", "cropB2.png", "cropB3.png", "cropB4.png" };
        break;
    case CropType::cropC:
        _cropImages = { "cropC1.png", "cropC2.png", "cropC3.png", "cropC4.png" };
        break;
    case CropType::cropD:
        _cropImages = { "cropD1.png", "cropD2.png", "cropD3.png", "cropD4.png" };
        break;
    case CropType::cropE:
        _cropImages = { "cropE1.png", "cropE2.png", "cropE3.png", "cropE4.png" };
        break;
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

void Crops::updateCropType(CropType type) {
    _type = type;
    _state = 1; // 重置状态为1
    switch (_type) {
    case CropType::pumpkin:
        _cropImages = { "pumpkin1.png", "pumpkin2.png", "pumpkin3.png", "pumpkin4.png" };
        break;
    case CropType::cropA:
        _cropImages = { "cropA1.png", "cropA2.png", "cropA3.png", "cropA4.png" };
        break;
    case CropType::cropB:
        _cropImages = { "cropB1.png", "cropB2.png", "cropB3.png", "cropB4.png" };
        break;
    case CropType::cropC:
        _cropImages = { "cropC1.png", "cropC2.png", "cropC3.png", "cropC4.png" };
        break;
    case CropType::cropD:
        _cropImages = { "cropD1.png", "cropD2.png", "cropD3.png", "cropD4.png" };
        break;
    case CropType::cropE:
        _cropImages = { "cropE1.png", "cropE2.png", "cropE3.png", "cropE4.png" };
        break;
    default:
        break;
    }
    if (_sprite) {
        _sprite->setTexture(_cropImages[_state - 1]);
    }
}

void Crops::updateCrop(float dt) {
    // 更新作物状态的逻辑
    _state++;
    if (_state > _cropImages.size()) {
        _state = _cropImages.size(); // 停在最终状态
    }
    _sprite->setTexture(_cropImages[_state - 1]);
}