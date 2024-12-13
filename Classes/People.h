#ifndef PEOPLE_H_
#define PEOPLE_H_

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocostudio;

class People : public cocos2d::CCNode {
public:
    People();
    virtual ~People();
    void setPosition(float new_x, float new_y);
    Armature* getArmature();

    //�ƶ�����
    void moveTo(int type, float toX, float toY);
    virtual void goForward(float dt);
    virtual void goBackward(float dt);
    virtual void goLeft(float dt);
    virtual void goRight(float dt);
    //CREATE_FUNC(People);

protected:
    //��ǰλ��
    float _x;
    float _y;
    //�ƶ���λ��
    float _toX;
    float _toY;
    //��������
    Armature* _armature;

    const static int GO_POSITIVE = 0;
    const static int GO_NEGATIVE = 1;
    const static int GO_LEFT = 2;
    const static int GO_RIGHT = 3;
};

#endif /* PEOPLE_H_ */


