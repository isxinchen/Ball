#include "HelloWorldScene.h"

USING_NS_CC;

//物理引擎标识  
int ballMask = 1 << 0;//球1  
int wallMask = 1 << 1;//地面
int groundMask = 1 << 2;
int obstacleMask = 1 << 3;

#define BALL_RADIUS 40
#define OBSTACLE_RADIUS 10
#define OBSTACLE_SPACE 50
#define GROUND_OBJ_WIDTH = 10
#define GROUND_OBJ_HEIGHT = 100
#define GROUND_OBJ_SPACE 50

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
    Vect gravity(0, -9.8f);
    //scene->getPhysicsWorld()->setGravity(gravity);


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
        auto groundObstacle = Sprite::create();
        groundItem->setContentSize(Size(10, 100));  
        groundObstacle->setContentSize(Size(10, 10));  
        groundItem->setPosition(visibleSize.width/2 - 44/2 -44 * 4 - 10 * 5 + 5 + i * (10 + 44), 50);  
        groundObstacle->setPosition(visibleSize.width/2 - 44/2 -44 * 4 - 10 * 5 + 5 + i * (10 + 44), 100);  
        Vect anchorPoint=Vect(0.5f, 0.5f);  
        groundItem->setAnchorPoint(anchorPoint);
        groundObstacle->setAnchorPoint(anchorPoint);
        //创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质  
        //第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0  
        PhysicsBody* groundItemBody=PhysicsBody::createEdgeBox(groundItem->getContentSize(),PHYSICSBODY_MATERIAL_DEFAULT,0);  
        PhysicsBody* groundObstacleBody=PhysicsBody::createCircle(groundObstacle->getContentSize().width/2,PHYSICSBODY_MATERIAL_DEFAULT, Vec2::ZERO);  

        //是否设置物体为静态  
        groundItemBody->setDynamic(false);  
        //设置物体的恢复力  
        groundItemBody->getShape(0)->setRestitution(0.0f);  
        //设置物体的摩擦力  
        groundItemBody->getShape(0)->setFriction(0.0f);  
        //设置物体密度  
        groundItemBody->getShape(0)->setDensity(1.0f);  
        //设置质量  
        groundItemBody->getShape(0)->setMass(5000);  
        //设置物体是否受重力系数影响  
        groundItemBody->setGravityEnable(false);  

        //是否设置物体为静态  
        groundObstacleBody->setDynamic(false);  
        //设置物体的恢复力  
        groundObstacleBody->getShape(0)->setRestitution(0.5f);  
        //设置物体的摩擦力  
        groundObstacleBody->getShape(0)->setFriction(0.0f);  
        //设置物体密度  
        groundObstacleBody->getShape(0)->setDensity(1.0f);  
        //设置质量  
        groundObstacleBody->getShape(0)->setMass(5000);  
        //设置物体是否受重力系数影响  
        groundObstacleBody->setGravityEnable(false);  

        //设置物体的冲力  
        //Vect force=Vect(500000.0f, 500000.0f);  
        //ballBodyOne->applyImpulse(force);  
        //把物体添加到精灵中  
        groundItem->setPhysicsBody(groundItemBody);  
        groundObstacle->setPhysicsBody(groundObstacleBody);  
        //设置标志  
        groundItem->setTag(3);  
        groundObstacle->setTag(3);  
        this->addChild(groundItem);  
        this->addChild(groundObstacle);  

        groundItemBody->setCategoryBitmask(obstacleMask);  
        groundItemBody->setCollisionBitmask(obstacleMask | ballMask);  
        groundItemBody->setContactTestBitmask(obstacleMask | ballMask);  

        groundObstacleBody->setCategoryBitmask(obstacleMask);  
        groundObstacleBody->setCollisionBitmask(obstacleMask | ballMask);  
        groundObstacleBody->setContactTestBitmask(obstacleMask | ballMask);  
    }
}

void HelloWorld::createObstacles(Size visibleSize)
{
    int i = 0, j = 0;
    for(i = 0; i < 6; i++){
        for(j = 0; j < 11; j++){
            if(i % 2 == 0 && j == 10){
                break;
            }
            auto obstacle=Sprite::create();  
            obstacle->setContentSize(Size(10, 10));  
            if(i % 2){
                obstacle->setPosition(visibleSize.width/2 - 5 - 44*5 - 10 * 5 +5 + j*(10 + 44), 
                        visibleSize.height/2 + 44/2 + 2 * 44 + 10 * 3 - 5 - i * (44 + 10));  
            }else{
                obstacle->setPosition(visibleSize.width/2 - 44/2 -44*4 - 10*5 +5 + j*(10 + 44), 
                        visibleSize.height/2 + 44/2 + 2 * 44 + 10 * 3 - 5 - i * (44 + 10));  
            }
            Vect anchorPoint=Vect(0.5f, 0.5f);  
            obstacle->setAnchorPoint(anchorPoint);
            obstacle->setColor(Color3B(178, 178, 178));
            //创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质  
            //第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0  
            PhysicsBody* obstacleBody=PhysicsBody::createCircle(obstacle->getContentSize().width/2,PHYSICSBODY_MATERIAL_DEFAULT, Vec2(1,1));  

            //是否设置物体为静态  
            obstacleBody->setDynamic(false);  
            //设置物体的恢复力  
            obstacleBody->getShape(0)->setRestitution(0.5f);  
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

            obstacleBody->setCategoryBitmask(obstacleMask);  
            obstacleBody->setCollisionBitmask(obstacleMask | ballMask);  
            obstacleBody->setContactTestBitmask(obstacleMask | ballMask);  
        }
    }

}

void HelloWorld::createBall(Size visibleSize)
{
    Rect rect = Rect::ZERO;
    rect.size = Size(41, 41);
    auto ballOne=Sprite::create();  
    //auto ballOne=Sprite::create("ball.png", rect);  
    ballOne->setContentSize(Size(40, 40));  
    ballOne->setPosition(visibleSize.width/2 +20,visibleSize.height - 40);  
    //Vect anchorPoint=Vect(0.5f, 0.5f);  
    //ballOne->setAnchorPoint(anchorPoint);
    //创建物体，并且物体的形状为圆形，第一参数为半径，第二个参数为物体材质  
    //第三个参数为边的厚度,就是在Debug模式下看到的物体外面线条的厚度，默认为0  
    ballBodyOne=PhysicsBody::createCircle(ballOne->getContentSize().width/2,PHYSICSBODY_MATERIAL_DEFAULT, Vec2::ZERO);  
    //ballBodyOne->getShape(0)->setMoment(0.5f);//力矩
    ballBodyOne->setRotationEnable(false);
      
    //是否设置物体为静态  
    //ballBody->setDynamic(false);  
    //设置物体的恢复力  
    ballBodyOne->getShape(0)->setRestitution(1.0f);  
    //设置物体的摩擦力  
    ballBodyOne->getShape(0)->setFriction(0.0f);  
    //设置物体密度  
    ballBodyOne->getShape(0)->setDensity(7.9f);  
    //设置质量  
    ballBodyOne->getShape(0)->setMass(5000);  
    //设置物体是否受重力系数影响  
    ballBodyOne->setGravityEnable(false);  
      
    //设置物体的冲力  
    //Vect force=Vect(500000.0f, 0.0f);  
    //ballBodyOne->applyImpulse(force);  
    //设置物体初速度
    Vect vel=Vect(100.0f, 0.0f);  
    ballBodyOne->setVelocity(vel);
    //把物体添加到精灵中  
    ballOne->setPhysicsBody(ballBodyOne);  
    //设置标志  
    ballOne->setTag(1);  
    this->addChild(ballOne);  

    ballBodyOne->setCategoryBitmask(ballMask);  
    ballBodyOne->setCollisionBitmask(groundMask | wallMask | obstacleMask | ballMask);  
    ballBodyOne->setContactTestBitmask(groundMask | wallMask | obstacleMask | ballMask);  
}

void HelloWorld::initInternal(Size visibleSize)
{
    createBall(visibleSize);
    createObstacles(visibleSize);
    createGround(visibleSize);

    Sprite* edgeLeft = Sprite::create();
    Sprite* edgeRight = Sprite::create();
    Sprite* edgeBottom = Sprite::create();
    Vec2 l_s(0, 0), l_e(0,visibleSize.height),//left  
         r_s(visibleSize.width, 0), r_e(visibleSize.width, visibleSize.height), //right
         b_s(0, 0), b_e(visibleSize.width, 0);//bottom
    PhysicsBody* leftBody = PhysicsBody::createEdgeSegment(l_s, l_e, PHYSICSBODY_MATERIAL_DEFAULT, 5);
    PhysicsBody* rightBody = PhysicsBody::createEdgeSegment(r_s, r_e, PHYSICSBODY_MATERIAL_DEFAULT, 5);
    PhysicsBody* bottomBody = PhysicsBody::createEdgeSegment(b_s, b_e, PHYSICSBODY_MATERIAL_DEFAULT, 5);

    leftBody->getShape(0)->setFriction(0.0f);  
    rightBody->getShape(0)->setFriction(0.0f);  
    bottomBody->getShape(0)->setFriction(0.0f);  
    leftBody->getShape(0)->setRestitution(1.0f);  
    rightBody->getShape(0)->setRestitution(1.0f);  
    bottomBody->getShape(0)->setRestitution(0.0f);  

    edgeLeft->setPhysicsBody(leftBody);  
    Vect anchorPoint=Vect(0.5f, 0.5f);  
    edgeLeft->setAnchorPoint(anchorPoint);
    edgeLeft->setPosition(Point(0, 0));  
    this->addChild(edgeLeft);  
    edgeLeft->setTag(0);
    
    edgeRight->setPhysicsBody(rightBody);  
    edgeRight->setPosition(Point(0, 0));  
    edgeRight->setAnchorPoint(anchorPoint);
    this->addChild(edgeRight);  
    edgeRight->setTag(0);

    edgeBottom->setPhysicsBody(bottomBody);  
    edgeBottom->setPosition(Point(0, 0));  
    edgeBottom->setAnchorPoint(anchorPoint);
    this->addChild(edgeBottom);  
    edgeBottom->setTag(0);

    leftBody->setCategoryBitmask(wallMask);  
    leftBody->setCollisionBitmask(wallMask | ballMask);  
    leftBody->setContactTestBitmask(wallMask | ballMask);

    rightBody->setCategoryBitmask(wallMask);  
    rightBody->setCollisionBitmask(wallMask | ballMask);  
    rightBody->setContactTestBitmask(wallMask | ballMask);

    bottomBody->setCategoryBitmask(groundMask);  
    bottomBody->setCollisionBitmask(groundMask | ballMask);  
    bottomBody->setContactTestBitmask(groundMask | ballMask);
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
    initInternal(visibleSize);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    CCLOG ("%s, %d \n", __FUNCTION__, __LINE__ );
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
    CCLog ("%s, %d \n", __FUNCTION__, __LINE__ );
     
    if ((contact.getShapeA()->getBody()->getCategoryBitmask() & ballMask) == ballMask) {  
        CCLog("ball1 touch somethin A");  
    }  
    if ((contact.getShapeB()->getBody()->getCategoryBitmask() & wallMask) == wallMask) {  
        CCLog("ball1 touch wall");  
    }  
    if ((contact.getShapeB()->getBody()->getCategoryBitmask() & obstacleMask) == obstacleMask) {  
        CCLog("ball1 touch obstacle");  
    }  
    if ((contact.getShapeB()->getBody()->getCategoryBitmask() & groundMask) == groundMask) {  
        CCLog("ball1 touch ground");  
        ballBodyOne->getShape(0)->setRestitution(0.0f);  
        Size visibleSize = Director::getInstance()->getVisibleSize();
        createBall(visibleSize);
    }  

    if ((contact.getShapeB()->getBody()->getCategoryBitmask() & ballMask) == ballMask) {  
        CCLog("ball1 touch somethin A");  
    }  
    if ((contact.getShapeA()->getBody()->getCategoryBitmask() & wallMask) == wallMask) {  
        CCLog("ball1 touch wall");  
    }  
    if ((contact.getShapeA()->getBody()->getCategoryBitmask() & obstacleMask) == obstacleMask) {  
        CCLog("ball1 touch obstacle");  
    }  
    if ((contact.getShapeA()->getBody()->getCategoryBitmask() & groundMask) == groundMask) {  
        CCLog("ball1 touch ground");  
    }  

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
#else

#endif
      
    return true;  
}  
