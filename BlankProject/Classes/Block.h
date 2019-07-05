//
//  Block.h
//  MatchThree
//
//  Created by Dimitar Dimitrov on 5/2/16.
//
//

#ifndef MATCH_THREE_BLOCK_H
#define MATCH_THREE_BLOCK_H

#include <cocos2d.h>
#include "Global.h"
using namespace cocos2d;

class Block : public cocos2d::Node
{
public:
	
    virtual void onEnter();
    void explode();
    
 
    
    static Block* createBlock(BlockType type, GridPosition gridPosition);
   
    
    
    cocos2d::Sprite* backgroundSprite;
    cocos2d::Sprite* blockSprite;
   
    void remove_animation(float start_animation, float end_animation);
    
    
    // Declare In Global Class
    BlockType type;
    GridPosition gridPosition;
    
    void setActive(bool isActive);
    BlockType getType()
    {
        return type;
    }
    void setType(BlockType type);
    
   
};

#endif /* _MATCH_THREE_BLOCK_H_ */
