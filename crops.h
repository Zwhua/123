#ifndef __CROPS_H__
#define __CROPS_H__

#include "cocos2d.h"

class Crops : public cocos2d::Node
{
public:
    enum class CropType
    {
        pumpkin,
        // 可以添加更多种类
    };

    static Crops* create(CropType type);
    void updateCrop(float dt);

private:
    bool init(CropType type);
    void updateCropState();

    CropType _type;
    int _state; // 当前状态（1-4）
    cocos2d::Sprite* _sprite;
    std::vector<std::string> _cropImages; // 作物图片文件名数组
};

#endif // __CROPS_H__