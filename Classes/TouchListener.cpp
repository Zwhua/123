#include "TouchListener.h"
#include "GameBGLayer.h"
#include "GameResources.h"

using namespace cocos2d;

TouchListener::TouchListener() : touchListener(nullptr) {
    // TODO Auto-generated constructor stub
}

TouchListener::~TouchListener() {
    // �Ƴ������¼�������
    _eventDispatcher->removeEventListener(touchListener);
}

bool TouchListener::init() {
    if (!CCLayer::init()) {
        return false;
    }

    winSize = CCDirector::sharedDirector()->getVisibleSize();
    origin = CCDirector::sharedDirector()->getVisibleOrigin();

    // ���������¼�������
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(TouchListener::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(TouchListener::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(TouchListener::onTouchEnded, this);

    // ע�ᴥ���¼�������
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool TouchListener::onTouchBegan(Touch* touch, Event* event) {
    beginPoint = touch->getLocation();
    return true;
}

void TouchListener::onTouchMoved(Touch* touch, Event* event) {
    winSize = CCDirector::sharedDirector()->getVisibleSize();
    origin = CCDirector::sharedDirector()->getVisibleOrigin();

    GameResources* resource = GameResources::GetInstance();
    float winHeight = resource->getWinHeight();
    float winWidth = resource->getWinWidth();

    //�����Сƫ��
    float maxValue_top = origin.y + winSize.height / 2 - (winHeight - winSize.height) / 2;
    float maxValue_bottom = origin.y + winSize.height / 2 + (winHeight - winSize.height) / 2;
    float maxValue_left = origin.x + winSize.width / 2 - (winWidth - winSize.width) / 2;
    float maxValue_right = origin.x + winSize.width / 2 + (winWidth - winSize.width) / 2;

    endPoint = touch->getLocation();
    CCPoint MoveTo = ccpSub(endPoint, beginPoint);
    beginPoint = endPoint;

    CCPoint now_position = this->getPosition();

    float now_x = now_position.x;
    float now_y = now_position.y;
    CCPoint result_position = ccpAdd(now_position, MoveTo);

    //����Խ�紦��
    if (result_position.x + this->getContentSize().width / 2 > maxValue_right) {
        result_position.x = maxValue_right - this->getContentSize().width / 2;
    }
    else if (result_position.x + this->getContentSize().width / 2 < maxValue_left) {
        result_position.x = maxValue_left - this->getContentSize().width / 2;
    }
    //����Խ�紦��
    if (result_position.y + this->getContentSize().height / 2 > maxValue_bottom) {
        result_position.y = maxValue_bottom - this->getContentSize().height / 2;
    }
    else if (result_position.y + this->getContentSize().height / 2 < maxValue_top) {
        result_position.y = maxValue_top - this->getContentSize().height / 2;
    }

    this->setPosition(result_position);
}

void TouchListener::onTouchEnded(Touch* touch, Event* event) {
    // �ɿ���ʵ��
}

