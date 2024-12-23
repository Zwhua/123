#include "talk.h"
#include "cocos2d.h"

USING_NS_CC;

Dialog::Dialog() :
    m_title(nullptr),
    m_content_fontSize(0),
    m_title_fontSize(0),
    m_callback(nullptr),
    m_callbackListener(nullptr),
    m_size(0, 0),
    backgroundImage(nullptr),
    currentContentIndex(0),
    favorability(0), // 初始化好感度
    isConfessMode(false), // 初始化标志变量
    isTaskMode(false), // 初始化任务模式标志变量
    taskCompleted(false), // 初始化任务完成标志变量
    contentLabel(nullptr),
    favorabilityLabel(nullptr) // 初始化好感度标签
{
}

Dialog::~Dialog()
{
    CC_SAFE_RELEASE(background);
    CC_SAFE_RELEASE(menu);
}

Scene* Dialog::createScene()
{
    auto scene = Scene::create();
    auto layer = Dialog::create();
    scene->addChild(layer);
    return scene;
}

bool Dialog::init()
{
    if (!Layer::init()) {
        return false;
    }
    menu = Menu::create();
    node = Vector<Node*>();
    visibleSize = Director::getInstance()->getVisibleSize(); // 获取窗口的大小
    loadFavorability(); // 加载好感度
    return true;
}

bool Dialog::addButton(const char* normalImage, const char* selectedImage, const char* button_title, int tag, Vec2 position)
{
    // 创建图片菜单按钮
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, this, menu_selector(Dialog::ButtonCallback));
    menuImage->setTag(tag);
    menuImage->setPosition(position);

    // 添加文字说明并设置位置
    CCSize imenu = menuImage->getContentSize();
    CCLabelTTF* ttf = CCLabelTTF::create(button_title, "", 20);
    ttf->setColor(ccc3(0, 0, 0));
    ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
    menuImage->addChild(ttf);
    Menu* menu = Menu::create(menuImage, NULL);
    node.pushBack(menu);
    return true;
}

bool Dialog::addButton(const char* normalImage, const char* selectedImage, int tag, Vec2 position)
{
    // 创建图片菜单按钮
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, this, menu_selector(Dialog::ButtonCallback));
    menuImage->setTag(tag);
    menuImage->setScale(3.0f);
    Menu* menu = Menu::create(menuImage, NULL);
    menu->setPosition(position);
    node.pushBack(menu);
    return true;
}

void Dialog::setCallbackFunc(CCObject* target, SEL_CallFuncN callfun)
{
    m_callbackListener = target;
    m_callback = callfun;
}

void Dialog::setTitle(const char* titles, int fontSize)
{
    m_title = titles;
    m_title_fontSize = fontSize;
}

const char* Dialog::getTitle() const
{
    return m_title;
}

int Dialog::getTitleSize() const
{
    return m_title_fontSize;
}

void Dialog::setContent(const std::vector<std::string>& contents, int fontSize)
{
    m_contents = contents;
    m_content_fontSize = fontSize;
}

const char* Dialog::getContentLabel() const
{
    if (isConfessMode) {
        if (currentContentIndex < m_confessContents.size()) {
            return m_confessContents[currentContentIndex].c_str();
        }
    }
    else if (isTaskMode) {
        if (currentContentIndex < m_taskContents.size()) {
            return m_taskContents[currentContentIndex].c_str();
        }
    }
    else {
        if (currentContentIndex < m_contents.size()) {
            return m_contents[currentContentIndex].c_str();
        }
    }
    return nullptr;
}

int Dialog::getContentFontSize() const
{
    return m_content_fontSize;
}

void Dialog::setBackground(const char* backgroundImage)
{
    this->backgroundImage = backgroundImage;
}

void Dialog::ButtonCallback(Ref* sender)
{
    Node* node = dynamic_cast<Node*>(sender);
    if (m_callback && m_callbackListener) {
        (m_callbackListener->*m_callback)(node); // 设置回调函数
    }

    // 切换到下一个文本
    if (isConfessMode && !m_confessContents.empty()) {
        currentContentIndex = (currentContentIndex + 1) % m_confessContents.size();
    }
    else if (isTaskMode && !m_taskContents.empty()) {
        currentContentIndex = (currentContentIndex + 1) % m_taskContents.size();
    }
    else if (!m_contents.empty()) {
        currentContentIndex = (currentContentIndex + 1) % m_contents.size();
    }

    updateContent();

    // 检查是否已经运行完整个 message 向量
    if (!isConfessMode && !isTaskMode && currentContentIndex == 0) {
        favorability++; // 增加好感度
        updateFavorability(); // 更新好感度显示
        saveFavorability(); // 保存好感度
    }
}

void Dialog::DeleteButtonCallback(Ref* sender)
{
    // 保存好感度
    saveFavorability();
    // 移除对话框
    this->removeFromParentAndCleanup(true);
}

void Dialog::ConfessButtonCallback(Ref* sender)
{
    isConfessMode = true;
    isTaskMode = false;
    if (favorability > 80) {
        // 显示表白成功的文本
        m_confessContents = { "Confession successful!", "You are now in a relationship." };
    }
    else {
        // 显示表白失败的文本
        m_confessContents = { "Confession failed.", "Your favorability is too low." };
    }
    currentContentIndex = 0;
    updateContent();
}

void Dialog::TaskButtonCallback(Ref* sender)
{
    isTaskMode = true;
    isConfessMode = false;
    if (taskCompleted) {
        // 显示任务完成的文本
        m_taskContents = { "Task completed!", "Favorability increased by 10." };
        favorability += 10; // 增加好感度
        updateFavorability(); // 更新好感度显示
        saveFavorability(); // 保存好感度
        taskCompleted = false; // 重置任务完成状态
    }
    else {
        // 显示任务内容的文本
        m_taskContents = { "Task: Go to the location (400, 400)." };
    }
    currentContentIndex = 0;
    updateContent();
}

void Dialog::addDeleteButton(const char* normalImage, const char* selectedImage, Vec2 position)
{
    auto deleteButton = MenuItemImage::create(normalImage, selectedImage, CC_CALLBACK_1(Dialog::DeleteButtonCallback, this));
    deleteButton->setPosition(position);
    deleteButton->setScale(3.0f);
    auto deleteMenu = Menu::create(deleteButton, NULL);
    deleteMenu->setPosition(Vec2::ZERO);
    this->addChild(deleteMenu, 2);
}

void Dialog::addConfessButton(const char* normalImage, const char* selectedImage, Vec2 position)
{
    auto confessButton = MenuItemImage::create(normalImage, selectedImage, CC_CALLBACK_1(Dialog::ConfessButtonCallback, this));
    confessButton->setPosition(position);
    confessButton->setScale(3.0f);
    auto confessMenu = Menu::create(confessButton, NULL);
    confessMenu->setPosition(Vec2::ZERO);
    this->addChild(confessMenu, 2);
}

void Dialog::addTaskButton(const char* normalImage, const char* selectedImage, Vec2 position)
{
    auto taskButton = MenuItemImage::create(normalImage, selectedImage, CC_CALLBACK_1(Dialog::TaskButtonCallback, this));
    taskButton->setPosition(position);
    taskButton->setScale(3.0f);
    auto taskMenu = Menu::create(taskButton, NULL);
    taskMenu->setPosition(Vec2::ZERO);
    this->addChild(taskMenu, 2);
}

void Dialog::setTaskCompleted(bool completed)
{
    taskCompleted = completed;
}

void Dialog::updateContent()
{
    if (contentLabel) {
        contentLabel->setString(getContentLabel());
    }
}

void Dialog::updateFavorability()
{
    if (!favorabilityLabel) {
        favorabilityLabel = Label::createWithSystemFont("", "", 28);
        favorabilityLabel->setPosition(visibleSize.width - 160, visibleSize.height / 4 + 80);
        this->addChild(favorabilityLabel, 1);
    }
    favorabilityLabel->setString("Favorability: " + std::to_string(favorability));
}

void Dialog::saveFavorability()
{
    UserDefault::getInstance()->setIntegerForKey("favorability", favorability);
    UserDefault::getInstance()->flush();
}

void Dialog::loadFavorability()
{
    favorability = UserDefault::getInstance()->getIntegerForKey("favorability", 0);
}

void Dialog::onEnter()
{
    Layer::onEnter();
    background = Sprite::create("textBox.png");

    // 设置背景图片的缩放比例
    background->setScale(6.0f);

    // 设置背景图片的位置，使其位于视图的下半部分
    background->setPosition(visibleSize.width / 2, visibleSize.height / 4);
    addChild(background, 1);

    // 设置对话框的大小为背景图像的大小，并应用缩放比例
    m_size = background->getContentSize() * 0.8f;

    for (const auto& node2 : node)
    {
        addChild(node2, 2);
    }
    if (getTitle()) {
        Label* title = Label::createWithSystemFont(getTitle(), "", getTitleSize());
        // 设置标题的位置，使其位于背景图片的上方
        title->setPosition(200, 280);
        addChild(title, 1);
    }
    if (getContentLabel()) {
        contentLabel = Label::createWithSystemFont(getContentLabel(), "", getContentFontSize());
        // 设置内容的位置，使其位于背景图片的中间
        contentLabel->setPosition(visibleSize.width / 2, visibleSize.height / 4);
        addChild(contentLabel, 1);
    }

    FiniteTimeAction* action = Sequence::create(
        ScaleTo::create(0.2, 0.2),
        ScaleTo::create(0.4, 0.4),
        ScaleTo::create(0.6, 0.6),
        ScaleTo::create(0.8, 0.8), NULL);
    this->runAction(action);
}

void Dialog::onExit()
{
    Layer::onExit();
}