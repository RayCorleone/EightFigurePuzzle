#include "Classes/Scene/SelfTypeScene.h"
#include "Classes/Scene/SecondMainScene.h"
#include "Classes/Overall/WidelyUsed.h"
#include "Classes/Block/Block.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include <string>

USING_NS_CC;
using namespace std;
using namespace cocos2d::extension;
using namespace CocosDenshion;

Block* block2[N][N];				// 二维向量,九宫格
int a2[N][N][maxState] = { 0 };	// 状态记录列表
int path2[maxState] = { 0 };
int shortest2[maxState] = { 0 };
int now2[N][N] = { 0 };
int nex2[N][N] = { 0 };
int cnt2 = 0;

bool ready = 0;
std::string s;		//起始状态字符
bool sOk = 0;	//起始状态输入结束
std::string t;		//终止状态字符
bool tOk = 0;	//目标状态输入结束

Scene* SelfTypeScene::createScene()
{
	return SelfTypeScene::create();
}

bool SelfTypeScene::init()
{
	if (!Scene::init()) { return false; };

	initStillNode();
	initMoveLayer();
	initGameLayer();

	return true;
}

//静态元素
void SelfTypeScene::initStillNode()
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
	auto markBoard = Sprite::create("pic/MarkBoard2.png");
	if (markBoard == nullptr) {
		problemLoading("MarkBoard2.png");
	}
	else {
		markBoard->setScale(vSize.height / gameBackGround->getContentSize().height);
		markBoard->setPosition(Vec2(vSize.width * 0.25 + origin.x, vSize.height * 0.43 + origin.y));
		stillNode->addChild(markBoard, 3);
	}
}

//动态元素
void SelfTypeScene::initMoveLayer()
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto stillLayer = Layer::create();
	this->addChild(stillLayer, 2);
	auto moveLayer = Layer::create();
	this->addChild(moveLayer, 4);
	moveLayer->setVisible(false);

	//起始状态输入框
	auto startEditBox = EditBox::create(Size(265, 65), Scale9Sprite::create("pic/EditBox.png"));
	startEditBox->setPosition(Vec2(origin.x + vSize.width * 0.266, origin.y + vSize.height * 0.781));
	startEditBox->setMaxLength(9);				//输入框最多能输入多少个字符
	startEditBox->setText("Input 0-8");			//初始化文字
	startEditBox->setFontColor(Color3B(0, 0, 0)); //文字颜色
	startEditBox->setFontSize(50);
	stillLayer->addChild(startEditBox);

	//起始状态确认按钮
	auto startBottom = MenuItemImage::create("pic/YesNormal.png", "pic/YesSelected.png",
		CC_CALLBACK_1(SelfTypeScene::startEdit, this, startEditBox));
	startBottom->setPosition(Vec2(origin.x + vSize.width * 0.39, origin.y + vSize.height * 0.7755));
	startBottom->setScale(1.22f);

	//终止状态输入框
	auto targetEditBox = EditBox::create(Size(265, 65), Scale9Sprite::create("pic/EditBox.png"));
	targetEditBox->setPosition(Vec2(origin.x + vSize.width * 0.266, origin.y + vSize.height * 0.6682));
	targetEditBox->setMaxLength(9);				//输入框最多能输入多少个字符
	targetEditBox->setText("Input 0-8");			//初始化文字
	targetEditBox->setFontColor(Color3B(0, 0, 0)); //文字颜色
	targetEditBox->setFontSize(50);
	stillLayer->addChild(targetEditBox);

	//终止状态确认按钮
	auto targetBottom = MenuItemImage::create("pic/YesNormal.png", "pic/YesSelected.png",
		CC_CALLBACK_1(SelfTypeScene::targetEdit, this, targetEditBox));
	targetBottom->setPosition(Vec2(origin.x + vSize.width * 0.39, origin.y + vSize.height * 0.6627));
	targetBottom->setScale(1.22f);

	//暂停按钮
	auto pauseItem = MenuItemImage::create("pic/PauseNormal.png", "pic/PauseSelected.png",
		CC_CALLBACK_1(SelfTypeScene::getSetUpLayer, this, moveLayer));
	if (pauseItem == nullptr || pauseItem->getContentSize().width <= 0 || pauseItem->getContentSize().height <= 0) {
		problemLoading("'PauseNormal.png' and 'PauseSelected.png'");
	}
	else {
		const float x = origin.x + pauseItem->getContentSize().width / 2;
		const float y = origin.y + pauseItem->getContentSize().height / 2;
		pauseItem->setPosition(Vec2(x, y));
	}

	//静态菜单
	auto stillMenu = Menu::create(startBottom, targetBottom, pauseItem, NULL);
	stillMenu->setPosition(Vec2::ZERO);
	stillLayer->addChild(stillMenu);

	//二级菜单
	initMenu(moveLayer);
	moveLayer->runAction(ScaleTo::create(0.01, 0.01));
}

//菜单元素
void SelfTypeScene::initMenu(Node* pNode)
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
		cccontrol_selector(SelfTypeScene::musicSliderCallBack), Control::EventType::VALUE_CHANGED);
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
		cccontrol_selector(SelfTypeScene::effectSliderCallBack), Control::EventType::VALUE_CHANGED);
	pNode->addChild(effectSlider, 6);

	//动态菜单关闭按钮
	auto menuCloseItem = MenuItemImage::create("pic/MenuCloseNormal.png", "pic/MenuCloseSelected.png",
		CC_CALLBACK_1(SelfTypeScene::menuClose, this, pNode));
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
		CC_CALLBACK_1(SelfTypeScene::returnCallBack, this));
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
		CC_CALLBACK_1(SelfTypeScene::gameCloseCallback, this));
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
void SelfTypeScene::musicSliderCallBack(Ref* pSender, Control::EventType type)
{
	auto slider = (ControlSlider*)pSender;
	auto value = slider->getValue();
	value = value / 100;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
	UserDefault::getInstance()->setFloatForKey("MusicVolume", value);
}

//音效滑块效果
void SelfTypeScene::effectSliderCallBack(Ref* pSender, Control::EventType type)
{
	auto slider = (ControlSlider*)pSender;
	auto value = slider->getValue();
	value = value / 100;
	SimpleAudioEngine::getInstance()->setEffectsVolume(value);
	UserDefault::getInstance()->setFloatForKey("EffectVolume", value);
}

//获取起始状态
void SelfTypeScene::startEdit(Ref* pSender, EditBox* editbox)
{
	//输入合法标志
	bool f = 0;
	bool List[9] = { 0 };

	if (sOk == 0) {	//还未初始化
		s = editbox->getText();

		int temp = -1;
		for (int i = 0; i < N; i++) {	// 起始态输入
			for (int j = 0; j < N; j++) {
				temp = s[i * N + j] - '0';
				//输入越界
				if (temp < 0 || temp > 8 || List[temp] == 1) {
					f = 1;	break;
				}
				else
					List[temp] = 1;
			}
			if (f == 1)
				break;
		}
		//输入不足
		for (int i = 0; i < 9; i++) {
			if (List[i] != 1)
				f = 1;
		}

		if (f == 1)	sOk = 0;	//输入不合法，重新输入
		else		sOk = 1;	//输入合法，不再输入
	}
}

//获取终止状态
void SelfTypeScene::targetEdit(Ref* pSender, EditBox* editbox)
{
	//输入合法标志
	bool f = 0;
	bool List[9] = { 0 };

	if (tOk == 0) {	//还未初始化
		t = editbox->getText();

		int temp = -1;
		for (int i = 0; i < N; i++) {	// 起始态输入
			for (int j = 0; j < N; j++) {
				temp = t[i * N + j] - '0';
				//输入越界
				if (temp < 0 || temp > 8 || List[temp] == 1) {
					f = 1;	break;
				}
				else
					List[temp] = 1;
			}
			if (f == 1)
				break;
		}
		//输入不足
		for (int i = 0; i < 9; i++) {
			if (List[i] != 1)
				f = 1;
		}

		if (f == 1)	tOk = 0;	//输入不合法，重新输入
		else		tOk = 1;	//输入合法，不再输入
	}
}

//鼠标点击关闭操作
void SelfTypeScene::gameCloseCallback(Ref* pSender)
{
	UserDefault::getInstance()->setFloatForKey("MusicVolume", 0.5);
	UserDefault::getInstance()->setFloatForKey("EffectVolume", 0.5);
	Director::getInstance()->end();
}

//鼠标点击弹出菜单
void SelfTypeScene::getSetUpLayer(Ref* pSender, Node* pNode)
{
	pNode->runAction(ScaleTo::create(0.01, 1));
	pNode->setVisible(true);
	this->pauseSchedulerAndActions();
}

//鼠标点击关闭菜单
void SelfTypeScene::menuClose(Ref* pSender, Node* pNode)
{
	pNode->setVisible(false);
	pNode->runAction(ScaleTo::create(0.01, 0.01));
	this->resumeSchedulerAndActions();
}

//返回上级场景
void SelfTypeScene::returnCallBack(Ref* pSender)
{
	step = 0;
	maxStep = 0;
	isEnd = 0;
	sOk = 0;
	tOk = 0;
	ready = 0;
	auto scene = SecondMainScene::createScene();
	Director::getInstance()->replaceScene(scene);
}

/*********************************演示逻辑部分*********************************/
void SelfTypeScene::initGameLayer()
{
	//数据输入
	this->schedule(schedule_selector(SelfTypeScene::initData), 0.05f);
	this->schedule(schedule_selector(SelfTypeScene::moveBlocks), 1.5f);
	this->schedule(schedule_selector(SelfTypeScene::endGame), 2.0f);
	this->schedule(schedule_selector(SelfTypeScene::stepUpdate), 0.05f);
}

void SelfTypeScene::initData(float dt) {
	Size vSize = Director::getInstance()->getVisibleSize();
	Point origin = Director::getInstance()->getVisibleOrigin();

	auto gameLayer = Node::create();
	this->addChild(gameLayer, 3);

	if (sOk == 1 && tOk == 1 && ready == 0) {	// 输入均正确
		//步数栏
		stepLabel = LabelTTF::create("0", "fonts/Marker Felt", 44);
		stepLabel->setColor(ccc3(0, 0, 1));
		stepLabel->setPosition(Point(origin.x + vSize.width * 0.403, origin.y + vSize.height * 0.29 - stepLabel->getContentSize().height));
		gameLayer->addChild(stepLabel, 1);

		isEnd = 0;
		maxStep = 0;
		step = 0;

		// 获取初始、目标状态
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				start[i][j] = s[i * N + j] - '0';
				target[i][j] = t[i * N + j] - '0';
			}
		}
		statecpy(now2, start);
		statecpy(nex2, start);

		//设置各个目标小图片位置 - 创建目标小区域
		int x = -142, y = -140;
		for (int i = 2; i >= 0; i--) {
			for (int j = 0; j < 3; j++) {
				int tempX = origin.x + vSize.width * 0.387 + x;
				int tempY = origin.y + vSize.height * 0.505 + y;
				pos[i][j] = Point(tempX, tempY);
				x = x + 90;
			}
			x = -142;
			y = y + 90;
		}
		int picNum;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				picNum = target[i][j];
				Block* p = new Block(pos[i][j], picNum);
				p->getPic()->setScale(0.56f);
				this->addChild(p->getPic(), 1);
			}
		}

		//设置各个起始小图片位置 - 创建起始小区域
		x = -142;	y = -140;
		for (int i = 2; i >= 0; i--) {
			for (int j = 0; j < 3; j++) {
				int tempX = origin.x + vSize.width * 0.175 + x;
				int tempY = origin.y + vSize.height * 0.505 + y;
				pos[i][j] = Point(tempX, tempY);
				x = x + 90;
			}
			x = -142;
			y = y + 90;
		}
		picNum = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				picNum = start[i][j];
				Block* p = new Block(pos[i][j], picNum);
				p->getPic()->setScale(0.56f);
				this->addChild(p->getPic(), 1);
			}
		}

		//设置各个演示图片位置
		x = -142;
		y = -140;
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

		//创建演示主区域
		picNum = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				picNum = start[i][j];
				Block* p = new Block(pos[i][j], picNum);
				block2[i][j] = p;
				this->addChild(p->getPic(), 1);
			}
		}

		// 预判断
		if (!(calDe(start) % 2 == calDe(target) % 2)) {
			isEnd = 1;
			maxStep = maxState;
		}
		// A*寻路
		int res = Astar1(a2, start, target, path2);
		if (res == -1) {
			isEnd = 1;
			maxStep = maxState;
		}
		// shortest[]-存储状态标号；maxStep-存储总步数
		while (res > 0) {
			shortest2[maxStep++] = res;
			res = path2[res];
		}

		cnt2 = maxStep;
		ready = 1;
		this->unscheduleUpdate();
	}
}

void SelfTypeScene::stepUpdate(float dt)
{
	if (ready == 1) {
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
}

void SelfTypeScene::moveBlocks(float dt)
{
	if (ready == 1) {
		if (cnt2 == maxStep) {	// 刚开始无操作
			step = 0;
			cnt2 = cnt2 - 1;
		}
		else if (cnt2 >= 0) {
			// 定位该移动的位置
			int x0 = 0, y0 = 0;	//空白位置
			int x1 = 0, y1 = 0;	//交换位置
			getBrack(now2, x0, y0);
			getState(a2, nex2, shortest2[cnt2]);
			getBrack(nex2, x1, y1);

			// 开始移动
			cocos2d::MoveTo* move1 = cocos2d::MoveTo::create(0.5f, block2[x0][y0]->getPos());
			cocos2d::MoveTo* move2 = cocos2d::MoveTo::create(0.5f, block2[x1][y1]->getPos());

			auto effectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/move.mp3");
			block2[x0][y0]->getPic()->runAction(Sequence::create(move2->clone(), DelayTime::create(0.05f), NULL));
			block2[x1][y1]->getPic()->runAction(Sequence::create(move1->clone(), DelayTime::create(0.05f), NULL));

			Point po = block2[x0][y0]->getPos();
			block2[x0][y0]->setPos(block2[x1][y1]->getPos());
			block2[x1][y1]->setPos(po);
			std::swap(block2[x0][y0], block2[x1][y1]);

			// 准备下一次
			step = step + 1;
			cnt2 = cnt2 - 1;
			statecpy(now2, nex2);
		}
		else if (cnt2 == -1) {
			cnt2 = cnt2 - 1;
		}
		else {
			isEnd = 1;
			this->unscheduleUpdate();
		}
	}
}

void SelfTypeScene::endGame(float dt)
{
	if (ready == 1) {
		if (isEnd == 1 && step == maxStep) {	//全部走完
			initEndLayer(1);
			this->unschedule(schedule_selector(SelfTypeScene::endGame));
			this->unschedule(schedule_selector(SelfTypeScene::moveBlocks));
		}
		else if (isEnd == 1) {	//提前终止
			initEndLayer(2);
			this->unschedule(schedule_selector(SelfTypeScene::endGame));
			this->unschedule(schedule_selector(SelfTypeScene::moveBlocks));
		}
	}
}

void SelfTypeScene::initEndLayer(const int result)
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
		CC_CALLBACK_1(SelfTypeScene::returnCallBack, this));
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
		CC_CALLBACK_1(SelfTypeScene::gameCloseCallback, this));
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