#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include "cocos2d.h"

class GameScene : public cocos2d::CCScene {
public:
	GameScene();
	virtual ~GameScene();
	bool init();

	/*virtual void test(float dt);*/
	CREATE_FUNC(GameScene);
};

#endif /* GAMESCENE_H_ */
