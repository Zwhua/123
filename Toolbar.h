#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__

#include "cocos2d.h"
#include "Hero.h"

class Toolbar : public cocos2d::Node
{
public:
    virtual bool init();
    int getSelectedTool() const;
	CREATE_FUNC(Toolbar);

private:
    void createToolbar();
    int selectedTool = -1;
    cocos2d::Sprite* currentSelectedItem = nullptr; // ���ٵ�ǰѡ�еĹ��߿�
};

#endif // __TOOLBAR_