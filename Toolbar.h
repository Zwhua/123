#ifndef __TOOLBAR_H__
#define __TOOLBAR_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Toolbar : public cocos2d::Node
{
public:
    static Toolbar* create();
    virtual bool init();
    void createToolbar();

};

#endif // __TOOLBAR_H__