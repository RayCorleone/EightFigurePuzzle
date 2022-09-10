#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "Classes/Block/Block.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::extension;

Block::Block(cocos2d::Point pos, int state)
{
	this->pos = pos;
	this->state = state;
	char picName[15];
	sprintf(picName, "block/b%d.png", state);
	this->name = picName;
	pic = Sprite::create(this->name);
	pic->setPosition(pos);
	pic->setScale(1.3f);
}

Block::~Block()
{
}

//Get系列
cocos2d::Sprite* Block::getPic() {
	return pic;
}

cocos2d::Point Block::getPos() {
	return pos;
}

int Block::getState() {
	return state;
}

std::string Block::getName() {
	return name;
}

//Set系列
void Block::setPos(cocos2d::Point pos) {
	this->pos = pos;
}

void Block::setState(const int newState) {
	this->state = newState;
	char picName[15];
	sprintf(picName, "block/b%d.png", newState);
	this->name = picName;
	pic->setScale(0.0f);
	pic = Sprite::create(picName);
	pic->setPosition(pos);
	pic->setScale(1.3f);
}