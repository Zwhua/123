#include "BackpackSystem.h"
#include "crops.h"
USING_NS_CC;

BackpackSystem::BackpackSystem() : backpackLayer(nullptr), currentAction(""), _isBackpackOpen(false) {}

BackpackSystem::~BackpackSystem() {}

BackpackSystem* BackpackSystem::create() {
    BackpackSystem* ret = new (std::nothrow) BackpackSystem();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

void BackpackSystem::showBackpackLayer(cocos2d::Node* parent) {
    if (!backpackLayer) {
        initBackpackLayer();
    }
    parent->addChild(backpackLayer, 4); // ȷ��������� z-order Ϊ 4
    _isBackpackOpen = true; // ���±�־Ϊ��״̬
}

void BackpackSystem::hideBackpackLayer() {
    if (backpackLayer) {
        backpackLayer->removeFromParent();
        backpackLayer = nullptr; // �� backpackLayer ��Ϊ nullptr
    }
    _isBackpackOpen = false; // ���±�־Ϊ�ر�״̬
}


bool BackpackSystem::isBackpackOpen() const {
    return _isBackpackOpen;
}

const std::string& BackpackSystem::getCurrentAction() const {
    return currentAction;
}

void BackpackSystem::setSelectCropCallback(const std::function<void(const std::string&, Crops::CropType)>& callback) {
    selectCropCallback = callback;
}
void BackpackSystem::setUpdateToolbarCallback(const std::function<void(const std::string&)>& callback) {
    updateToolbarCallback = callback;
}

void BackpackSystem::selectItem(const std::string& itemName) {
    currentAction = itemName;
    Crops::CropType cropType;
    if (itemName == "pumpkin4.png") {
        cropType = Crops::CropType::pumpkin;
    }
    else if (itemName == "cropA4.png") {
        cropType = Crops::CropType::cropA;
    }
    else if (itemName == "cropB4.png") {
        cropType = Crops::CropType::cropB;
    }
    else if (itemName == "cropC4.png") {
        cropType = Crops::CropType::cropC;
    }
    else if (itemName == "cropD4.png") {
        cropType = Crops::CropType::cropD;
    }
    else if (itemName == "cropE4.png") {
        cropType = Crops::CropType::cropE;
    }

    if (selectCropCallback) {
        selectCropCallback(itemName, cropType);
    }
    if (updateToolbarCallback) {
        updateToolbarCallback(itemName);
    }

    hideBackpackLayer();
}

void BackpackSystem::initBackpackLayer() {
    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const float layerWidth = visibleSize.width / 2;
    const float layerHeight = visibleSize.height / 2;

    // ʹ��ͼƬ��Ϊ������ı���
    backpackLayer = Sprite::create("Setting menu.png");
    backpackLayer->setContentSize(Size(layerWidth, layerHeight));
    backpackLayer->setPosition(Vec2(visibleSize.width / 4 + layerWidth / 2, visibleSize.height / 4 + layerHeight / 2));

    // ���ƺ�ɫ���߷ָ�Ϊ 5x5 �ĸ���
    DrawNode* drawNode = DrawNode::create();
    const float cellWidth = (layerWidth - 5) / 5;
    const float cellHeight = (layerHeight - 5) / 5;

    const Color4F brown(0.65f, 0.16f, 0.16f, 1.0f); // ������ɫ

    for (int i = 1; i < 5; ++i) {
        drawNode->drawLine(Vec2(i * cellWidth, 0), Vec2(i * cellWidth, layerHeight), brown);
        drawNode->drawLine(Vec2(0, i * cellHeight), Vec2(layerWidth, i * cellHeight), brown);
    }
    backpackLayer->addChild(drawNode);

    addBackpackItems();
}

void BackpackSystem::addBackpackItems()
{
    const float itemSize = 50.0f;
    const float spacing = 43.0f; // ����ˮƽ���
    const std::vector<std::string> cropImages = { "pumpkin4.png", "cropA4.png", "cropB4.png", "cropC4.png", "cropD4.png", "cropE4.png" };

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
