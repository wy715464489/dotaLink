//
//  BattleMap.cpp
//  dotaLink
//
//  Created by 秦亮亮 on 14-6-30.
//
//

#include "BattleMap.h"
Scene* BattleMap::createScene()
{
	auto scene = Scene::create();
    auto layer = BattleMap::create();
    Manager::getInstance()->bmap=layer;

    scene->addChild(layer);
	return scene;
}

bool BattleMap::init()
{
    Node::init();
	//对cocostudio 设计的ui进行调整
    //std::string fileName="TileMap.tmx";
    //std::string fileName="stormwind.tmx";
    std::string fileName="map2.tmx";
    auto str=String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename(fileName.c_str()).c_str());
    
    this->tileMap=TMXTiledMap::createWithXML(str->getCString(), "");
    this->createHero(1);
    this->setViewPointCenter(heroTeam->getLeader()->getPosition());
    
    this->coverLayer=tileMap->getLayer("coverLayer");
    this->blockLayer=tileMap->getLayer("blockLayer");
    this->blockLayer->setVisible(false);
    this->addChild(tileMap, -1,100);
   

	auto listener = EventListenerTouchOneByOne::create();
    
	//lambda expression: advanced feature in C++ 11
	listener->onTouchBegan = [&](Touch *touch, Event *unused_event)->bool {return true;};
	listener->onTouchEnded = CC_CALLBACK_2(BattleMap::onTouchEnded, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    this->scheduleUpdate();
	return true;
}

void BattleMap::onEnter()
{
    Node::onEnter();
}

void BattleMap::createHero(int type)
{
    TMXObjectGroup* objectGroup=this->tileMap->getObjectGroup("items");
    if(type==1){
        auto born=objectGroup->getObject("hero");
        float x=born["x"].asDouble();
        float y=born["y"].asDouble();
        Hero* player=Hero::create(101);
        player->type=type;
        player->setPosition(x,y);
        this->heroTeam=Team::create(player);
        tileMap->addChild(heroTeam,0);
        
        MNpc* mh=MNpc::create(player);
        this->team.pushBack(mh);
        
//        auto hero=Hero::create(102);
//        hero->type=type;
//        mh=MNpc::create(hero);
//        hero->follow(this->heroTeam->getLeader());
//        this->heroTeam->addMember(hero);
    }
//    type=2;
//    int ids[6]={102,103,201,301,302,401};
//    if(type==2){
//        ValueVector vec=objectGroup->getObjects();
//        for(int i=0;i<vec.size();i++){
//            ValueMap v=vec[i].asValueMap();
//            if(v["name"].asString()=="npc"){
//                Npc* npc=Npc::create(ids[rand()%6]);
//                npc->type=type;
//                float x=v["x"].asDouble();
//                float y=v["y"].asDouble();
//                this->tileMap->addChild(npc);
//                npc->setPosition(x,y);
//                npc->run();
//                MNpc* mNpc=MNpc::create(npc);
//                this->npcs.pushBack(mNpc);
//
//            }
//        }
//    }
}

void BattleMap::setViewPointCenter(cocos2d::Point position)
{
    auto winSize = Director::getInstance()->getWinSize();
    float x = MAX(position.x, winSize.width / 2);
    float y = MAX(position.y, winSize.height / 2);
    x = MIN(x, (tileMap->getMapSize().width * this->tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (tileMap->getMapSize().height * tileMap->getTileSize().height) - winSize.height / 2);
    auto actualPosition = Point(x, y);
    
    auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
    auto viewPoint = centerOfView - actualPosition;
    this->setPosition(viewPoint);
}

void BattleMap::setPlayerPosition(cocos2d::Point position)
{
    /*
    Point tileCoord = this->tileCoordForPosition(position);
    int tileGid = meta->getTileGIDAt(tileCoord);
    if (tileGid) {
        auto properties = tileMap->getPropertiesForGID(tileGid).asValueMap();
        if (!properties.empty()) {
            auto collision = properties["Collidable"].asString();
            if ("true" == collision) {
                return;
            }
        }
    }
     */
    //this->player->setPosition(position);
//    this->player->run();
//    this->player->run(position);
//    auto collectable = this->tileMap->getPropertiesForGID(tileGid)["Collectable"].asString();
//    if ("True" == collectable) {
//        meta->removeTileAt(tileCoord);
//        foreground->removeTileAt(tileCoord);
//        
//        this->numCollected++;
//        this->hud->numCollectedChanged(_numCollected);
//    }
}

Point BattleMap::tileCoordForPosition(Point position)
{
    int x = position.x / tileMap->getTileSize().width+1;
    int y =((tileMap->getMapSize().height * tileMap->getTileSize().height) - position.y) / tileMap->getTileSize().height-1;
    return Point(x, y);
}


void BattleMap::pickUp()
{
//    for(Hero* hero : team)
    {
//        Point tileCoord = this->tileCoordForPosition(hero->getPosition());
        //int tileGid = this->tileLayer->getTileGIDAt(tileCoord);
//        auto collectable = this->tileMap->getPropertiesForGID(tileGid)["Collectable"].asString();
//        if ("True" == collectable) {
//                meta->removeTileAt(tileCoord);
//                foreground->removeTileAt(tileCoord);
//    
//               this->numCollected++;
//               this->hud->numCollectedChanged(_numCollected);
//        }
    }
}

void BattleMap::update(float dt)
{
    this->setViewPointCenter(heroTeam->getLeader()->getPosition());
    this->repositionHero();
}

void BattleMap::repositionHero()
{
    auto p = this->heroTeam->getLeader()->getPosition();
    p = CC_POINT_POINTS_TO_PIXELS(p);
    Sprite* sprite=this->coverLayer->getTileAt(this->tileCoordForPosition(p));
    if(sprite){
        auto sp=sprite->getPosition();
        if(sp.y+75>p.y){
            this->tileMap->reorderChild(heroTeam, 1);
        } else{
            this->tileMap->reorderChild(heroTeam, 0);
        }
    }
}

bool BattleMap::isblock(Vec2 pos)
{
    /*
    pos=CC_POINT_POINTS_TO_PIXELS(pos);
    TMXObjectGroup* objectGroup=this->tileMap->getObjectGroup("block");
    auto left=objectGroup->getObject("left");
    auto right=objectGroup->getObject("right");
    auto top=objectGroup->getObject("top");
    auto bottom=objectGroup->getObject("bottom");
    
    if(pos.x<=left["width"].asDouble()    || pos.x>=right["x"].asDouble()||
       pos.y<=bottom["height"].asDouble() || pos.y>=top["y"].asDouble())
    {
        return true;
    } 
    */
    Vec2 grid=this->tileCoordForPosition(pos);
    int tileGrid=this->blockLayer->getTileGIDAt(grid);
    Value properties=tileMap->getPropertiesForGID(tileGrid);
    if(properties.getType()==Value::Type::MAP){
        ValueMap map=properties.asValueMap();
        auto isBlock = map["isBlock"].asString();
        if ("true" == isBlock) {
            return true;
        }
    }
    return false;
}

Vector<MNpc*> BattleMap::getTargets(std::vector<Vec2> arr,int type)
{
    Vector<MNpc*> targets;
    Vector<MNpc*> npcs=type==0?this->npcs:this->team;
    for(int i=0;i<npcs.size();i++){
        MNpc* npc=npcs.at(i);
        Vec2 grid=this->tileCoordForPosition(npc->view->getPosition());
        for(int j=0;j<arr.size();j++){
            if (grid.x==arr.at(j).x && grid.y==arr.at(j).y) {
                targets.pushBack(npc);
                break;
            }
        }
    }
    return targets;
}

void BattleMap::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    auto touchPoint = touch->getLocation();
    auto startPoint=touch->getStartLocation();
    touchPoint = this->convertToNodeSpace(touchPoint);
    startPoint = this->convertToNodeSpace(startPoint);
    float dx=touchPoint.x-startPoint.x;
    float dy=touchPoint.y-startPoint.y;
    if(dx>0 && fabs(dx)>fabs(dy)) this->heroTeam->setDir(Dir::right);
    if(dx<0 && fabs(dx)>fabs(dy)) this->heroTeam->setDir(Dir::left);
    if(dy>0 && fabs(dy)>fabs(dx)) this->heroTeam->setDir(Dir::top);
    if(dy<0 && fabs(dy)>fabs(dx)) this->heroTeam->setDir(Dir::bottom);
}

void BattleMap::onExit()
{

}