//
//  Block.cpp
//  MatchThree
//
//  Created by Dimitar Dimitrov on 5/2/16.
//
//

#include "Block.h"
#include "Global.h"
USING_NS_CC;


Block* Block::createBlock(BlockType type, GridPosition gridPosition)
{
    Block* block =  new (std::nothrow) Block();
    if (block && block->init())
    {
        block->autorelease();
        block->type = type;
        block->gridPosition = gridPosition;
        
        return block;
    } else
        CC_SAFE_DELETE(block);
    
    return nullptr;
}
void Block::setType(BlockType type)
{
    this->type = type;
    
    if (blockSprite != nullptr)
    {
        blockSprite->setTexture(BlockTypeToFrameName[static_cast<int>(type)]);
    }
    
}
void Block::onEnter()
{
    Node::onEnter();
    
    setContentSize(_BlockSize);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    
    backgroundSprite = Sprite::create("back.png");
    backgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    backgroundSprite->setPosition(_BlockSize * 0.5f);
    addChild(backgroundSprite, 0);
    
    blockSprite = Sprite::create(BlockTypeToFrameName[static_cast<int>(type)]);
    blockSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    blockSprite->setPosition(_BlockSize * 0.5f);
    addChild(blockSprite, 1);
}
void Block::setActive(bool isActive)
{
//    if (isActive)
//        backgroundSprite->setSpriteFrame("back_active.png");
//    else
//        backgroundSprite->setSpriteFrame("back.png");
}
