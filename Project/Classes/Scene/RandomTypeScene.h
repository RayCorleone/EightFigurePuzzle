#ifndef __RANDOM_TYPESCENE_H__
#define __RANDOM_TYPESCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;
using namespace cocos2d::extension;

class RandomTypeScene :public cocos2d::Scene
{
private:
    cocos2d::Point pos[3][3];    //方框图片的位置
    int start[3][3] = { 1,2,3,8,0,4,7,6,5 };    //初始棋盘
    int target[3][3] = { 1,2,3,8,0,4,7,6,5 };   //目标棋盘
    bool isEnd = 0;     // 结束标志
    int maxStep = 0;    // 总步数
    int step = 0;       // 当前步数
    LabelTTF* stepLabel;

public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(RandomTypeScene);

    void initStillNode();
    void initMoveLayer();
    void initGameLayer();
    void initMenu(cocos2d::Node* pNode);

    void gameCloseCallback(cocos2d::Ref* pSender);
    void musicSliderCallBack(cocos2d::Ref* pSender, Control::EventType type);
    void effectSliderCallBack(cocos2d::Ref* pSender, Control::EventType type);
    void getSetUpLayer(cocos2d::Ref* pSender, cocos2d::Node* pNode);
    void menuClose(cocos2d::Ref* pSender, cocos2d::Node* pNode);
    void returnCallBack(cocos2d::Ref* pSender);

    void update(float dt);
    void moveBlocks(float dt);

    void endGame(float dt);
    void initEndLayer(const int result);
};

#endif // __RANDOM_TYPESCENE_H__