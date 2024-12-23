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

    // 在 Toolbar::init() 方法中初始化 _backpackSystem
    _backpackSystem = BackpackSystem::create();
    this->addChild(_backpackSystem, 4); // 确保背包系统的 z-order 为 4

    createToolbar();
    // 设置更新工具栏的回调函数
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
        // 更新当前作物类型
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

    // 在 Toolbar::init() 方法中初始化 _animalBackpackSystem
    _animalBackpackSystem = AnimalBackpack::create();
    this->addChild(_animalBackpackSystem, 4); // 确保动物背包系统的 z-order 为 4

    // 设置更新工具栏的回调函数
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
        // 更新当前动物类型
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

                    // 如果点击的是第一个工具框，显示背包页面
                    if (i == 0)
                    {
                        auto mapScene = dynamic_cast<MapScene*>(this->getParent());
                        if (mapScene)
                        {
                            mapScene->toggleBackpackLayer();
                        }
                        _backpackSystem->showBackpackLayer(this);
                    }

					// 如果点击的是第四个工具框，显示动物背包页面
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
            // 在第三个工具框中放入 fishing.png
            else if (i == 2)
            {
                auto fishingRodItem = Sprite::create("fishing.png");
                if (fishingRodItem != nullptr)
                {
                    fishingRodItem->setScale(5.0f); // 调整图片大小
                    fishingRodItem->setPosition(Vec2(toolbarItem->getContentSize().width / 2, toolbarItem->getContentSize().height / 2));
                    toolbarItem->addChild(fishingRodItem, 4); // 确保图片的 z-order 为 4
                }
            }
            //在第四个工具框中放入animalA.png
            else if (i == 3)
            {
                auto hookItem = Sprite::create("animalA1.png");
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

void Toolbar::setUpdateCropTypeCallback(const std::function<void(Crops::CropType)>& callback) {
    updateCropTypeCallback = callback;
}

void Toolbar::setUpdateAnimalTypeCallback(const std::function<void(Animal::AnimalType)>& callback) {
    updateAnimalTypeCallback = callback;
}
