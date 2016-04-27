#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    void onEnter();

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    bool onContactBegin(const cocos2d::PhysicsContact& contact); 

private:
    void setPhyWorld(cocos2d::PhysicsWorld *);  
    void initInternal(cocos2d::Size visibleSize);
    void createBall(cocos2d::Size visibleSize);
    void createAbstacles(cocos2d::Size visibleSize);
    cocos2d::PhysicsWorld *m_world;
};

#endif // __HELLOWORLD_SCENE_H__
