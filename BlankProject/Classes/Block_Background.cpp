//
//  Block_Background_Background.cpp
//  BlankProject-mobile
//
//  Created by Farah on 6/27/19.
//

#include "Block_Background.hpp"

#include "Global.h"
USING_NS_CC;


Block_Background* Block_Background::createBgBlock()
{
    Block_Background* block =  new (std::nothrow) Block_Background();
    if (block && block->init())
    {
        block->autorelease();
       
        
        return block;
    } else
        CC_SAFE_DELETE(block);
    
    return nullptr;
}
void Block_Background::onEnter()
{
    Node::onEnter();
    
    setContentSize(_BlockSize);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    if(Box_check_odd%2==0)
    {
        backgroundSprite = Sprite::create("Light_box.png");
    }
    else
        
    {
        backgroundSprite = Sprite::create("Dark_box.png");
    }
    Box_check_odd++;
    
    backgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    backgroundSprite->setPosition(_BlockSize * 0.5f);
    addChild(backgroundSprite, 0);
    
   
}
