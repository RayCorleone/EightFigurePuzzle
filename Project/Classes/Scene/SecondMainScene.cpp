#include "Classes/Overall/WidelyUsed.h"
#include "Classes/Scene/MainScene.h"
#include "Classes/Scene/SecondMainScene.h"
#include "Classes/Scene/RandomTypeScene.h"
#include "Classes/Scene/SelfTypeScene.h"
#include "Classes/Scene/AdvanceTypeScene.h"
#include "SimpleAudioEngine.h"
#include "cocos-ext.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::extension;

Scene* SecondMainScene::createScene()
{
	return SecondMainScene::create();
}

bool SecondMainScene::init()
{
	if (!Scene::init()) { return false; }

	//静态元素层
	initStillNode();
	//交互元素层
	initMoveLayer();

	return true;
}

//静态元素层
void SecondMainScene::initStillNode()
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto stillNode = Node::create();
	this->addChild(stillNode, 1);

	//大背景
	auto theBackGround = Sprite::create("pic/TheBG.png");
	if (theBackGround == nullptr) {
		problemLoading("TheBG.png");
	}
	else {
		theBackGround->setScale(1.5);
		theBackGround->setPosition(Vec2(vSize.width / 2 + origin.x, vSize.height / 2 + origin.y));
		stillNode->addChild(theBackGround, 1);
	}

	//背景
	auto backGround = Sprite::create("pic/SMSBackground.png");
	if (backGround == nullptr) {
		problemLoading("SMSBackground.png");
	}
	else {
		backGround->setScale(vSize.width / backGround->getContentSize().width);
		backGround->setPosition(Vec2(vSize.width / 2 + origin.x, vSize.height / 2 + origin.y));
		stillNode->addChild(backGround, 2);
	}

	this->scheduleUpdate();
}

//交互元素层
void SecondMainScene::initMoveLayer()
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto stillLayer = Layer::create();
	this->addChild(stillLayer, 2);
	auto moveLayer = Layer::create();
	this->addChild(moveLayer, 3);
	moveLayer->setVisible(false);
	auto introLayer = Layer::create();
	this->addChild(introLayer, 4);
	introLayer->setVisible(false);

	//选择随机模式
	auto randomItem = MenuItemImage::create("pic/RandomNormal.png", "pic/RandomSelected.png",
		CC_CALLBACK_1(SecondMainScene::randomMoodCallBack, this));
	if (randomItem == nullptr || randomItem->getContentSize().width <= 0 || randomItem->getContentSize().height <= 0) {
		problemLoading("'RandomNormal.png' and 'RandomSelected.png'");
	}
	else {
		const float x = vSize.width * 0.2;
		const float y = vSize.height * 0.53;
		randomItem->setScale(1.3f);
		randomItem->setPosition(Vec2(x, y));
	}

	//选择自主模式
	auto selfItem = MenuItemImage::create("pic/SelfNormal.png", "pic/SelfSelected.png",
		CC_CALLBACK_1(SecondMainScene::selfMoodCallBack, this));
	if (selfItem == nullptr || selfItem->getContentSize().width <= 0 || selfItem->getContentSize().height <= 0) {
		problemLoading("'SelfNormal.png' and 'SelfSelected.png'");
	}
	else {
		const float x = vSize.width * 0.5;
		const float y = vSize.height * 0.53;
		selfItem->setScale(1.3f);
		selfItem->setPosition(Vec2(x, y));
	}

	//选择高级模式
	auto advanceItem = MenuItemImage::create("pic/AdvanceNormal.png", "pic/AdvanceSelected.png",
		CC_CALLBACK_1(SecondMainScene::advanceMoodCallBack, this));
	if (advanceItem == nullptr || advanceItem->getContentSize().width <= 0 || advanceItem->getContentSize().height <= 0) {
		problemLoading("'AdvanceNormal.png' and 'AdvanceSelected.png'");
	}
	else {
		const float x = vSize.width * 0.8;
		const float y = vSize.height * 0.53;
		advanceItem->setScale(1.3f);
		advanceItem->setPosition(Vec2(x, y));
	}

	//关闭按钮
	auto closeItem = MenuItemImage::create("pic/CloseNormal.png", "pic/CloseSelected.png",
		CC_CALLBACK_1(SecondMainScene::gameCloseCallback, this));
	if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0) {
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else {
		const float x = origin.x + vSize.width - closeItem->getContentSize().width / 2;
		const float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	//设置按钮
	auto setupItem = MenuItemImage::create("pic/SetNormal.png", "pic/SetSelected.png",
		CC_CALLBACK_1(SecondMainScene::getSetUpLayer, this, moveLayer));
	if (setupItem == nullptr || setupItem->getContentSize().width <= 0 || setupItem->getContentSize().height <= 0) {
		problemLoading("'SetNormal.png' and 'SetSelected.png'");
	}
	else {
		const float x = origin.x + setupItem->getContentSize().width / 2;
		const float y = origin.y + setupItem->getContentSize().height / 2;
		setupItem->setPosition(Vec2(x, y));
	}

	//介绍按钮
	auto introItem = MenuItemImage::create("pic/IntroNormal.png", "pic/IntroSelected.png",
		CC_CALLBACK_1(SecondMainScene::getSetUpLayer, this, introLayer));
	if (introItem == nullptr || introItem->getContentSize().width <= 0 || introItem->getContentSize().height <= 0) {
		problemLoading("'IntroNormal.png' and 'IntroSelected.png'");
	}
	else {
		const float x = origin.x + vSize.width - introItem->getContentSize().width;
		const float y = origin.y + vSize.height - introItem->getContentSize().height / 1.68;
		introItem->setScale(1.25f);
		introItem->setPosition(Vec2(x, y));
	}

	auto menu = Menu::create(randomItem, selfItem, advanceItem, closeItem, setupItem, introItem, NULL);
	menu->setPosition(Vec2::ZERO);
	stillLayer->addChild(menu, 1);

	initSetUpMenu(moveLayer);
	moveLayer->runAction(ScaleTo::create(0.01, 0.01));

	initIntroMenu(introLayer);
	introLayer->runAction(ScaleTo::create(0.01, 0.01));
}

//设置菜单层
void SecondMainScene::initSetUpMenu(cocos2d::Node* pNode)
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	//半透明遮挡画布
	auto menuBack = Sprite::create("pic/MenuBack.png");
	if (menuBack == nullptr) {
		problemLoading("pic/MenuBack.png");
	}
	else {
		menuBack->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height / 2);
		pNode->addChild(menuBack, 1);
	}

	//动态菜单背景
	auto menuBackground = Sprite::create("pic/MenuBackground.png");
	if (menuBackground == nullptr) {
		problemLoading("MenuBackground.png");
	}
	else {
		menuBackground->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height / 2);
		pNode->addChild(menuBackground, 2);
	}

	//动态菜单关闭按钮
	auto menuCloseItem = MenuItemImage::create("pic/MenuCloseNormal.png", "pic/MenuCloseSelected.png",
		CC_CALLBACK_1(SecondMainScene::menuClose, this, pNode));
	if (menuCloseItem == nullptr || menuCloseItem->getContentSize().width <= 0 || menuCloseItem->getContentSize().height <= 0) {
		problemLoading("MenuCloseNormal.png and MenuCloseSelected.png");
	}
	else {
		menuCloseItem->setAnchorPoint(Vec2(1, 1));
		const float x = origin.x + vSize.width / 2 + menuBackground->getContentSize().width / 2;
		const float y = origin.y + vSize.height / 2 + menuBackground->getContentSize().height / 2;
		menuCloseItem->setPosition(Vec2(x, y));
	}

	//音乐音量按钮
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

	//音乐控制滑条
	auto musicSlider = ControlSlider::create("pic/SliderNormal.png", "pic/SliderSelected.png", "pic/SliderButton.png");
	musicSlider->setPosition(Vec2(origin.x + vSize.width * 0.52, origin.y + vSize.height * 0.65));
	musicSlider->setMinimumValue(0);
	musicSlider->setMaximumValue(100);
	auto tempMV = UserDefault::getInstance()->getFloatForKey("MusicVolume") * 100;
	musicSlider->setValue(tempMV);
	musicSlider->addTargetWithActionForControlEvents(this,
		cccontrol_selector(SecondMainScene::musicSliderCallBack), Control::EventType::VALUE_CHANGED);
	pNode->addChild(musicSlider, 4);

	//音效音量按钮
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

	//音效控制滑条
	auto effectSlider = ControlSlider::create("pic/SliderNormal.png", "pic/SliderSelected.png", "pic/SliderButton.png");
	effectSlider->setPosition(Vec2(origin.x + vSize.width * 0.52, origin.y + vSize.height * 0.52));
	effectSlider->setMinimumValue(0);
	effectSlider->setMaximumValue(100);
	auto tempEV = UserDefault::getInstance()->getFloatForKey("EffectVolume") * 100;
	effectSlider->setValue(tempEV);
	effectSlider->addTargetWithActionForControlEvents(this,
		cccontrol_selector(SecondMainScene::effectSliderCallBack), Control::EventType::VALUE_CHANGED);
	pNode->addChild(effectSlider, 6);

	//返回按钮
	auto returnGameItem = MenuItemImage::create("pic/ReturnMSNormal.png", "pic/ReturnMSSelected.png",
		CC_CALLBACK_1(SecondMainScene::returnCallBack, this));
	if (returnGameItem == nullptr || returnGameItem->getContentSize().width <= 0 || returnGameItem->getContentSize().height <= 0) {
		problemLoading("'ReturnMSNormal.png' and 'ReturnMSSelected.png'");
	}
	else {
		returnGameItem->setScale(1.4);
		const float x = origin.x + vSize.width * 0.5;
		const float y = origin.y + vSize.height * 0.355;
		returnGameItem->setPosition(Vec2(x, y));
	}

	//二级菜单
	auto moveMenu = Menu::create(menuCloseItem, returnGameItem, NULL);
	moveMenu->setPosition(Vec2::ZERO);
	pNode->addChild(moveMenu, 3);
}

//介绍层
void SecondMainScene::initIntroMenu(cocos2d::Node* pNode)
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//半透明遮挡画布
	auto introBack = Sprite::create("pic/MenuBack.png");
	if (introBack == nullptr) {
		problemLoading("pic/MenuBack.png");
	}
	else {
		introBack->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height / 2);
		pNode->addChild(introBack, 1);
	}

	//动态菜单背景
	auto introBackground = Sprite::create("pic/IntroBackground.png");
	if (introBackground == nullptr) {
		problemLoading("IntroBackground.png");
	}
	else {
		introBackground->setScale(1.2f);
		introBackground->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height / 2);
		pNode->addChild(introBackground, 2);
	}

	//动态菜单关闭按钮
	auto shopCloseItem = MenuItemImage::create("pic/MenuCloseNormal.png", "pic/MenuCloseSelected.png",
		CC_CALLBACK_1(SecondMainScene::menuClose, this, pNode));
	if (shopCloseItem == nullptr || shopCloseItem->getContentSize().width <= 0 || shopCloseItem->getContentSize().height <= 0) {
		problemLoading("MenuCloseNormal.png and MenuCloseSelected.png");
	}
	else {
		shopCloseItem->setAnchorPoint(Vec2(1, 1));
		const float x = origin.x + vSize.width / 2 + 1.125 * introBackground->getContentSize().width / 2;
		const float y = origin.y + vSize.height / 2 + 1.2 * introBackground->getContentSize().height / 2;
		shopCloseItem->setPosition(Vec2(x, y));
	}

	//二级菜单
	auto shopMenu = Menu::create(shopCloseItem, NULL);
	shopMenu->setPosition(Vec2::ZERO);
	pNode->addChild(shopMenu, 3);
}

//鼠标点击选择随机模式
void SecondMainScene::randomMoodCallBack(Ref* pSender)
{
	auto scene = RandomTypeScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

//鼠标点击选择自主模式
void SecondMainScene::selfMoodCallBack(Ref* pSender)
{
	auto scene = SelfTypeScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

//鼠标点击选择高级模式
void SecondMainScene::advanceMoodCallBack(Ref* pSender)
{
	auto scene = AdvanceTypeScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

//鼠标点击关闭操作
void SecondMainScene::gameCloseCallback(cocos2d::Ref* pSender)
{
	UserDefault::getInstance()->setFloatForKey("MusicVolume", 0.5);
	UserDefault::getInstance()->setFloatForKey("EffectVolume", 0.5);
	Director::getInstance()->end();
}

//鼠标点击弹出二级菜单
void SecondMainScene::getSetUpLayer(Ref* pSender, Node* pNode)
{
	pNode->runAction(ScaleTo::create(0.01, 1));
	pNode->setVisible(true);
}

//鼠标点击关闭二级菜单
void SecondMainScene::menuClose(Ref* pSender, Node* pNode)
{
	pNode->setVisible(false);
	pNode->runAction(ScaleTo::create(0.01, 0.01));
}

//音乐滑块效果
void SecondMainScene::musicSliderCallBack(Ref* pSender, Control::EventType type)
{
	auto slider = (ControlSlider*)pSender;
	auto value = slider->getValue();
	value = value / 100;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
	UserDefault::getInstance()->setFloatForKey("MusicVolume", value);
}

//音效滑块效果
void SecondMainScene::effectSliderCallBack(Ref* pSender, Control::EventType type)
{
	auto slider = (ControlSlider*)pSender;
	auto value = slider->getValue();
	value = value / 100;
	SimpleAudioEngine::getInstance()->setEffectsVolume(value);
	UserDefault::getInstance()->setFloatForKey("EffectVolume", value);
}

//返回上级场景
void SecondMainScene::returnCallBack(Ref* pSender)
{
	auto scene = MainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}