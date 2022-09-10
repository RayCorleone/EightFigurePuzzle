#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::extension;

class Block
{
private:
	int state;	// ״̬0-8
	cocos2d::Point pos;	// λ��
	std::string name;	// ͼƬ��
	cocos2d::Sprite* pic;	// ͼƬ

public:
	Block(cocos2d::Point pos, int state);
	~Block();

	cocos2d::Point getPos();
	cocos2d::Sprite* getPic();
	int getState();
	std::string getName();

	void setState(const int newState);
	void setPos(cocos2d::Point pos);
};

#endif // !__BLOCK_H__
