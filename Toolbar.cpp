#include "Toolbar.h"

USING_NS_CC;

Toolbar* Toolbar::create()
{
    Toolbar* ret = new (std::nothrow) Toolbar();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        delete ret;
        return nullptr;
    }
}

bool Toolbar::init()
{
    if (!Node::init())
    {
        return false;
    }

    createToolbar();
    return true;
}

void Toolbar::createToolbar()
{
    const auto visibleSize = Director::getInstance()->getVisibleSize();

    // 创建物品栏背景
    auto toolbarBackground = Sprite::create("toolbar_background.png");
    toolbarBackground->setPosition(Vec2(visibleSize.width / 2, toolbarBackground->getContentSize().height / 2));
    this->addChild(toolbarBackground, 1);

}
