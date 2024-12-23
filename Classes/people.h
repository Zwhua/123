#ifndef __PEOPLE_H__
#define __PEOPLE_H__

#include "cocos2d.h"

class People : public cocos2d::Node
{
public:
    virtual bool init();
    CREATE_FUNC(People);

private:
    std::vector<cocos2d::Sprite*> sprites; // 用于存储九个精灵的向量
};

#endif // __PEOPLE_H__