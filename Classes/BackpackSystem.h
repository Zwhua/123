#ifndef BACKPACK_SYSTEM_H
#define BACKPACK_SYSTEM_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "crops.h"

class BackpackSystem : public cocos2d::Layer {
public:
    // 构造函数
    BackpackSystem();
    ~BackpackSystem();

    // 创建静态实例
    static BackpackSystem* create();

    // 显示背包页面
    void showBackpackLayer(cocos2d::Node* parent);

    // 隐藏背包页面
    void hideBackpackLayer();

    // 选择物品
    void selectItem(const std::string& itemName);

    // 获取当前选择的行为
    const std::string& getCurrentAction() const;

    // 设置选择作物的回调函数
    void setSelectCropCallback(const std::function<void(const std::string&, Crops::CropType)>& callback);

    //
    void setUpdateToolbarCallback(const std::function<void(const std::string&)>& callback);

    bool isBackpackOpen() const;
private:
    // 初始化背包层
    void initBackpackLayer();

    // 添加背包物品到层
    void addBackpackItems();

    // 背包层和当前动作
    cocos2d::Sprite* backpackLayer; // 修改类型为 Sprite*
    std::string currentAction;

    // 选择作物的回调函数
    std::function<void(const std::string&, Crops::CropType)> selectCropCallback;

    std::function<void(const std::string&)> updateToolbarCallback;

    bool _isBackpackOpen;
};

#endif // BACKPACK_SYSTEM_H
