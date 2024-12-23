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
    favorability(0), // ��ʼ���øж�
    isConfessMode(false), // ��ʼ����־����
    isTaskMode(false), // ��ʼ������ģʽ��־����
    taskCompleted(false), // ��ʼ��������ɱ�־����
    contentLabel(nullptr),
    favorabilityLabel(nullptr) // ��ʼ���øжȱ�ǩ
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
    visibleSize = Director::getInstance()->getVisibleSize(); // ��ȡ���ڵĴ�С
    loadFavorability(); // ���غøж�
    return true;
}

bool Dialog::addButton(const char* normalImage, const char* selectedImage, const char* button_title, int tag, Vec2 position)
{
    // ����ͼƬ�˵���ť
    MenuItemImage* menuImage = MenuItemImage::create(normalImage, selectedImage, this, menu_selector(Dialog::ButtonCallback));
    menuImage->setTag(tag);
    menuImage->setPosition(position);

    // �������˵��������λ��
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
    // ����ͼƬ�˵���ť
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
        (m_callbackListener->*m_callback)(node); // ���ûص�����
    }

    // �л�����һ���ı�
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

    // ����Ƿ��Ѿ����������� message ����
    if (!isConfessMode && !isTaskMode && currentContentIndex == 0) {
        favorability++; // ���Ӻøж�
        updateFavorability(); // ���ºøж���ʾ
        saveFavorability(); // ����øж�
    }
}

void Dialog::DeleteButtonCallback(Ref* sender)
{
    // ����øж�
    saveFavorability();
    // �Ƴ��Ի���
    this->removeFromParentAndCleanup(true);
}

void Dialog::ConfessButtonCallback(Ref* sender)
{
    isConfessMode = true;
    isTaskMode = false;
    if (favorability > 80) {
        // ��ʾ��׳ɹ����ı�
        m_confessContents = { "Confession successful!", "You are now in a relationship." };
    }
    else {
        // ��ʾ���ʧ�ܵ��ı�
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
        // ��ʾ������ɵ��ı�
        m_taskContents = { "Task completed!", "Favorability increased by 10." };
        favorability += 10; // ���Ӻøж�
        updateFavorability(); // ���ºøж���ʾ
        saveFavorability(); // ����øж�
        taskCompleted = false; // �����������״̬
    }
    else {
        // ��ʾ�������ݵ��ı�
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

    // ���ñ���ͼƬ�����ű���
    background->setScale(6.0f);

    // ���ñ���ͼƬ��λ�ã�ʹ��λ����ͼ���°벿��
    background->setPosition(visibleSize.width / 2, visibleSize.height / 4);
    addChild(background, 1);

    // ���öԻ���Ĵ�СΪ����ͼ��Ĵ�С����Ӧ�����ű���
    m_size = background->getContentSize() * 0.8f;

    for (const auto& node2 : node)
    {
        addChild(node2, 2);
    }
    if (getTitle()) {
        Label* title = Label::createWithSystemFont(getTitle(), "", getTitleSize());
        // ���ñ����λ�ã�ʹ��λ�ڱ���ͼƬ���Ϸ�
        title->setPosition(200, 280);
        addChild(title, 1);
    }
    if (getContentLabel()) {
        contentLabel = Label::createWithSystemFont(getContentLabel(), "", getContentFontSize());
        // �������ݵ�λ�ã�ʹ��λ�ڱ���ͼƬ���м�
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