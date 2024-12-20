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
    const float toolbarHeight = 25.0f; // �������߶�
    const float toolbarWidth = 25.0f; // ���������
    const float spacing = 50.0f; // ��������֮��ļ��

    // �����������߿�
    for (int i = 0; i < 6; ++i)
    {
        auto toolbarItem = Sprite::create("toolbar.png");
        if (toolbarItem != nullptr)
        {
            toolbarItem->setScale(0.5f); // �������߿��С
            float xPosition = (visibleSize.width - (6 * toolbarWidth + 5 * spacing)) / 2 + i * (toolbarWidth + spacing);
            float yPosition = toolbarHeight * 2;
            toolbarItem->setPosition(Vec2(xPosition, yPosition));
            this->addChild(toolbarItem, 3); // ȷ�����߿�� z-order Ϊ 3

            // ��ӵ���¼�������
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = [this, i, toolbarItem](Touch* touch, Event* event) {
                auto target = static_cast<Sprite*>(event->getCurrentTarget());
                Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
                Size size = target->getContentSize();
                Rect rect = Rect(0, 0, size.width, size.height);

                if (rect.containsPoint(locationInNode))
                {
                    // ���µ�ǰѡ�еĹ���������
                    selectedTool = i;
                    // ����֮ǰѡ�еĹ��߿���ɫ
                    if (currentSelectedItem != nullptr)
                    {
                        currentSelectedItem->runAction(TintTo::create(0.2f, 255, 255, 255));
                    }

                    // ���õ�ǰѡ�еĹ��߿���ɫ����
                    toolbarItem->runAction(TintTo::create(0.2f, 150, 150, 150));
                    currentSelectedItem = toolbarItem;

                    return true;
                }
                return false;
                };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, toolbarItem);

            // ������ߵĹ��߿��з��� pumpkin4.png
            if (i == 0)
            {
                auto pumpkinItem = Sprite::create("pumpkin4.png");
                if (pumpkinItem != nullptr)
                {
                    pumpkinItem->setScale(5.0f); // ��������ͼƬ��С
                    pumpkinItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                    toolbarItem->addChild(pumpkinItem, 4); // ȷ������ͼƬ�� z-order Ϊ 4
                }
            }
            // �ڵڶ������߿��з��� hook.png
            else if (i == 1)
            {
                auto hookItem = Sprite::create("hook.png");
                if (hookItem != nullptr)
                {
                    hookItem->setScale(5.0f); // ����ͼƬ��С
                    hookItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                    toolbarItem->addChild(hookItem, 4); // ȷ��ͼƬ�� z-order Ϊ 4
                }
            }
        }
    }
}

int Toolbar::getSelectedTool() const
{
    return selectedTool;
}