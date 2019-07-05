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
    }
    else
        CC_SAFE_DELETE(block);
    
    return nullptr;
}
void Block::explode()
{
    auto explodeParticles = ParticleSystemQuad::create("explode.plist");
    explodeParticles->setAutoRemoveOnFinish(true);
    explodeParticles->setPosition(getPosition());
    getParent()->addChild(explodeParticles, 10);
}

void Block::setType(BlockType type)
{
    this->type = type;
    if (blockSprite != nullptr)
    {
        blockSprite->setTexture(BlockTypeToFrameName[static_cast<int>(type)]);
    }
}
void Block::remove_animation(float start_animation, float end_animation)
{
    runAction(Sequence::create(ScaleTo::create(0.5, 0), MoveTo::create(1, Vec2(start_animation,end_animation)),NULL));
}
void Block::onEnter()
{
    Node::onEnter();
    setContentSize(_BlockSize);
    setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    if(Box_check_odd%2==0)
//    {
//        backgroundSprite = Sprite::create("Light_box.png");
//    }
//    else
//
//    {
//        backgroundSprite = Sprite::create("Dark_box.png");
//    }
//    Box_check_odd++;
//
//    backgroundSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
//    backgroundSprite->setPosition(_BlockSize * 0.5f);
//    addChild(backgroundSprite, 0);
    
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
