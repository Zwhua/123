#ifndef __CROPS_H__
#define __CROPS_H__

#include "cocos2d.h"

class Crops : public cocos2d::Node
{
public:
    enum class CropType
    {
        pumpkin,
        cropA,
        cropB,
        cropC,
        cropD,
        cropE,
        // �������Ӹ�������
    };

    static Crops* create(CropType type);
    bool init(CropType type);
    void updateCropState();
    CropType getType() const { return _type; }
    void updateCropType(CropType type);
    void updateCrop(float dt); // ���� updateCrop ��������

private:

    CropType _type;
    int _state; // ��ǰ״̬��1-4��
    cocos2d::Sprite* _sprite;
    std::vector<std::string> _cropImages; // ����ͼƬ�ļ�������
};

#endif // __CROPS_H__