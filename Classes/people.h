#ifndef __PEOPLE_H__
#define __PEOPLE_H__

#include "cocos2d.h"

class People : public cocos2d::Node
{
public:
    virtual bool init();
    CREATE_FUNC(People);

private:
    std::vector<cocos2d::Sprite*> sprites; // ���ڴ洢�Ÿ����������
};

#endif // __PEOPLE_H__