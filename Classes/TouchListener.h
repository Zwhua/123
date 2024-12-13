#ifndef TOUCHLISTENER_H_
#define TOUCHLISTENER_H_

#include "cocos2d.h"

using namespace cocos2d;

class TouchListener : public cocos2d::CCLayer {
public:
    TouchListener();
    virtual ~TouchListener();
    bool init();
    CREATE_FUNC(TouchListener);

protected:
    bool onTouchBegan(Touch* touch, Event* event);//����
    void onTouchMoved(Touch* touch, Event* event);//�ƶ�
    void onTouchEnded(Touch* touch, Event* event);//�ɿ�

    CCSize winSize;
    CCPoint origin;
    CCPoint beginPoint;
    CCPoint endPoint;
    EventListenerTouchOneByOne* touchListener;
};

#endif /* TOUCHLISTENER_H_ */

