#include "Toolbar.h"
#include "BackpackSystem.h"
#include "crops.h"
#include "MapScene.h"
#include "animalBackpack.h"
#include "animal.h"

USING_NS_CC;

bool Toolbar::init()
{
    if (!Node::init())
    {
        return false;
    }

    // �� Toolbar::init() �����г�ʼ�� _backpackSystem
    _backpackSystem = BackpackSystem::create();
    this->addChild(_backpackSystem, 4); // ȷ������ϵͳ�� z-order Ϊ 4

    createToolbar();
    // ���ø��¹������Ļص�����
    _backpackSystem->setUpdateToolbarCallback([this](const std::string& cropImage) {
        auto toolbarItem = dynamic_cast<Sprite*>(this->getChildren().at(0));
        if (toolbarItem) {
            toolbarItem->removeAllChildren();
            auto cropItem = Sprite::create(cropImage);
            if (cropItem) {
                cropItem->setScale(5.0f);
                cropItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                toolbarItem->addChild(cropItem, 4);
            }
        }
        // ���µ�ǰ��������
        if (updateCropTypeCallback) {
            Crops::CropType cropType;
            if (cropImage == "pumpkin4.png") {
                cropType = Crops::CropType::pumpkin;
            }
            else if (cropImage == "cropA4.png") {
                cropType = Crops::CropType::cropA;
            }
            else if (cropImage == "cropB4.png") {
                cropType = Crops::CropType::cropB;
            }
            else if (cropImage == "cropC4.png") {
                cropType = Crops::CropType::cropC;
            }
            else if (cropImage == "cropD4.png") {
                cropType = Crops::CropType::cropD;
            }
            else if (cropImage == "cropE4.png") {
                cropType = Crops::CropType::cropE;
            }

            updateCropTypeCallback(cropType);
        }
        });

    // �� Toolbar::init() �����г�ʼ�� _animalBackpackSystem
    _animalBackpackSystem = AnimalBackpack::create();
    this->addChild(_animalBackpackSystem, 4); // ȷ�����ﱳ��ϵͳ�� z-order Ϊ 4

    // ���ø��¹������Ļص�����
    _animalBackpackSystem->setUpdateToolbarCallback([this](const std::string& animalImage) {
        auto toolbarItem = dynamic_cast<Sprite*>(this->getChildren().at(3));
        if (toolbarItem) {
            toolbarItem->removeAllChildren();
            auto animalItem = Sprite::create(animalImage);
            if (animalItem) {
                animalItem->setScale(5.0f);
                animalItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                toolbarItem->addChild(animalItem, 4);
            }
        }
        // ���µ�ǰ��������
        if (updateAnimalTypeCallback) {
            Animal::AnimalType animalType;
            if (animalImage == "animalA1.png") {
                animalType = Animal::AnimalType::animalA;
            }
            else if (animalImage == "animalB.png") {
                animalType = Animal::AnimalType::animalB;
            }
            updateAnimalTypeCallback(animalType);
        }
        });
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

                    // ���������ǵ�һ�����߿���ʾ����ҳ��
                    if (i == 0)
                    {
                        auto mapScene = dynamic_cast<MapScene*>(this->getParent());
                        if (mapScene)
                        {
                            mapScene->toggleBackpackLayer();
                        }
                        _backpackSystem->showBackpackLayer(this);
                    }

					// ���������ǵ��ĸ����߿���ʾ���ﱳ��ҳ��
                    else if (i == 3)
                    {
                        auto mapScene = dynamic_cast<MapScene*>(this->getParent());
                        if (mapScene)
                        {
                            mapScene->toggleAnimalBackpackLayer();
                        }
                        _animalBackpackSystem->showBackpackLayer(this);
                    }
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
            // �ڵ��������߿��з��� fishing.png
            else if (i == 2)
            {
                auto fishingRodItem = Sprite::create("fishing.png");
                if (fishingRodItem != nullptr)
                {
                    fishingRodItem->setScale(5.0f); // ����ͼƬ��С
                    fishingRodItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                    toolbarItem->addChild(fishingRodItem, 4); // ȷ��ͼƬ�� z-order Ϊ 4
                }
            }
            //�ڵ��ĸ����߿��з���animalA.png
            else if (i == 3)
            {
                auto hookItem = Sprite::create("animalA1.png");
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

void Toolbar::setUpdateCropTypeCallback(const std::function<void(Crops::CropType)>& callback) {
    updateCropTypeCallback = callback;
}

void Toolbar::setUpdateAnimalTypeCallback(const std::function<void(Animal::AnimalType)>& callback) {
    updateAnimalTypeCallback = callback;
}
