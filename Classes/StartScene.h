#ifndef STARTSCENE_H_
#define STARTSCENE_H_

#include "cocos2d.h"

class StartScene : public cocos2d::CCScene {
public:
	StartScene();
	virtual ~StartScene();
	bool init();

	//��ʼ�����������ť����Ļص�����
	void menustartcallback(CCObject* pSender);
	void menuquitcallback(CCObject* pSender);

	CREATE_FUNC(StartScene);
};

#endif /* STARTSCENE_H_ */
