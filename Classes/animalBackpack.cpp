#include "AnimalBackpack.h"
#include "ui/CocosGUI.h"
#include "animal.h"

USING_NS_CC;
AnimalBackpack::AnimalBackpack() : backpackLayer(nullptr), currentAction(""), _isBackpackOpen(false) {}

AnimalBackpack::~AnimalBackpack() {}
AnimalBackpack* AnimalBackpack::create()
{
    AnimalBackpack* backpack = new (std::nothrow) AnimalBackpack();
    if (backpack && backpack->init())
    {
        backpack->autorelease();
        return backpack;
    }
    CC_SAFE_DELETE(backpack);
    return nullptr;
}

void AnimalBackpack::initBackpackLayer() {
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const float layerWidth = visibleSize.width / 2;
    const float layerHeight = visibleSize.height / 2;

    // 使用图片作为背包层的背景
    backpackLayer = Sprite::create("Setting menu.png");
    backpackLayer->setContentSize(Size(layerWidth, layerHeight));
    backpackLayer->setPosition(Vec2(visibleSize.width / 4 + layerWidth / 2, visibleSize.height / 4 + layerHeight / 2));

    // 绘制黑色的线分割为 5x5 的格子
    DrawNode* drawNode = DrawNode::create();
    const float cellWidth = (layerWidth - 5) / 5;
    const float cellHeight = (layerHeight - 5) / 5;

    const Color4F brown(0.65f, 0.16f, 0.16f, 1.0f); // 定义棕色

    for (int i = 1; i < 5; ++i) {
        drawNode->drawLine(Vec2(i * cellWidth, 0), Vec2(i * cellWidth, layerHeight), brown);
        drawNode->drawLine(Vec2(0, i * cellHeight), Vec2(layerWidth, i * cellHeight), brown);
    }
    backpackLayer->addChild(drawNode);

    addBackpackItems();
}

void AnimalBackpack::addBackpackItems()
{
    const float itemSize = 50.0f;
    const float spacing = 43.0f; // 调整水平间距
    const std::vector<std::string> cropImages = { "animalA1.png", "animalB.png" };

    for (size_t i = 0; i < cropImages.size(); ++i) {
        auto item = Sprite::create(cropImages[i]);
        item->setScale(itemSize / item->getContentSize().width);
        float xPosition = (i % 5) * (itemSize + spacing) + itemSize / 2 + spacing;
        float yPosition = (i / 5) * (itemSize + (itemSize / 2)) + itemSize / 2 + 18;
        item->setPosition(Vec2(xPosition, yPosition));
        backpackLayer->addChild(item);

        auto listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = [this, item, cropImages, i](Touch* touch, Event* event) {
            Vec2 locationInNode = item->convertToNodeSpace(touch->getLocation());
            Size size = item->getContentSize();
            Rect rect = Rect(0, 0, size.width, size.height);

            if (rect.containsPoint(locationInNode)) {
                selectItem(cropImages[i]);
                return true;
            }
            return false;
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, item);
    }
}

void AnimalBackpack::setUpdateToolbarCallback(const std::function<void(const std::string&)>& callback)
{
    _updateToolbarCallback = callback;
}

void AnimalBackpack::setSelectAnimalCallback(const std::function<void(const std::string&, Animal::AnimalType)>& callback)
{
    _selectAnimalCallback = callback;
}

void AnimalBackpack::showBackpackLayer(cocos2d::Node* parent)
{
    if (!backpackLayer) {
        initBackpackLayer();
    }
    parent->addChild(backpackLayer, 4); // 确保背包层的 z-order 为 4
    _isBackpackOpen = true; // 更新标志为打开状态
}

void AnimalBackpack::hideBackpackLayer()
{
    if (backpackLayer) {
        backpackLayer->removeFromParent();
        backpackLayer = nullptr; // 将 backpackLayer 置为 nullptr
    }
    _isBackpackOpen = false; // 更新标志为关闭状态
}

bool AnimalBackpack::isBackpackOpen() const
{
    return _isBackpackOpen;
}

void AnimalBackpack::selectItem(const std::string& itemName) {
    currentAction = itemName;
    Animal::AnimalType animalType;
    if (itemName == "animalA1.png") {
        animalType = Animal::AnimalType::animalA;
    }
    else if (itemName == "animalB.png") {
        animalType = Animal::AnimalType::animalB;
    }
    if (_selectAnimalCallback) {
        _selectAnimalCallback(itemName, animalType);
    }
    if (_updateToolbarCallback) {
        _updateToolbarCallback(itemName);
    }

    hideBackpackLayer();
}