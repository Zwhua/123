#include "WelcomeScene.h"
#include "MapScene.h"
#include "ui/CocosGUI.h" // ���� Cocos2d-x UI ͷ�ļ�

USING_NS_CC;

Scene* WelcomeScene::createScene()
{
    return WelcomeScene::create();
}

bool WelcomeScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    // ��ȡ���ڵĴ�С
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ����һ��������Ϊ����
    auto background = Sprite::create("sky.jpg");
    if (background == nullptr)
    {
        CCLOG("Error while loading: %s", "sky.jpg");
        return false;
    }
    else
    {
        // ��ȡͼƬ�Ĵ�С
            const auto backgroundSize = background->getContentSize();

        // �������ű���
        const float scaleX = visibleSize.width / backgroundSize.width;
        const float scaleY = visibleSize.height / backgroundSize.height;
        const float scale = std::max(scaleX, scaleY);

        // ���ñ�����������ű���
        background->setScale(scale);

        // ���ñ��������λ��Ϊ��Ļ����
        background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
        this->addChild(background, 0);
    }

    // ���� "��Ϸ����" ����
    auto titleSprite = Sprite::create("title.png");
    if (titleSprite == nullptr)
    {
        CCLOG("Error while loading: %s", "title.png");
        return false;
    }
    else
    {
        titleSprite->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.6));
        titleSprite->setScale(1.8); // ���������С
        this->addChild(titleSprite, 1);

        // ���������ƶ��Ķ���
        auto moveUp = MoveBy::create(1.0f, Vec2(0, 10));
        auto moveDown = MoveBy::create(1.0f, Vec2(0, -10));
        auto sequence = Sequence::create(moveUp, moveDown, nullptr);
        auto repeat = RepeatForever::create(sequence);

        // ���ж���
        titleSprite->runAction(repeat);
    }

    // ��ť֮��ļ��
    const float buttonSpacing = visibleSize.width / 4;

    // ���� "����" ��ť
    auto createButton = ui::Button::create("create_normal.png", "create_selected.png");
    createButton->setPosition(Vec2(buttonSpacing, visibleSize.height * 0.2));
    createButton->setScale(1.8); // ������ť��С
    this->addChild(createButton, 1);
   
    // ��ӵ���¼�������
    createButton->addClickEventListener([](Ref* sender) {
        auto mapScene = MapScene::createScene();
        if (mapScene != nullptr) {
            Director::getInstance()->replaceScene(TransitionFade::create(0.5, mapScene));
        }
        else {
            CCLOG("Error: Failed to create MapScene");
            // ������������¼��־�򴴽�Ĭ�ϳ���
        }
        Director::getInstance()->replaceScene(TransitionFade::create(0.5, mapScene));
        });

    // ���� "����" ��ť
    auto loadButton = ui::Button::create("load_normal.png", "load_selected.png");
    loadButton->setPosition(Vec2(buttonSpacing * 2, visibleSize.height * 0.2));
    loadButton->setScale(1.8); // ������ť��С
    this->addChild(loadButton, 1);

    // ���� "�˳�" ��ť
    auto exitButton = ui::Button::create("exit_normal.png", "exit_selected.png");
    exitButton->setPosition(Vec2(buttonSpacing * 3, visibleSize.height * 0.2));
    exitButton->setScale(1.8); // ������ť��С
    this->addChild(exitButton, 1);

    // ��ӵ���¼�������
    exitButton->addClickEventListener([](Ref* sender) {
        Director::getInstance()->end();
        });

    return true;
}