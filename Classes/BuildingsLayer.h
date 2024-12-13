#ifndef BUILDINGSLAYER_H_
#define BUILDINGSLAYER_H_

#include "cocos2d.h"
#include "TouchListener.h"

using namespace cocos2d;

class BuildingsLayer : public TouchListener {
public:
    BuildingsLayer();
    virtual ~BuildingsLayer();
    bool init();
    CREATE_FUNC(BuildingsLayer);

private:
    bool onTouchBegan(Touch* touch, Event* event);//按下
    void onTouchMoved(Touch* touch, Event* event);//移动
    void onTouchEnded(Touch* touch, Event* event);//松开

    CCSize winSize;
    CCPoint origin;
    CCPoint beginPoint;
    CCPoint endPoint;
    EventListenerTouchOneByOne* touchListener;
};

#endif /* BUILDINGSLAYER_H_ */

