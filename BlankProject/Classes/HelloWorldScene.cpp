/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful err or message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    //Sprite *Background;
    //Background=Sprite::create("dressup-screen-bg.png");
    //Background->setPosition(Vec2(768/2, 1024/2));
    //this->addChild(Background);
    
    
    Size screenSize = Director::getInstance()->getVisibleSize();
    
    
    grid = Grid::createGrid(MAX_ROWS, MAX_COLS);
    grid->setPosition(screenSize.width * 0.5f - grid->getContentSize().width * 0.5f,
                      screenSize.height * 0.5f - grid->getContentSize().height * 0.5f - 30);
    addChild(grid, 1);
    grid->generateRandomBlocks();

    
    
    //activeBlock = nullptr;
    
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    
    
    
    return true;
}
#pragma mark Touches
bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 point = touch->getLocation();
    
//    if (grid->isBusy() || grid->getBoundingBox().containsPoint(point) == false)
//        return false;
    
    auto block = grid->getBlockForPosition(point);
    if (block == nullptr || block == activeBlock)
        return true;
    
    if (activeBlock != nullptr)
    {
        // If blocks are neighbours try to swap them
        if (grid->areNeighbours(activeBlock, block))
        {
            swapBlocks(block, activeBlock);
            runAction(Sequence::create(
                                       DelayTime::create(0.3f), // Wait for the animation to end
                                       CallFunc::create(CC_CALLBACK_0(HelloWorld::checkForMatches, this, activeBlock, block)),
                                       nullptr));
            
            activeBlock->setActive(false);
            block->setActive(false);
            activeBlock = nullptr;
        }
        else // If not set active the selected one
        {
            activeBlock->setActive(false);
            activeBlock = block;
            activeBlock->setActive(true);
        }
    }
    else
    {
        activeBlock = block;
        activeBlock->setActive(true);
    }
    
    return true;
}

// Detect swipes
void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
//    if (isBusy || grid->isBusy() || activeBlock == nullptr)
//        return;
    
    
    if (activeBlock == nullptr)
        return;
    
    Vec2 touchDistance = touch->getLocation() - touch->getPreviousLocation();
    Block* blockToSwap = nullptr;
    
    // Right
    if (touchDistance.x >= SWIPE_TRESHOLD && (activeBlock->gridPosition.row + 1 < MAX_ROWS))
        blockToSwap = grid->blocks[activeBlock->gridPosition.row + 1][activeBlock->gridPosition.col];
    
    // Left
    if (touchDistance.x <= -SWIPE_TRESHOLD && (activeBlock->gridPosition.row > 0))
        blockToSwap = grid->blocks[activeBlock->gridPosition.row - 1][activeBlock->gridPosition.col];
    
    // Up
    if (touchDistance.y >= SWIPE_TRESHOLD && (activeBlock->gridPosition.col + 1 < MAX_COLS))
        blockToSwap = grid->blocks[activeBlock->gridPosition.row][activeBlock->gridPosition.col + 1];
    
    // Down
    if (touchDistance.y <= -SWIPE_TRESHOLD && (activeBlock->gridPosition.col > 0))
        blockToSwap = grid->blocks[activeBlock->gridPosition.row][activeBlock->gridPosition.col - 1];
    
    if (blockToSwap != nullptr)
    {
        swapBlocks(blockToSwap, activeBlock);
        runAction(Sequence::create(
                                   DelayTime::create(0.3f), // Wait for the animation to end
                                   CallFunc::create(CC_CALLBACK_0(HelloWorld::checkForMatches, this, activeBlock, blockToSwap)),
                                   nullptr));
        
        activeBlock->setActive(false);
        activeBlock = nullptr;
    }
    
}


void HelloWorld::checkForMatches(Block* first, Block* second)
{
    bool firstMatches = grid->checkForMatches(first);
    bool secondMatches = grid->checkForMatches(second);
    
    if (firstMatches == false && secondMatches == false)
    {
        // No matches swap them back and return
        swapBlocks(first, second);
        return;
    }
    
    if (firstMatches)
        resolveMatchesForBlock(first);
    
    if (secondMatches)
        resolveMatchesForBlock(second);
    
    if (firstMatches || secondMatches)
    {
        grid->fillBlanks();
        activeBlock = nullptr;
    }
}

void HelloWorld::resolveMatchesForBlock(Block* block)
{
    if (block == nullptr)
        return;
    
    std::vector<Block*> matches;
    int num_matches = grid->findMatches(block, matches);
    //addScore(num_matches);
    
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pop.wav");
    
    for (auto match : matches)
        grid->removeBlockAt(match->gridPosition);
}



void HelloWorld::swapBlocks(Block* first, Block* second)
{
    grid->swapBlocks(first, second);
    first->runAction(MoveTo::create(0.3f, second->getPosition()));
    second->runAction(MoveTo::create(0.3f, first->getPosition()));
}




void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
