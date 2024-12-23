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

    void addDeleteButton(const char* normalImage, const char* selectedImage, Vec2 position); // 添加删除按钮的函数
    void addConfessButton(const char* normalImage, const char* selectedImage, Vec2 position); // 添加表白按钮的函数
    void addTaskButton(const char* normalImage, const char* selectedImage, Vec2 position); // 添加任务按钮的函数

    void setTaskCompleted(bool completed); // 设置任务完成状态

protected:
    virtual void onEnter() override;
    virtual void onExit() override;

private:
    void ButtonCallback(Ref* sender);
    void updateContent();
    void DeleteButtonCallback(Ref* sender); // 添加删除按钮的回调函数
    void ConfessButtonCallback(Ref* sender); // 添加表白按钮的回调函数
    void TaskButtonCallback(Ref* sender); // 添加任务按钮的回调函数
    void updateFavorability(); // 添加更新好感度的函数
    void saveFavorability(); // 添加保存好感度的函数
    void loadFavorability(); // 添加加载好感度的函数

    const char* m_title;
    std::vector<std::string> m_contents;
    std::vector<std::string> m_confessContents; // 添加表白结果的文本内容
    std::vector<std::string> m_taskContents; // 添加任务文本内容
    int m_content_fontSize;
    int m_title_fontSize;
    int currentContentIndex;
    int favorability; // 添加好感度变量
    bool isConfessMode; // 添加标志变量
    bool isTaskMode; // 添加任务模式标志变量
    bool taskCompleted; // 添加任务完成标志变量
    Size visibleSize; // 添加此行
    SEL_CallFuncN m_callback;
    CCObject* m_callbackListener;
    Size m_size;
    Sprite* background;
    Menu* menu;
    Vector<Node*> node;
    Size visiableSize;
    const char* backgroundImage;
    Label* contentLabel;
    Label* favorabilityLabel; // 添加好感度标签
};

#endif
