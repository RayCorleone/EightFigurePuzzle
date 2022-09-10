#include "Classes/Scene/RandomTypeScene.h"
#include "Classes/Scene/SecondMainScene.h"
#include "Classes/Overall/WidelyUsed.h"
#include "Classes/Block/Block.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
using namespace cocos2d::extension;
using namespace CocosDenshion;

Block* block[N][N];				// 二维向量,九宫格
int a[N][N][maxState] = { 0 };	// 状态记录列表
int path[maxState] = { 0 };
int shortest[maxState] = { 0 };
int now[N][N] = { 0 };
int nex[N][N] = { 0 };
int cnt = 0;

Scene* RandomTypeScene::createScene()
{
	return RandomTypeScene::create();
}

bool RandomTypeScene::init()
{
	if (!Scene::init()) { return false; };

	initStillNode();
	initMoveLayer();
	initGameLayer();

	return true;
}

//静态元素
void RandomTypeScene::initStillNode()
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto stillNode = Node::create();
	this->addChild(stillNode, 1);

	//大背景
	auto theBackGround = Sprite::create("pic/TheBG2.png");
	if (theBackGround == nullptr) {
		problemLoading("TheBG2.png");
	}
	else {
		theBackGround->setScale(vSize.height / theBackGround->getContentSize().height + 0.02);
		theBackGround->setPosition(Vec2(vSize.width / 2 + origin.x, vSize.height / 2 + origin.y));
		stillNode->addChild(theBackGround, 1);
	}

	//游戏区背景
	auto gameBackGround = Sprite::create("pic/GameBG.png");
	if (gameBackGround == nullptr) {
		problemLoading("GameBG.png");
	}
	else {
		gameBackGround->setScale(vSize.height / gameBackGround->getContentSize().height);
		gameBackGround->setAnchorPoint(Vec2(1, 0));
		gameBackGround->setPosition(Vec2(vSize.width + origin.x, origin.y));
		stillNode->addChild(gameBackGround, 2);
	}

	//通知栏背景
	auto markBoard = Sprite::create("pic/MarkBoard.png");
	if (markBoard == nullptr) {
		problemLoading("MarkBoard.png");
	}
	else {
		markBoard->setScale(vSize.height / gameBackGround->getContentSize().height);
		markBoard->setPosition(Vec2(vSize.width * 0.25 + origin.x, vSize.height * 0.43 + origin.y));
		stillNode->addChild(markBoard, 3);
	}
}

//动态元素
void RandomTypeScene::initMoveLayer()
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto stillLayer = Layer::create();
	this->addChild(stillLayer, 2);
	auto moveLayer = Layer::create();
	this->addChild(moveLayer, 4);
	moveLayer->setVisible(false);

	//暂停按钮
	auto pauseItem = MenuItemImage::create("pic/PauseNormal.png", "pic/PauseSelected.png",
		CC_CALLBACK_1(RandomTypeScene::getSetUpLayer, this, moveLayer));
	if (pauseItem == nullptr || pauseItem->getContentSize().width <= 0 || pauseItem->getContentSize().height <= 0) {
		problemLoading("'PauseNormal.png' and 'PauseSelected.png'");
	}
	else {
		const float x = origin.x + pauseItem->getContentSize().width / 2;
		const float y = origin.y + pauseItem->getContentSize().height / 2;
		pauseItem->setPosition(Vec2(x, y));
	}

	//静态菜单
	auto stillMenu = Menu::create(pauseItem, NULL);
	stillMenu->setPosition(Vec2::ZERO);
	stillLayer->addChild(stillMenu);

	//二级菜单
	initMenu(moveLayer);
	moveLayer->runAction(ScaleTo::create(0.01, 0.01));
}

//菜单元素
void RandomTypeScene::initMenu(Node* pNode)
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
		cccontrol_selector(RandomTypeScene::musicSliderCallBack), Control::EventType::VALUE_CHANGED);
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
		cccontrol_selector(RandomTypeScene::effectSliderCallBack), Control::EventType::VALUE_CHANGED);
	pNode->addChild(effectSlider, 6);

	//动态菜单关闭按钮
	auto menuCloseItem = MenuItemImage::create("pic/MenuCloseNormal.png", "pic/MenuCloseSelected.png",
		CC_CALLBACK_1(RandomTypeScene::menuClose, this, pNode));
	if (menuCloseItem == nullptr || menuCloseItem->getContentSize().width <= 0 || menuCloseItem->getContentSize().height <= 0) {
		problemLoading("MenuCloseNormal.png and MenuCloseSelected.png");
	}
	else {
		menuCloseItem->setAnchorPoint(Vec2(1, 1));
		const float x = origin.x + vSize.width / 2 + menuBackground->getContentSize().width / 2;
		const float y = origin.y + vSize.height / 2 + menuBackground->getContentSize().height / 2;
		menuCloseItem->setPosition(Vec2(x, y));
	}

	//返回按钮
	auto returnGameItem = MenuItemImage::create("pic/ReturnHomeNormal.png", "pic/ReturnHomeSelected.png",
		CC_CALLBACK_1(RandomTypeScene::returnCallBack, this));
	if (returnGameItem == nullptr || returnGameItem->getContentSize().width <= 0 || returnGameItem->getContentSize().height <= 0) {
		problemLoading("'ReturnHomeNormal.png' and 'ReturnHomeSelected.png'");
	}
	else {
		returnGameItem->setScale(1.2);
		const float x = origin.x + vSize.width * 0.37;
		const float y = origin.y + vSize.height * 0.355;
		returnGameItem->setPosition(Vec2(x, y));
	}

	//退出按钮
	auto closeItem = MenuItemImage::create("pic/CloseNormal2.png", "pic/CloseSelected2.png",
		CC_CALLBACK_1(RandomTypeScene::gameCloseCallback, this));
	if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0) {
		problemLoading("'CloseNormal2.png' and 'CloseSelected2.png'");
	}
	else {
		closeItem->setScale(1.2);
		const float x = origin.x + vSize.width * 0.63;
		const float y = origin.y + vSize.height * 0.355;
		closeItem->setPosition(Vec2(x, y));
	}

	//二级菜单
	auto moveMenu = Menu::create(menuCloseItem, returnGameItem, closeItem, NULL);
	moveMenu->setPosition(Vec2::ZERO);
	pNode->addChild(moveMenu, 3);
}

//音乐滑块效果
void RandomTypeScene::musicSliderCallBack(Ref* pSender, Control::EventType type)
{
	auto slider = (ControlSlider*)pSender;
	auto value = slider->getValue();
	value = value / 100;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
	UserDefault::getInstance()->setFloatForKey("MusicVolume", value);
}

//音效滑块效果
void RandomTypeScene::effectSliderCallBack(Ref* pSender, Control::EventType type)
{
	auto slider = (ControlSlider*)pSender;
	auto value = slider->getValue();
	value = value / 100;
	SimpleAudioEngine::getInstance()->setEffectsVolume(value);
	UserDefault::getInstance()->setFloatForKey("EffectVolume", value);
}

//鼠标点击关闭操作
void RandomTypeScene::gameCloseCallback(Ref* pSender)
{
	UserDefault::getInstance()->setFloatForKey("MusicVolume", 0.5);
	UserDefault::getInstance()->setFloatForKey("EffectVolume", 0.5);
	Director::getInstance()->end();
}

//鼠标点击弹出菜单
void RandomTypeScene::getSetUpLayer(Ref* pSender, Node* pNode)
{
	pNode->runAction(ScaleTo::create(0.01, 1));
	pNode->setVisible(true);
	this->pauseSchedulerAndActions();
}

//鼠标点击关闭菜单
void RandomTypeScene::menuClose(Ref* pSender, Node* pNode)
{
	pNode->setVisible(false);
	pNode->runAction(ScaleTo::create(0.01, 0.01));
	this->resumeSchedulerAndActions();
}

//返回上级场景
void RandomTypeScene::returnCallBack(Ref* pSender)
{
	step = 0;
	maxStep = 0;
	isEnd = 0;
	int tempStart[N][N] = { 1,2,3,8,0,4,7,6,5 };
	statecpy(start, tempStart);
	statecpy(target, tempStart);
	auto scene = SecondMainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

/*********************************演示逻辑部分*********************************/
void RandomTypeScene::initGameLayer()
{
	Size vSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	auto gameLayer = Node::create();
	this->addChild(gameLayer, 3);

	isEnd = 0;
	maxStep = 0;
	step = 0;

	// 随机生成初始状态
	autoGenerate(start);
	statecpy(now, start);
	statecpy(nex, start);

	// A*寻路
	int res = Astar1(a, start, target, path);
	if (res == -1) {
		isEnd = 1;
		maxStep = maxState;
	}

	// shortest[]-存储状态标号；maxStep-存储总步数
	while (res > 0) {
		shortest[maxStep++] = res;
		res = path[res];
	}

	//设置各个图片位置
	int x = -142, y = -140;
	for (int i = 2; i >= 0; i--) {
		for (int j = 0; j < 3; j++) {
			int tempX = origin.x + vSize.width * 0.7 + x;
			int tempY = origin.y + vSize.height * 0.43 + y;
			pos[i][j] = Point(tempX, tempY);
			x = x + 205;
		}
		x = -142;
		y = y + 205;
	}

	//步数栏
	stepLabel = LabelTTF::create("0", "fonts/Marker Felt", 44);
	stepLabel->setColor(ccc3(0, 0, 1));
	stepLabel->setPosition(Point(origin.x + vSize.width * 0.403, origin.y + vSize.height * 0.29 - stepLabel->getContentSize().height));
	gameLayer->addChild(stepLabel, 1);

	//创建演示主区域
	int picNum;
	//std::srand(time(NULL));
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			picNum = start[i][j];
			Block* p = new Block(pos[i][j], picNum);
			block[i][j] = p;
			this->addChild(p->getPic(), 1);
		}
	}

	//刷新
	cnt = maxStep;
	this->schedule(schedule_selector(RandomTypeScene::moveBlocks), 1.5f);
	this->schedule(schedule_selector(RandomTypeScene::endGame), 2.0f);
	this->scheduleUpdate();
}

void RandomTypeScene::update(float dt)
{
	char newStep[20];
	if (step == maxStep) {
		sprintf(newStep, "%d", step);
		stepLabel->setString(newStep);
		this->unscheduleUpdate();
	}
	else {
		sprintf(newStep, "%d", step);
		stepLabel->setString(newStep);
	}
}

void RandomTypeScene::moveBlocks(float dt)
{
	if (cnt == maxStep) {	// 刚开始无操作
		step = 0;
		cnt = cnt - 1;
	}
	else if (cnt >= 0) {
		// 定位该移动的位置
		int x0 = 0, y0 = 0;	//空白位置
		int x1 = 0, y1 = 0;	//交换位置
		getBrack(now, x0, y0);
		getState(a, nex, shortest[cnt]);
		getBrack(nex, x1, y1);

		// 开始移动
		cocos2d::MoveTo* move1 = cocos2d::MoveTo::create(0.5f, block[x0][y0]->getPos());
		cocos2d::MoveTo* move2 = cocos2d::MoveTo::create(0.5f, block[x1][y1]->getPos());

		auto effectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/move.mp3");
		block[x0][y0]->getPic()->runAction(Sequence::create(move2->clone(), DelayTime::create(0.05f), NULL));
		block[x1][y1]->getPic()->runAction(Sequence::create(move1->clone(), DelayTime::create(0.05f), NULL));

		Point po = block[x0][y0]->getPos();
		block[x0][y0]->setPos(block[x1][y1]->getPos());
		block[x1][y1]->setPos(po);
		std::swap(block[x0][y0], block[x1][y1]);

		// 准备下一次
		step = step + 1;
		cnt = cnt - 1;
		statecpy(now, nex);
	}
	else if(cnt==-1){
		cnt = cnt - 1;
	}
	else {
		isEnd = 1;
		this->unscheduleUpdate();
	}
}

void RandomTypeScene::endGame(float dt)
{
	if (isEnd == 1 && step == maxStep) {	//全部走完
		initEndLayer(1);
		this->unschedule(schedule_selector(RandomTypeScene::endGame));
		this->unschedule(schedule_selector(RandomTypeScene::moveBlocks));
	}
	else if(isEnd == 1) {	//提前终止
		initEndLayer(2);
		this->unschedule(schedule_selector(RandomTypeScene::endGame));
		this->unschedule(schedule_selector(RandomTypeScene::moveBlocks));
	}
}

void RandomTypeScene::initEndLayer(const int result)
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto pNode = Layer::create();

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
	char resultName[30];
	sprintf(resultName, "pic/Result%d.png", result);
	auto menuBackground = Sprite::create(resultName);
	menuBackground->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height / 2);
	pNode->addChild(menuBackground, 2);

	//返回按钮
	auto returnGameItem = MenuItemImage::create("pic/ReturnHomeNormal.png", "pic/ReturnHomeSelected.png",
		CC_CALLBACK_1(RandomTypeScene::returnCallBack, this));
	if (returnGameItem == nullptr || returnGameItem->getContentSize().width <= 0 || returnGameItem->getContentSize().height <= 0) {
		problemLoading("'ReturnHomeNormal.png' and 'ReturnHomeSelected.png'");
	}
	else {
		returnGameItem->setScale(1.2);
		const float x = origin.x + vSize.width * 0.37;
		const float y = origin.y + vSize.height * 0.355;
		returnGameItem->setPosition(Vec2(x, y));
	}

	//退出按钮
	auto closeItem = MenuItemImage::create("pic/CloseNormal2.png", "pic/CloseSelected2.png",
		CC_CALLBACK_1(RandomTypeScene::gameCloseCallback, this));
	if (closeItem == nullptr || closeItem->getContentSize().width <= 0 || closeItem->getContentSize().height <= 0) {
		problemLoading("'CloseNormal2.png' and 'CloseSelected2.png'");
	}
	else {
		closeItem->setScale(1.2);
		const float x = origin.x + vSize.width * 0.63;
		const float y = origin.y + vSize.height * 0.355;
		closeItem->setPosition(Vec2(x, y));
	}

	//二级菜单
	auto moveMenu = Menu::create(returnGameItem, closeItem, NULL);
	moveMenu->setPosition(Vec2::ZERO);
	pNode->addChild(moveMenu, 3);

	this->addChild(pNode, 5);
}