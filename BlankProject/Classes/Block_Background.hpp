//
//  Block_Background.hpp
//  BlankProject-mobile
//
//  Created by Farah on 6/27/19.
//

#ifndef Block_Background_hpp
#define Block_Background_hpp

#include <cocos2d.h>
#include "Global.h"
using namespace cocos2d;

class Block_Background : public cocos2d::Node
{
public:
    
    virtual void onEnter();
    
    
    
    static Block_Background* createBgBlock();
    
    
    cocos2d::Sprite* backgroundSprite;
    cocos2d::Sprite* blockSprite;
    
    
   
    
};

#endif /* Block_Background_hpp */
