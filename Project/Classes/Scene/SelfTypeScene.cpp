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

Block* block2[N][N];				// ��ά����,�Ź���
int a2[N][N][maxState] = { 0 };	// ״̬��¼�б�
int path2[maxState] = { 0 };
int shortest2[maxState] = { 0 };
int now2[N][N] = { 0 };
int nex2[N][N] = { 0 };
int cnt2 = 0;

bool ready = 0;
std::string s;		//��ʼ״̬�ַ�
bool sOk = 0;	//��ʼ״̬�������
std::string t;		//��ֹ״̬�ַ�
bool tOk = 0;	//Ŀ��״̬�������

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

//��̬Ԫ��
void SelfTypeScene::initStillNode()
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto stillNode = Node::create();
	this->addChild(stillNode, 1);

	//�󱳾�
	auto theBackGround = Sprite::create("pic/TheBG2.png");
	if (theBackGround == nullptr) {
		problemLoading("TheBG2.png");
	}
	else {
		theBackGround->setScale(vSize.height / theBackGround->getContentSize().height + 0.02);
		theBackGround->setPosition(Vec2(vSize.width / 2 + origin.x, vSize.height / 2 + origin.y));
		stillNode->addChild(theBackGround, 1);
	}

	//��Ϸ������
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

	//֪ͨ������
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

//��̬Ԫ��
void SelfTypeScene::initMoveLayer()
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto stillLayer = Layer::create();
	this->addChild(stillLayer, 2);
	auto moveLayer = Layer::create();
	this->addChild(moveLayer, 4);
	moveLayer->setVisible(false);

	//��ʼ״̬�����
	auto startEditBox = EditBox::create(Size(265, 65), Scale9Sprite::create("pic/EditBox.png"));
	startEditBox->setPosition(Vec2(origin.x + vSize.width * 0.266, origin.y + vSize.height * 0.781));
	startEditBox->setMaxLength(9);				//����������������ٸ��ַ�
	startEditBox->setText("Input 0-8");			//��ʼ������
	startEditBox->setFontColor(Color3B(0, 0, 0)); //������ɫ
	startEditBox->setFontSize(50);
	stillLayer->addChild(startEditBox);

	//��ʼ״̬ȷ�ϰ�ť
	auto startBottom = MenuItemImage::create("pic/YesNormal.png", "pic/YesSelected.png",
		CC_CALLBACK_1(SelfTypeScene::startEdit, this, startEditBox));
	startBottom->setPosition(Vec2(origin.x + vSize.width * 0.39, origin.y + vSize.height * 0.7755));
	startBottom->setScale(1.22f);

	//��ֹ״̬�����
	auto targetEditBox = EditBox::create(Size(265, 65), Scale9Sprite::create("pic/EditBox.png"));
	targetEditBox->setPosition(Vec2(origin.x + vSize.width * 0.266, origin.y + vSize.height * 0.6682));
	targetEditBox->setMaxLength(9);				//����������������ٸ��ַ�
	targetEditBox->setText("Input 0-8");			//��ʼ������
	targetEditBox->setFontColor(Color3B(0, 0, 0)); //������ɫ
	targetEditBox->setFontSize(50);
	stillLayer->addChild(targetEditBox);

	//��ֹ״̬ȷ�ϰ�ť
	auto targetBottom = MenuItemImage::create("pic/YesNormal.png", "pic/YesSelected.png",
		CC_CALLBACK_1(SelfTypeScene::targetEdit, this, targetEditBox));
	targetBottom->setPosition(Vec2(origin.x + vSize.width * 0.39, origin.y + vSize.height * 0.6627));
	targetBottom->setScale(1.22f);

	//��ͣ��ť
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

	//��̬�˵�
	auto stillMenu = Menu::create(startBottom, targetBottom, pauseItem, NULL);
	stillMenu->setPosition(Vec2::ZERO);
	stillLayer->addChild(stillMenu);

	//�����˵�
	initMenu(moveLayer);
	moveLayer->runAction(ScaleTo::create(0.01, 0.01));
}

//�˵�Ԫ��
void SelfTypeScene::initMenu(Node* pNode)
{
	auto vSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

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
	auto tempMV = UserDefault::getInstance()->getFloatForKey("MusicVolume") * 100;
	musicSlider->setValue(tempMV);
	musicSlider->addTargetWithActionForControlEvents(this,
		cccontrol_selector(SelfTypeScene::musicSliderCallBack), Control::EventType::VALUE_CHANGED);
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
	auto tempEV = UserDefault::getInstance()->getFloatForKey("EffectVolume") * 100;
	effectSlider->setValue(tempEV);
	effectSlider->addTargetWithActionForControlEvents(this,
		cccontrol_selector(SelfTypeScene::effectSliderCallBack), Control::EventType::VALUE_CHANGED);
	pNode->addChild(effectSlider, 6);

	//��̬�˵��رհ�ť
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

	//���ذ�ť
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

	//�˳���ť
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

	//�����˵�
	auto moveMenu = Menu::create(menuCloseItem, returnGameItem, closeItem, NULL);
	moveMenu->setPosition(Vec2::ZERO);
	pNode->addChild(moveMenu, 3);
}

//���ֻ���Ч��
void SelfTypeScene::musicSliderCallBack(Ref* pSender, Control::EventType type)
{
	auto slider = (ControlSlider*)pSender;
	auto value = slider->getValue();
	value = value / 100;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(value);
	UserDefault::getInstance()->setFloatForKey("MusicVolume", value);
}

//��Ч����Ч��
void SelfTypeScene::effectSliderCallBack(Ref* pSender, Control::EventType type)
{
	auto slider = (ControlSlider*)pSender;
	auto value = slider->getValue();
	value = value / 100;
	SimpleAudioEngine::getInstance()->setEffectsVolume(value);
	UserDefault::getInstance()->setFloatForKey("EffectVolume", value);
}

//��ȡ��ʼ״̬
void SelfTypeScene::startEdit(Ref* pSender, EditBox* editbox)
{
	//����Ϸ���־
	bool f = 0;
	bool List[9] = { 0 };

	if (sOk == 0) {	//��δ��ʼ��
		s = editbox->getText();

		int temp = -1;
		for (int i = 0; i < N; i++) {	// ��ʼ̬����
			for (int j = 0; j < N; j++) {
				temp = s[i * N + j] - '0';
				//����Խ��
				if (temp < 0 || temp > 8 || List[temp] == 1) {
					f = 1;	break;
				}
				else
					List[temp] = 1;
			}
			if (f == 1)
				break;
		}
		//���벻��
		for (int i = 0; i < 9; i++) {
			if (List[i] != 1)
				f = 1;
		}

		if (f == 1)	sOk = 0;	//���벻�Ϸ�����������
		else		sOk = 1;	//����Ϸ�����������
	}
}

//��ȡ��ֹ״̬
void SelfTypeScene::targetEdit(Ref* pSender, EditBox* editbox)
{
	//����Ϸ���־
	bool f = 0;
	bool List[9] = { 0 };

	if (tOk == 0) {	//��δ��ʼ��
		t = editbox->getText();

		int temp = -1;
		for (int i = 0; i < N; i++) {	// ��ʼ̬����
			for (int j = 0; j < N; j++) {
				temp = t[i * N + j] - '0';
				//����Խ��
				if (temp < 0 || temp > 8 || List[temp] == 1) {
					f = 1;	break;
				}
				else
					List[temp] = 1;
			}
			if (f == 1)
				break;
		}
		//���벻��
		for (int i = 0; i < 9; i++) {
			if (List[i] != 1)
				f = 1;
		}

		if (f == 1)	tOk = 0;	//���벻�Ϸ�����������
		else		tOk = 1;	//����Ϸ�����������
	}
}

//������رղ���
void SelfTypeScene::gameCloseCallback(Ref* pSender)
{
	UserDefault::getInstance()->setFloatForKey("MusicVolume", 0.5);
	UserDefault::getInstance()->setFloatForKey("EffectVolume", 0.5);
	Director::getInstance()->end();
}

//����������˵�
void SelfTypeScene::getSetUpLayer(Ref* pSender, Node* pNode)
{
	pNode->runAction(ScaleTo::create(0.01, 1));
	pNode->setVisible(true);
	this->pauseSchedulerAndActions();
}

//������رղ˵�
void SelfTypeScene::menuClose(Ref* pSender, Node* pNode)
{
	pNode->setVisible(false);
	pNode->runAction(ScaleTo::create(0.01, 0.01));
	this->resumeSchedulerAndActions();
}

//�����ϼ�����
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

/*********************************��ʾ�߼�����*********************************/
void SelfTypeScene::initGameLayer()
{
	//��������
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

	if (sOk == 1 && tOk == 1 && ready == 0) {	// �������ȷ
		//������
		stepLabel = LabelTTF::create("0", "fonts/Marker Felt", 44);
		stepLabel->setColor(ccc3(0, 0, 1));
		stepLabel->setPosition(Point(origin.x + vSize.width * 0.403, origin.y + vSize.height * 0.29 - stepLabel->getContentSize().height));
		gameLayer->addChild(stepLabel, 1);

		isEnd = 0;
		maxStep = 0;
		step = 0;

		// ��ȡ��ʼ��Ŀ��״̬
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				start[i][j] = s[i * N + j] - '0';
				target[i][j] = t[i * N + j] - '0';
			}
		}
		statecpy(now2, start);
		statecpy(nex2, start);

		//���ø���Ŀ��СͼƬλ�� - ����Ŀ��С����
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

		//���ø�����ʼСͼƬλ�� - ������ʼС����
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

		//���ø�����ʾͼƬλ��
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

		//������ʾ������
		picNum = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 3; j++) {
				picNum = start[i][j];
				Block* p = new Block(pos[i][j], picNum);
				block2[i][j] = p;
				this->addChild(p->getPic(), 1);
			}
		}

		// Ԥ�ж�
		if (!(calDe(start) % 2 == calDe(target) % 2)) {
			isEnd = 1;
			maxStep = maxState;
		}
		// A*Ѱ·
		int res = Astar1(a2, start, target, path2);
		if (res == -1) {
			isEnd = 1;
			maxStep = maxState;
		}
		// shortest[]-�洢״̬��ţ�maxStep-�洢�ܲ���
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
		if (cnt2 == maxStep) {	// �տ�ʼ�޲���
			step = 0;
			cnt2 = cnt2 - 1;
		}
		else if (cnt2 >= 0) {
			// ��λ���ƶ���λ��
			int x0 = 0, y0 = 0;	//�հ�λ��
			int x1 = 0, y1 = 0;	//����λ��
			getBrack(now2, x0, y0);
			getState(a2, nex2, shortest2[cnt2]);
			getBrack(nex2, x1, y1);

			// ��ʼ�ƶ�
			cocos2d::MoveTo* move1 = cocos2d::MoveTo::create(0.5f, block2[x0][y0]->getPos());
			cocos2d::MoveTo* move2 = cocos2d::MoveTo::create(0.5f, block2[x1][y1]->getPos());

			auto effectID = CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("audio/move.mp3");
			block2[x0][y0]->getPic()->runAction(Sequence::create(move2->clone(), DelayTime::create(0.05f), NULL));
			block2[x1][y1]->getPic()->runAction(Sequence::create(move1->clone(), DelayTime::create(0.05f), NULL));

			Point po = block2[x0][y0]->getPos();
			block2[x0][y0]->setPos(block2[x1][y1]->getPos());
			block2[x1][y1]->setPos(po);
			std::swap(block2[x0][y0], block2[x1][y1]);

			// ׼����һ��
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
		if (isEnd == 1 && step == maxStep) {	//ȫ������
			initEndLayer(1);
			this->unschedule(schedule_selector(SelfTypeScene::endGame));
			this->unschedule(schedule_selector(SelfTypeScene::moveBlocks));
		}
		else if (isEnd == 1) {	//��ǰ��ֹ
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
	char resultName[30];
	sprintf(resultName, "pic/Result%d.png", result);
	auto menuBackground = Sprite::create(resultName);
	menuBackground->setPosition(origin.x + vSize.width / 2, origin.y + vSize.height / 2);
	pNode->addChild(menuBackground, 2);

	//���ذ�ť
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

	//�˳���ť
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

	//�����˵�
	auto moveMenu = Menu::create(returnGameItem, closeItem, NULL);
	moveMenu->setPosition(Vec2::ZERO);
	pNode->addChild(moveMenu, 3);

	this->addChild(pNode, 5);
}