#ifndef ADDB#ifndef ADDBUILDINGLAYER_H_
#define ADDBUILDINGLAYER_H_

#include "cocos2d.h"
#include "TouchListener.h"

using namespace cocos2d;

class AddBuildingLayer : public TouchListener {
public:
    AddBuildingLayer();
    virtual ~AddBuildingLayer();
    bool init();
    virtual void test(CCObject* pSender);
    CREATE_FUNC(AddBuildingLayer);

private:
    bool onTouchBegan(Touch* touch, Event* event);//����
    void onTouchMoved(Touch* touch, Event* event);//�ƶ�
    void onTouchEnded(Touch* touch, Event* event);//�ɿ�

    EventListenerTouchOneByOne* touchListener;
};

#endif /* ADDBUILDINGLAYER_H_ */


