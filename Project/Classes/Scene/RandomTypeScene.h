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
    cocos2d::Point pos[3][3];    //����ͼƬ��λ��
    int start[3][3] = { 1,2,3,8,0,4,7,6,5 };    //��ʼ����
    int target[3][3] = { 1,2,3,8,0,4,7,6,5 };   //Ŀ������
    bool isEnd = 0;     // ������־
    int maxStep = 0;    // �ܲ���
    int step = 0;       // ��ǰ����
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