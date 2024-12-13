#include "BuildingsLayer.h"
#include "TouchListener.h"
#include "GameResources.h"

using namespace cocos2d;

BuildingsLayer::BuildingsLayer() : touchListener(nullptr) {
    // TODO Auto-generated constructor stub
}

BuildingsLayer::~BuildingsLayer() {
    // 移除触摸事件监听器
    _eventDispatcher->removeEventListener(touchListener);
}

bool BuildingsLayer::init() {
    TouchListener::init();
    GameResources* res = GameResources::GetInstance();

    CCSize size;
    size.height = res->getWinHeight();
    size.width = res->getWinWidth();
    this->setContentSize(size);

    winSize = CCDirector::sharedDirector()->getVisibleSize();
    origin = CCDirector::sharedDirector()->getVisibleOrigin();
    CCSprite* canteen = CCSprite::create("canteen.png");
    canteen->setPosition(ccp(res->getRealWidth(7), res->getRealHeight(7)));
    CCLog("%d %d", res->getRealHeight(15), res->getRealWidth(15));
    // canteen->setPosition( ccp(origin.x + winSize.width/2, origin.y + winSize.height/2) );
    this->addChild(canteen);

    // 创建触摸事件监听器
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(BuildingsLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(BuildingsLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(BuildingsLayer::onTouchEnded, this);

    // 注册触摸事件监听器
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool BuildingsLayer::onTouchBegan(Touch* touch, Event* event) {
    // 按下的实现
    return true;
}

void BuildingsLayer::onTouchMoved(Touch* touch, Event* event) {
    // 移动的实现
}

void BuildingsLayer::onTouchEnded(Touch* touch, Event* event) {
    // 松开的实现
}

