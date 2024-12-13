#include "AddBuildingLayer.h"
#include "TouchListener.h"
#include "GameResources.h"

using namespace cocos2d;

AddBuildingLayer::AddBuildingLayer() : touchListener(nullptr) {
    // TODO Auto-generated constructor stub
}

AddBuildingLayer::~AddBuildingLayer() {
    // �Ƴ������¼�������
    _eventDispatcher->removeEventListener(touchListener);
}

bool AddBuildingLayer::init() {
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

    // ���������¼�������
    touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(AddBuildingLayer::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(AddBuildingLayer::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(AddBuildingLayer::onTouchEnded, this);

    // ע�ᴥ���¼�������
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

bool AddBuildingLayer::onTouchBegan(Touch* touch, Event* event) {
    // ���µ�ʵ��
    return true;
}

void AddBuildingLayer::onTouchMoved(Touch* touch, Event* event) {
    // �ƶ���ʵ��
}

void AddBuildingLayer::onTouchEnded(Touch* touch, Event* event) {
    // �ɿ���ʵ��
}

void AddBuildingLayer::test(CCObject* pSender) {
    // test ������ʵ��
}
