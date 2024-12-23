#ifndef _Dialog_Layer_
#define _Dialog_Layer_

#include "cocos2d.h"

USING_NS_CC;

class Dialog : public Layer
{
public:
    Dialog();
    virtual ~Dialog();

    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(Dialog);

    bool addButton(const char* normalImage, const char* selectedImage, const char* button_title, int tag, Vec2 position);
    bool addButton(const char* normalImage, const char* selectedImage, int tag, Vec2 position);

    void setCallbackFunc(CCObject* target, SEL_CallFuncN callfun);

    void setTitle(const char* titles, int fontSize);
    const char* getTitle() const;
    int getTitleSize() const;

    void setContent(const std::vector<std::string>& contents, int fontSize);
    const char* getContentLabel() const;
    int getContentFontSize() const;

    void setBackground(const char* backgroundImage);

    void addDeleteButton(const char* normalImage, const char* selectedImage, Vec2 position); // ���ɾ����ť�ĺ���
    void addConfessButton(const char* normalImage, const char* selectedImage, Vec2 position); // ��ӱ�װ�ť�ĺ���
    void addTaskButton(const char* normalImage, const char* selectedImage, Vec2 position); // �������ť�ĺ���

    void setTaskCompleted(bool completed); // �����������״̬

protected:
    virtual void onEnter() override;
    virtual void onExit() override;

private:
    void ButtonCallback(Ref* sender);
    void updateContent();
    void DeleteButtonCallback(Ref* sender); // ���ɾ����ť�Ļص�����
    void ConfessButtonCallback(Ref* sender); // ��ӱ�װ�ť�Ļص�����
    void TaskButtonCallback(Ref* sender); // �������ť�Ļص�����
    void updateFavorability(); // ��Ӹ��ºøжȵĺ���
    void saveFavorability(); // ��ӱ���øжȵĺ���
    void loadFavorability(); // ��Ӽ��غøжȵĺ���

    const char* m_title;
    std::vector<std::string> m_contents;
    std::vector<std::string> m_confessContents; // ��ӱ�׽�����ı�����
    std::vector<std::string> m_taskContents; // ��������ı�����
    int m_content_fontSize;
    int m_title_fontSize;
    int currentContentIndex;
    int favorability; // ��Ӻøжȱ���
    bool isConfessMode; // ��ӱ�־����
    bool isTaskMode; // �������ģʽ��־����
    bool taskCompleted; // ���������ɱ�־����
    Size visibleSize; // ��Ӵ���
    SEL_CallFuncN m_callback;
    CCObject* m_callbackListener;
    Size m_size;
    Sprite* background;
    Menu* menu;
    Vector<Node*> node;
    Size visiableSize;
    const char* backgroundImage;
    Label* contentLabel;
    Label* favorabilityLabel; // ��Ӻøжȱ�ǩ
};

#endif
