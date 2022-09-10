#include "Classes/Scene/MainScene.h"
#include "Classes/Overall/WidelyUsed.h"
#include "Classes/Scene/SecondMainScene.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::extension;

//����MainScene����
Scene* MainScene::createScene()
{
    return MainScene::create();
}

//��ʼ��MainScene����
bool MainScene::init()
{
    if (!Scene::init()) { return false; }
   
    //��̬Ԫ�ز�
    initStillNode();
    //����Ԫ�ز�
    initMoveLayer();
    
    return true;
}

//��̬Ԫ�ز�
void MainScene::initStillNode()
{
    auto vSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto stillNode = Node::create();
    this->addChild(stillNode, 1);

    //�󱳾�
    auto theBackGround = Sprite::create("pic/TheBG.png");
    if (theBackGround == nullptr) {
        problemLoading("TheBG.png");
    }
    else {
        theBackGround->setScale(1.5);
        theBackGround->setPosition(Vec2(vSize.width / 2 + origin.x, vSize.height / 2 + origin.y));
        stillNode->addChild(theBackGround, 1);
    }
    
    //����ͼƬ
    auto mainBackGround = Sprite::create("pic/MSBackground.png");
    if (mainBackGround == nullptr) {
        problemLoading("MSBackground.png");
    }
    else {
        mainBackGround->setScale(2);
        mainBackGround->setPosition(Vec2(2 * vSize.width / 3 + origin.x, vSize.height / 2 + origin.y));
        stillNode->addChild(mainBackGround, 2);
    }

    //��������
    auto mainName = Sprite::create("pic/Title.png");
    if (mainName == nullptr) {
        problemLoading("Title.png");
    }
    else {
        mainName->setScale(1.5f);
        mainName->setPosition(Vec2(vSize.width / 2 + origin.x, 2 * vSize.height / 3 + origin.y));
        stillNode->addChild(mainName, 3);
    }

    //��������
    auto label = Label::createWithTTF("Eight-Figure Puzzle", "fonts/Marker Felt.ttf", 100);
    if (label == nullptr) {
        problemLoading("Marker Felt.ttf");
    }
    else {
        label->setColor(Color3B(0, 0, 0));
        label->setPosition(Vec2(origin.x + vSize.width / 2, origin.y + 5 * vSize.height / 9));
        stillNode->addChild(label, 4);
    }
}

//����Ԫ�ز�
void MainScene::initMoveLayer()
{
    auto vSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto stillLayer = Layer::create();
    this->addChild(stillLayer, 2);
    auto moveLayer = Layer::create();
    this->addChild(moveLayer, 3);
    moveLayer->setVisible(false);

    //�رհ�ť
    auto closeItem = MenuItemImage::create("pic/CloseNormal.png", "pic/CloseSelected.png",
        CC_CALLBACK_1(MainScene::gameCloseCallback, this));
    if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0) {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else {
        const float x = origin.x + vSize.width - closeItem->getContentSize().width / 2;
        const float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    //���ð�ť
    auto setupItem = MenuItemImage::create("pic/SetNormal.png", "pic/SetSelected.png",
        CC_CALLBACK_1(MainScene::getSetUpLayer, this, moveLayer));
    if (setupItem == nullptr || setupItem->getContentSize().width <= 0 || setupItem->getContentSize().height <= 0) {
        problemLoading("'SetNormal.png' and 'SetSelected.png'");
    }
    else {
        const float x = origin.x + setupItem->getContentSize().width / 2;
        const float y = origin.y + setupItem->getContentSize().height / 2;
        setupItem->setPosition(Vec2(x, y));
    }

    //��ʼ��ʾ��ť
    auto startGameItem = MenuItemImage::create("pic/MSStartGameNormal.png", "pic/MSStartGameSelected.png",
        CC_CALLBACK_1(MainScene::startGameCallBack, this));
    if (startGameItem == nullptr || startGameItem->getContentSize().width <= 0 || startGameItem->getContentSize().height <= 0) {
        problemLoading("'MSStartGameNormal.png' and 'MSStartGameSelected.png'");
    }
    else {
        const float x = origin.x + vSize.width / 2;
        const float y = origin.y + vSize.height * 0.2;
        startGameItem->setPosition(Vec2(x, y));
    }

    //��̬�˵�
    auto stillMenu = Menu::create(closeItem, setupItem, startGameItem, NULL);
    stillMenu->setPosition(Vec2::ZERO);
    stillLayer->addChild(stillMenu);

    //�����˵�
    initSecondMenu(moveLayer);
    moveLayer->runAction(ScaleTo::create(0.01, 0.01));
}

//�����˵�Ԫ��
void MainScene::initSecondMenu(Node* pNode)
{
    auto vSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/MainSceneBGM.mp3", true);
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(UserDefault::getInstance()->getFloatForKey("MusicVolume"));
    SimpleAudioEngine::getInstance()->setEffectsVolume(UserDefault::getInstance()->getFloatForKey("EffectVolume"));
    
    //��͸���ڵ�����
    auto menuBack = Sprite::create("pic/MenuBack.png");
    if (menuBack == nullptr) {
        problemLoading("pic/MenuBack.png");
    }
    else {
        menuBack->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height / 2);
        pNode->addChild(menuBack, 1);
    }

    //��̬�˵�����
    auto menuBackground = Sprite::create("pic/MenuBackground.png");
    if (menuBackground == nullptr) {
        problemLoading("MenuBackground.png");
    }
    else {
        menuBackground->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height / 2);
        pNode->addChild(menuBackground, 2);
    }

    //��̬�˵������ַ�
    auto menuBackGWord = Sprite::create("pic/MenuBackGWord.png");
    if (menuBackGWord == nullptr) {
        problemLoading("MenuBackGWord.png");
    }
    else {
        menuBackGWord->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height * 0.35);
        pNode->addChild(menuBackGWord, 3);
    }

    //��̬�˵��رհ�ť
    auto menuCloseItem = MenuItemImage::create("pic/MenuCloseNormal.png", "pic/MenuCloseSelected.png",
        CC_CALLBACK_1(MainScene::menuClose, this, pNode));
    if (menuCloseItem == nullptr || menuCloseItem->getContentSize().width <= 0 || menuCloseItem->getContentSize().height <= 0) {
        problemLoading("MenuCloseNormal.png and MenuCloseSelected.png");
    }
    else {
        menuCloseItem->setAnchorPoint(Vec2(1, 1));
        const float x = origin.x + vSize.width / 2 + menuBackground->getContentSize().width / 2;
        const float y = origin.y + vSize.height / 2 + menuBackground->getContentSize().height / 2;
        menuCloseItem->setPosition(Vec2(x, y));
    }

    //����������ť
    auto musicItem = Sprite::create("pic/Sound.png");
    if (musicItem == nullptr) {
        problemLoading("Sound.png");
    }
    else {
        musicItem->setScale(1.7f);
        const float x = origin.x + vSize.width * 0.32;
        const float y = origin.y + vSize.height * 0.65;
        musicItem->setPosition(Vec2(x, y));
        pNode->addChild(musicItem, 5);
    }

    //���ֿ��ƻ���
    auto musicSlider = ControlSlider::create("pic/SliderNormal.png", "pic/SliderSelected.png", "pic/SliderButton.png");
    musicSlider->setPosition(Vec2(origin.x + vSize.width * 0.52, origin.y + vSize.height * 0.65));
    musicSlider->setMinimumValue(0);
    musicSlider->setMaximumValue(100);
    const auto tempMV = UserDefault::getInstance()->getFloatForKey("MusicVolume") * 100;
    musicSlider->setValue(tempMV);
    musicSlider->addTargetWithActionForControlEvents(this,
        cccontrol_selector(MainScene::musicSliderCallBack), Control::EventType::VALUE_CHANGED);
    pNode->addChild(musicSlider, 4);

    //��Ч������ť
    auto effectItem = Sprite::create("pic/Effect.png");
    if (effectItem == nullptr) {
        problemLoading("Effect.png");
    }
    else {
        effectItem->setScale(1.7f);
        const float x = origin.x + vSize.width * 0.32;
        const float y = origin.y + vSize.height * 0.52;
        effectItem->setPosition(Vec2(x, y));
        pNode->addChild(effectItem, 7);
    }

    //��Ч���ƻ���
    auto effectSlider = ControlSlider::create("pic/SliderNormal.png", "pic/SliderSelected.png", "pic/SliderButton.png");
    effectSlider->setPosition(Vec2(origin.x + vSize.width * 0.52, origin.y + vSize.height * 0.52));
    effectSlider->setMinimumValue(0);
    effectSlider->setMaximumValue(100);
    const auto tempEV = UserDefault::getInstance()->getFloatForKey("EffectVolume") * 100;
    effectSlider->setValue(tempEV);
    effectSlider->addTargetWithActionForControlEvents(this,
        cccontrol_selector(MainScene::effectSliderCallBack), Control::EventType::VALUE_CHANGED);
    pNode->addChild(effectSlider, 6);

    //�����˵�
    auto moveMenu = Menu::create(menuCloseItem, NULL);
    moveMenu->setPosition(Vec2::ZERO);
    pNode->addChild(moveMenu, 3);
}

//���ֻ���Ч��
void MainScene::musicSliderCallBack(Ref* pSender, Control::EventType type)
{
    auto slider = (ControlSlider*)pSender;
    auto value = slider->getValue();
    value = value / 100;
    SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
    UserDefault::getInstance()->setFloatForKey("MusicVolume", value);
}

//��Ч����Ч��
void MainScene::effectSliderCallBack(Ref* pSender, Control::EventType type)
{
    auto slider = (ControlSlider*)pSender;
    auto value = slider->getValue();
    value = value / 100;
    SimpleAudioEngine::getInstance()->setEffectsVolume(value);
    UserDefault::getInstance()->setFloatForKey("EffectVolume", value);
}

//������رղ���
void MainScene::gameCloseCallback(Ref* pSender)
{
    UserDefault::getInstance()->setFloatForKey("MusicVolume", 0.5);
    UserDefault::getInstance()->setFloatForKey("EffectVolume", 0.5);
    Director::getInstance()->end();
}

//��������������˵�
void MainScene::getSetUpLayer(Ref* pSender, Node* pNode)
{
    pNode->runAction(ScaleTo::create(0.01, 1));
    pNode->setVisible(true);
}

//������رն����˵�
void MainScene::menuClose(Ref* pSender, Node* pNode)
{
    pNode->setVisible(false);
    pNode->runAction(ScaleTo::create(0.01, 0.01));
}

//��ʼ��Ϸ�л�����
void MainScene::startGameCallBack(Ref* pSender)
{
    auto scene = SecondMainScene::createScene();
    Director::getInstance()->replaceScene(scene);
}