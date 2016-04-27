#include "HelloWorldScene.h"

USING_NS_CC;

//#define TWO_BALL


//物理引擎标识  
int ball1Mask = 1 << 0;//球1  
int ball2Mask = 1 << 1;//球2  
int wallMask = 1 << 2;//地面

Scene* HelloWorld::createScene()
{
#if 0
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
#endif

    //创建有物理空间的场景  
    Scene* scene=Scene::createWithPhysics();  

    //设置Debug模式，你会看到物体的表面被线条包围，主要为了在调试中更容易地观察  
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);  

    HelloWorld* layer=HelloWorld::create();  

    //把空间保持我们创建的层中，就是上面所说m_world的作用，方便后面设置空间的参数  
    layer->setPhyWorld(scene->getPhysicsWorld());  
    
    scene->addChild(layer);  
    return scene;
}
void HelloWorld::setPhyWorld(PhysicsWorld *world)
{
    m_world = world;
}

void HelloWorld::onEnter()  
{  
    Layer::onEnter();  
    //添加监听器  
    auto contactListener=EventListenerPhysicsContact::create();  
    //设置监听器的碰撞开始函数  
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);  
    //添加到事件分发器中  
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);  
}

void HelloWorld::createGround(Size visibleSize)
{
    int i = 0;
    for(i = 0; i < 10; i++){
        auto groundItem=Sprite::create();  
        groundItem->setContentSize(Size(10, 100));  
        groundItem->setPosition(visibleSize.width/2 - 5 - 44*5 - 10 * 5 +5 + i*(10 + 44), visibleSize.height/2- 44/2 -2*44-10*3 +5 - 100);  
        //创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质  
        //第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0  
        PhysicsBody* groundItemBody=PhysicsBody::createEdgeBox(groundItem->getContentSize(),PHYSICSBODY_MATERIAL_DEFAULT,1);  
        //  
        //ballBodyOne->setCategoryBitmask(1);  

        //是否设置物体为静态  
        groundItemBody->setDynamic(false);  
        //设置物体的恢复力  
        groundItemBody->getShape(0)->setRestitution(1.0f);  
        //设置物体的摩擦力  
        groundItemBody->getShape(0)->setFriction(0.0f);  
        //设置物体密度  
        groundItemBody->getShape(0)->setDensity(1.0f);  
        //设置质量  
        groundItemBody->getShape(0)->setMass(5000);  
        //设置物体是否受重力系数影响  
        groundItemBody->setGravityEnable(false);  

        //设置物体的冲力  
        //Vect force=Vect(500000.0f, 500000.0f);  
        //ballBodyOne->applyImpulse(force);  
        //把物体添加到精灵中  
        groundItem->setPhysicsBody(groundItemBody);  
        //设置标志  
        groundItem->setTag(3);  
        this->addChild(groundItem);  

        groundItemBody->setCategoryBitmask(ball2Mask);  
        groundItemBody->setCollisionBitmask(wallMask|ball1Mask|ball2Mask);  
        groundItemBody->setContactTestBitmask(wallMask|ball1Mask|ball2Mask);  
    }
}


void HelloWorld::createAbstacles(Size visibleSize)
{
    int i = 0, j = 0;
    for(i = 0; i < 6; i++){
        for(j = 0; j < 10; j++){
            auto obstacle=Sprite::create();  
            obstacle->setContentSize(Size(10, 10));  
            if(i % 2){
                obstacle->setPosition(visibleSize.width/2 - 5 - 44*5 - 10 * 5 +5 + j*(10 + 44), visibleSize.height/2- 44/2 -2*44-10*3 +5 + i*(44+10));  
            }else{
                obstacle->setPosition(visibleSize.width/2 - 44/2 -44*4 - 10*5 +5 + j*(10 + 44), visibleSize.height/2- 44/2 -2*44-10*3 +5 + i*(44+10) );  
            }
            //创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质  
            //第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0  
            PhysicsBody* obstacleBody=PhysicsBody::createCircle(obstacle->getContentSize().width/2,PHYSICSBODY_MATERIAL_DEFAULT, Vec2(1,1));  
            //  
            //ballBodyOne->setCategoryBitmask(1);  

            //是否设置物体为静态  
            obstacleBody->setDynamic(false);  
            //设置物体的恢复力  
            obstacleBody->getShape(0)->setRestitution(0.2f);  
            //设置物体的摩擦力  
            obstacleBody->getShape(0)->setFriction(0.0f);  
            //设置物体密度  
            obstacleBody->getShape(0)->setDensity(1.0f);  
            //设置质量  
            obstacleBody->getShape(0)->setMass(5000);  
            //设置物体是否受重力系数影响  
            obstacleBody->setGravityEnable(true);  

            //设置物体的冲力  
            //Vect force=Vect(500000.0f, 500000.0f);  
            //ballBodyOne->applyImpulse(force);  
            //把物体添加到精灵中  
            obstacle->setPhysicsBody(obstacleBody);  
            //设置标志  
            obstacle->setTag(2);  
            this->addChild(obstacle);  

            obstacleBody->setCategoryBitmask(ball2Mask);  
            obstacleBody->setCollisionBitmask(wallMask|ball1Mask|ball2Mask);  
            obstacleBody->setContactTestBitmask(wallMask|ball1Mask|ball2Mask);  
        }
    }

}

void HelloWorld::createBall(Size visibleSize)
{
    auto ballOne=Sprite::create();  
    ballOne->setContentSize(Size(40, 40));  
    ballOne->setPosition(visibleSize.width/2 +20,visibleSize.height - 40);  
    //创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质  
    //第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0  
    ballBodyOne=PhysicsBody::createCircle(ballOne->getContentSize().width/2,PHYSICSBODY_MATERIAL_DEFAULT, Vec2(1,1));  
    //  
    //ballBodyOne->setCategoryBitmask(1);  
      
    //是否设置物体为静态  
    //ballBody->setDynamic(false);  
    //设置物体的恢复力  
    ballBodyOne->getShape(0)->setRestitution(1.0f);  
    //设置物体的摩擦力  
    ballBodyOne->getShape(0)->setFriction(0.0f);  
    //设置物体密度  
    ballBodyOne->getShape(0)->setDensity(1.0f);  
    //设置质量  
    //ballBodyOne->getShape(0)->setMass(5000);  
    //设置物体是否受重力系数影响  
    ballBodyOne->setGravityEnable(false);  
      
    //设置物体的冲力  
    //Vect force=Vect(500000.0f, 0.0f);  
    //ballBodyOne->applyImpulse(force);  
    //设置物体初速度
    Vect vel=Vect(200.0f, 0.0f);  
    ballBodyOne->setVelocity(vel);
    //把物体添加到精灵中  
    ballOne->setPhysicsBody(ballBodyOne);  
    //设置标志  
    ballOne->setTag(1);  
    this->addChild(ballOne);  

    ballBodyOne->setCategoryBitmask(ball1Mask);  
    ballBodyOne->setCollisionBitmask(wallMask|ball1Mask|ball2Mask);  
    ballBodyOne->setContactTestBitmask(wallMask|ball1Mask|ball2Mask);  
}

void HelloWorld::initInternal(Size visibleSize)
{
    createBall(visibleSize);
    createAbstacles(visibleSize);
    createGround(visibleSize);

    Sprite* edgeSpace=Sprite::create();  
    PhysicsBody* boundBody=PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT,3);  
    boundBody->getShape(0)->setFriction(0.0f);  
    boundBody->getShape(0)->setRestitution(1.0f);  
      
    edgeSpace->setPhysicsBody(boundBody);  
    edgeSpace->setPosition(Point(visibleSize.width/2,visibleSize.height/2));  
    this->addChild(edgeSpace);  
    edgeSpace->setTag(0);
      
    boundBody->setCategoryBitmask(wallMask);  
    boundBody->setCollisionBitmask(wallMask | ball1Mask|ball2Mask);  
    boundBody->setContactTestBitmask(wallMask|ball1Mask|ball2Mask);


}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
#if 0
    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    
    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);
#endif 
      
#ifdef TWO_BALL
    auto ballOne=Sprite::create();  
    ballOne->setContentSize(Size(50, 50));  
    ballOne->setPosition(visibleSize.width/2,visibleSize.height/2);  
    //创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质  
    //第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0  
    PhysicsBody* ballBodyOne=PhysicsBody::createCircle(ballOne->getContentSize().width/2,PHYSICSBODY_MATERIAL_DEFAULT, Vec2(1,1));  
    //  
    //ballBodyOne->setCategoryBitmask(1);  
      
    //是否设置物体为静态  
    //ballBody->setDynamic(false);  
    //设置物体的恢复力  
    ballBodyOne->getShape(0)->setRestitution(1.0f);  
    //设置物体的摩擦力  
    ballBodyOne->getShape(0)->setFriction(0.0f);  
    //设置物体密度  
    ballBodyOne->getShape(0)->setDensity(1.0f);  
    //设置质量  
    //ballBodyOne->getShape(0)->setMass(5000);  
    //设置物体是否受重力系数影响  
    ballBodyOne->setGravityEnable(false);  
      
    //设置物体的冲力  
    Vect force=Vect(500000.0f, 500000.0f);  
    ballBodyOne->applyImpulse(force);  
    //把物体添加到精灵中  
    ballOne->setPhysicsBody(ballBodyOne);  
    //设置标志  
    ballOne->setTag(1);  
    this->addChild(ballOne);  
      
    //设置第二个球  
    auto ballTwo=Sprite::create();  
    ballTwo->setContentSize(Size(100, 100));  
    ballTwo->setPosition(visibleSize.width/3,visibleSize.height/3);  
    PhysicsBody* ballBodyTwo=PhysicsBody::createCircle(ballOne->getContentSize().width/2,PHYSICSBODY_MATERIAL_DEFAULT);  
    //是否设置物体为静态  
    //ballBodyTwo->setDynamic(false);  
    ballBodyTwo->getShape(0)->setRestitution(1.0f);  
    ballBodyTwo->getShape(0)->setFriction(0.0f);  
    ballBodyTwo->getShape(0)->setDensity(1.0f);  
      
    ballBodyTwo->setGravityEnable(false);
    force=Vect(-500000.0f, -500000.0f);  
    ballBodyTwo->applyImpulse(force);  
    ballTwo->setPhysicsBody(ballBodyTwo);  
    ballTwo->setTag(2);  
    this->addChild(ballTwo);  
      
    //创建一个盒子，用来碰撞  
    Sprite* edgeSpace=Sprite::create();  
    PhysicsBody* boundBody=PhysicsBody::createEdgeBox(visibleSize,PHYSICSBODY_MATERIAL_DEFAULT,3);  
    boundBody->getShape(0)->setFriction(0.0f);  
    boundBody->getShape(0)->setRestitution(1.0f);  
      
    edgeSpace->setPhysicsBody(boundBody);  
    edgeSpace->setPosition(Point(visibleSize.width/2,visibleSize.height/2));  
    this->addChild(edgeSpace);  
    edgeSpace->setTag(0);  
      
      
      
      
    ballBodyOne->setCategoryBitmask(ball1Mask);  
    ballBodyOne->setCollisionBitmask(wallMask|ball1Mask|ball2Mask);  
    ballBodyOne->setContactTestBitmask(wallMask|ball1Mask|ball2Mask);  
      
    ballBodyTwo->setCategoryBitmask(ball2Mask);  
    ballBodyTwo->setCollisionBitmask(wallMask|ball1Mask|ball2Mask);  
    ballBodyTwo->setContactTestBitmask(wallMask|ball1Mask|ball2Mask);  
      
    boundBody->setCategoryBitmask(wallMask);  
    boundBody->setCollisionBitmask(wallMask | ball1Mask|ball2Mask);  
    boundBody->setContactTestBitmask(wallMask|ball1Mask|ball2Mask);
#else
    initInternal(visibleSize);
#endif

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Director::getInstance()->end();
    Vect vel=Vect(0.0f, 0.0f);  
    ballBodyOne->setVelocity(vel);
    ballBodyOne->setGravityEnable(true);  

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

bool HelloWorld::onContactBegin(const PhysicsContact& contact)  
{  
     
#ifdef TWO_BALL
    if ((contact.getShapeA()->getBody()->getCategoryBitmask() & ball1Mask) == ball1Mask) {  
        CCLOG("ball1 touch something");  
    }  
    if ((contact.getShapeB()->getBody()->getCategoryBitmask() & ball1Mask) == ball1Mask) {  
        CCLOG("ball1 touch something");  
    }  
      
      
    if ((contact.getShapeA()->getBody()->getCategoryBitmask() & ball2Mask) == ball2Mask) {  
        CCLOG("ball2 touch something");  
    }  
    if ((contact.getShapeB()->getBody()->getCategoryBitmask() & ball2Mask) == ball2Mask) {  
        CCLOG("ball2 touch something");  
    }  
#endif
      
    return true;  
}  
