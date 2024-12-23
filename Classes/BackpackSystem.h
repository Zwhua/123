#ifndef BACKPACK_SYSTEM_H
#define BACKPACK_SYSTEM_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "crops.h"

class BackpackSystem : public cocos2d::Layer {
public:
    // ���캯��
    BackpackSystem();
    ~BackpackSystem();

    // ������̬ʵ��
    static BackpackSystem* create();

    // ��ʾ����ҳ��
    void showBackpackLayer(cocos2d::Node* parent);

    // ���ر���ҳ��
    void hideBackpackLayer();

    // ѡ����Ʒ
    void selectItem(const std::string& itemName);

    // ��ȡ��ǰѡ�����Ϊ
    const std::string& getCurrentAction() const;

    // ����ѡ������Ļص�����
    void setSelectCropCallback(const std::function<void(const std::string&, Crops::CropType)>& callback);

    //
    void setUpdateToolbarCallback(const std::function<void(const std::string&)>& callback);

    bool isBackpackOpen() const;
private:
    // ��ʼ��������
    void initBackpackLayer();

    // ��ӱ�����Ʒ����
    void addBackpackItems();

    // ������͵�ǰ����
    cocos2d::Sprite* backpackLayer; // �޸�����Ϊ Sprite*
    std::string currentAction;

    // ѡ������Ļص�����
    std::function<void(const std::string&, Crops::CropType)> selectCropCallback;

    std::function<void(const std::string&)> updateToolbarCallback;

    bool _isBackpackOpen;
};

#endif // BACKPACK_SYSTEM_H
