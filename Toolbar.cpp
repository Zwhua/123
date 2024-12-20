#include "Toolbar.h"

USING_NS_CC;

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
    const float toolbarHeight = 25.0f; // 工具栏高度
    const float toolbarWidth = 25.0f; // 工具栏宽度
    const float spacing = 50.0f; // 工具栏项之间的间距

    // 创建六个工具框
    for (int i = 0; i < 6; ++i)
    {
        auto toolbarItem = Sprite::create("toolbar.png");
        if (toolbarItem != nullptr)
        {
            toolbarItem->setScale(0.5f); // 调整工具框大小
            float xPosition = (visibleSize.width - (6 * toolbarWidth + 5 * spacing)) / 2 + i * (toolbarWidth + spacing);
            float yPosition = toolbarHeight * 2;
            toolbarItem->setPosition(Vec2(xPosition, yPosition));
            this->addChild(toolbarItem, 3); // 确保工具框的 z-order 为 3

            // 添加点击事件监听器
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = [this, i, toolbarItem](Touch* touch, Event* event) {
                auto target = static_cast<Sprite*>(event->getCurrentTarget());
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                Size size = target->getContentSize();
                Rect rect = Rect(0, 0, size.width, size.height);

                if (rect.containsPoint(locationInNode))
                {
                    // 更新当前选中的工具栏索引
                    selectedTool = i;
                    // 重置之前选中的工具框颜色
                    if (currentSelectedItem != nullptr)
                    {
                        currentSelectedItem->runAction(TintTo::create(0.2f, 255, 255, 255));
                    }

                    // 设置当前选中的工具框颜色变深
                    toolbarItem->runAction(TintTo::create(0.2f, 150, 150, 150));
                    currentSelectedItem = toolbarItem;

                    return true;
                }
                return false;
                };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, toolbarItem);

            // 在最左边的工具框中放入 pumpkin4.png
            if (i == 0)
            {
                auto pumpkinItem = Sprite::create("pumpkin4.png");
                if (pumpkinItem != nullptr)
                {
                    pumpkinItem->setScale(5.0f); // 调整作物图片大小
                    pumpkinItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                    toolbarItem->addChild(pumpkinItem, 4); // 确保作物图片的 z-order 为 4
                }
            }
            // 在第二个工具框中放入 hook.png
            else if (i == 1)
            {
                auto hookItem = Sprite::create("hook.png");
                if (hookItem != nullptr)
                {
                    hookItem->setScale(5.0f); // 调整图片大小
                    hookItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                    toolbarItem->addChild(hookItem, 4); // 确保图片的 z-order 为 4
                }
            }
        }
    }
}

int Toolbar::getSelectedTool() const
{
    return selectedTool;
}