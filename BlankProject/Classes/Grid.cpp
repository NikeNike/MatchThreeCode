//
//  Grid.cpp
//  MatchThree
//
//  Created by Dimitar Dimitrov on 5/2/16.
//
//

#include "Grid.h"
#include "SimpleAudioEngine.h"
#include "Global.h"
#include "Block_Background.hpp"
USING_NS_CC;




bool Grid::checkForMatches(Block* block)
{
    if (!block)
        return false;
    
    GridPosition gp = block->gridPosition;
    
    
    int matches = 1;
    int row = gp.row;
    
    // Check horizontal
    while(--row > -1 && blocks[row][gp.col] && blocks[row][gp.col]->getType() == block->getType())  // Left
    {
        matches++;
    }
    
    row = gp.row;
    while(++row < MAX_ROWS && blocks[row][gp.col] && blocks[row][gp.col]->getType() == block->getType())// Check right
    {
        matches++;
    }
    
    if (matches >= MAX_MATCHES)
    {
        return true;
    }
    
    
    matches = 1;
    int col = gp.col;
    // Check vertical
    while(--col > -1 && blocks[gp.row][col] && blocks[gp.row][col]->getType() == block->getType())
    {// Check down
        matches++;
    }
    col = gp.col;
    while(++col < MAX_COLS && blocks[gp.row][col] && blocks[gp.row][col]->getType() == block->getType())
    {// Check up
        matches++;
    }
    
    return (matches >= MAX_MATCHES);
    
}
void Grid::rest_store_value()
{
    //    for (int i = 0; i <= 99; i++)
    //    {
    //         store_row[i] = 0;
    //         store_col[i] = 0;
    //    }
}
int Grid::findMatches(Block* block, std::vector<Block*> &matches, bool isRoot)
{
    if (isRoot)
        matches.clear();
    
    rest_store_value();
    matches.push_back(block);
    
    
    GridPosition gp = block->gridPosition;
    addedPosition=block->gridPosition;
    
    int matchcount = 1;
    int row = gp.row;
    
    
    while(--row > -1 && blocks[row][gp.col] && blocks[row][gp.col]->getType() == block->getType())  // Left
    {
        CCLOG("Rowww slider%i",row);
        matchcount++;
        store_block[matchcount] = blocks[row][gp.col];
        isVertical=false;
    }
    
    row = gp.row;
    while(++row < MAX_ROWS && blocks[row][gp.col] && blocks[row][gp.col]->getType() == block->getType())// Check right
    {
        CCLOG("Rowww slider%i",row);
        matchcount++;
        store_block[matchcount] = blocks[row][gp.col];
        isVertical=false;
    }
    
    
    if (matchcount >= MAX_MATCHES)
    {
        for (int i = 2; i <= matchcount; i++)
        {
            matches.push_back(store_block[i]);
        }
    }
    
    
    
    
    int col = gp.col;
    matchcount = 1;
    while(--col > -1 && blocks[gp.row][col] && blocks[gp.row][col]->getType() == block->getType())
    {
        matchcount++;
        store_block[matchcount] = blocks[gp.row][col];
        isVertical=true;
        
    }
    
    col = gp.col;
    while(++col < MAX_COLS && blocks[gp.row][col] && blocks[gp.row][col]->getType() == block->getType())
    {
        matchcount++;
        store_block[matchcount] = blocks[gp.row][col];
        isVertical=true;
    }
    
    if (matchcount >= MAX_MATCHES)
    {
        for (int i = 2; i <= matchcount; i++)
        {
            matches.push_back(store_block[i]);
        }
    }
    
    
    
    
    if(matches.size()==4)
    {
        CCLOG("matchcount Counter %ld",matches.size());
        SliderBooster=true;
    }
    
    
    
    return matches.size();
}


void Grid::resolveMatchesForBlocks(const std::vector<Block*>& blocks)
{
    int numMatches = 0;
    std::vector<Block*> blocksToRemove;
    
    for (auto block : blocks)
    {
        if (checkForMatches(block))
        {
            std::vector<Block*> matches;
            numMatches += findMatches(block, matches);
            
            for (auto match : matches)
            {
                if (std::find(blocksToRemove.begin(), blocksToRemove.end(), match) == blocksToRemove.end())
                    blocksToRemove.push_back(match);
            }
        }
    }
    
    if (numMatches > 0)
    {
        EventCustom haveMatchesEvent(EVENT_GRID_HAVE_MATCHES);
        EventMatchesData em;
        em.matches = numMatches;
        haveMatchesEvent.setUserData((void *) &em);
        _eventDispatcher->dispatchEvent(&haveMatchesEvent);
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("pop.wav");
        for (auto block : blocksToRemove)
            removeBlockAt(block->gridPosition);
        
        fillBlanks();
    }
    else
    {
        EventCustom gridReadyEvent(EVENT_GRID_READY);
        _eventDispatcher->dispatchEvent(&gridReadyEvent);
        
        // busy = false;
    }
}



void Grid::swapBlocks(Block* first, Block* second)
{
    if (!first || !second)
        return;
    
    if (!areNeighbours(first, second))
    {
        CCLOGERROR("Blocks [%d, %d] and [%d, %d] are not neighbours!",
                   first->gridPosition.row,
                   first->gridPosition.col,
                   second->gridPosition.row,
                   second->gridPosition.col
                   );
        return;
    }
    
    blocks[first->gridPosition.row][first->gridPosition.col] = second;
    blocks[second->gridPosition.row][second->gridPosition.col] = first;
    
    // Swap blocks indexes
    GridPosition temp = first->gridPosition;
    first->gridPosition = second->gridPosition;
    second->gridPosition = temp;
}
bool Grid::areNeighbours(Block* first, Block* second)
{
    if (!first || !second)
        return false;
    int x = abs(first->gridPosition.row - second->gridPosition.row);
    int y = abs(first->gridPosition.col - second->gridPosition.col);
    if(x==1 && y==0)
    {
        CCLOG("column same");
    }
    if(x==0 && y==1)
    {
        CCLOG("row same");
    }
    return (x == 1 && y == 0) || (x == 0 && y == 1);
}
Block* Grid::getBlockForPosition(cocos2d::Vec2 position)
{
    unsigned int row = (unsigned int) ceil((position.x - getPosition().x) / _BlockSize.width) - 1;
    unsigned int col = (unsigned int) ceil((position.y - getPosition().y) / _BlockSize.height) - 1;
    
    //    CCASSERT((col < MAX_COLS && row < MAX_ROWS), "Incorrect row/col. Index out of range");
    
    if (blocks[row][col])
    {
        return blocks[row][col];
    }
    
    return nullptr;
}



void Grid::generateRandomBlocks()
{
    
    for (int col = 0; col < MAX_COLS; col++)
    {
        for (int row = 0; row < MAX_ROWS; row++)
        {
            
            
            Block_Background* block_bg = Block_Background::createBgBlock();
            
            block_bg->setPosition(row * _BlockSize.width + _BlockSize.width * 0.5f,
                                  col * _BlockSize.height + _BlockSize.height * 0.5f);
            
            addChild(block_bg, -1);
            
            
            Block* block = Block::createBlock((BlockType)random((int)BlockType::APPLE, (int)BlockType::ORANGE), {row, col});
            
            // Make sure there are no initial groups of matches
            while(checkForMatches(block) == true)
            {
                block->setType((BlockType)random((int)BlockType::APPLE, (int)BlockType::ORANGE));
            }
            //            CCLOG("%d row%f  col %f",turn,row * _BlockSize.width + _BlockSize.width * 0.5f,col * _BlockSize.width + _BlockSize.width * 0.5f);
            block->setPosition(row * _BlockSize.width + _BlockSize.width * 0.5f,
                               col * _BlockSize.height + _BlockSize.height * 0.5f);
            
            addChild(block, 1);
            blocks[row][col] = block;
            
            turn++;
            
        }
    }
    
    //    CCLOG("%f ",block[0][1]->getPositionX());
}

void Grid::removeBlockAt(GridPosition pos)
{
    std::vector<Block*> addedBlocks;
    
    //    CCLOG("pos row %d   pos col %d ",pos.row,pos.col);
    
    //    CCLOG("new row %f  ",pos.row * _BlockSize.width + _BlockSize.width * 0.5f);
    
        Block* block = blocks[pos.row][pos.col];
        CCLOG("row===>%d",pos.col);
    
        if(block->type==((BlockType)(int)BlockType::APPLE))
        {
            Which_Type=1;
           CCLOG("APPLe");
        }
        else if(block->type==((BlockType)(int)BlockType::WATERMELON))
        {
            Which_Type=2;
             CCLOG("WATERMELON");
        }
        else if(block->type==((BlockType)(int)BlockType::ORANGE))
        {
            Which_Type=4;
             CCLOG("ORANGE");
        }
        else if(block->type==((BlockType)(int)BlockType::GAVAVA))
        {
            Which_Type=3;
            
             CCLOG("GAVAVA");
        }
       
    if (block)
    {
        blocks[pos.row][pos.col] = nullptr;
        block->explode();
        block->remove_animation(130, 837);

//        if(SliderBooster)
//        {
//            SliderBooster=false;
//            if(Which_Type==1)
//            {
//                Block* block = Block::createBlock(BlockType::VERTICAL_BOOSTER_APPLE, {pos.row, pos.col});
//                Vec2 newPosition = Vec2(pos.row * _BlockSize.width + _BlockSize.width * 0.5f,
//                                       pos.col * _BlockSize.height + _BlockSize.height * 0.5f);
//                block->gridPosition = {pos.row,pos.col};
//                block->setScale(0);
//                block->runAction(Sequence::create(EaseBounceOut::create(MoveTo::create(0.7f, newPosition)),ScaleTo::create(0.5, 1),NULL));
//
//                addChild(block, 1);
//                blocks[pos.row][pos.col] = block;
//                addedBlocks.push_back(block);
//            }
//            else if(Which_Type==2)
//            {
//                Block* block = Block::createBlock(BlockType::VERTICAL_BOOSTER_WATERMELON, {pos.row, pos.col});
//                Vec2 newPosition = Vec2(pos.row * _BlockSize.width + _BlockSize.width * 0.5f,
//                                        pos.col * _BlockSize.height + _BlockSize.height * 0.5f);
//                block->gridPosition = {pos.row,pos.col};
//                block->setScale(0);
//                block->runAction(Sequence::create(EaseBounceOut::create(MoveTo::create(0.7f, newPosition)),ScaleTo::create(0.5, 1),NULL));
//
//                addChild(block, 1);
//                blocks[pos.row][pos.col] = block;
//                addedBlocks.push_back(block);
//            }
//            else if(Which_Type==3)
//            {
//                Block* block = Block::createBlock(BlockType::VERTICAL_BOOSTER_GUAVA, {pos.row, pos.col});
//                Vec2 newPosition = Vec2(pos.row * _BlockSize.width + _BlockSize.width * 0.5f,
//                                        pos.col * _BlockSize.height + _BlockSize.height * 0.5f);
//                block->gridPosition = {pos.row,pos.col};
//                block->setScale(0);
//                block->runAction(Sequence::create(EaseBounceOut::create(MoveTo::create(0.7f, newPosition)),ScaleTo::create(0.5, 1),NULL));
//
//                addChild(block, 1);
//                blocks[pos.row][pos.col] = block;
//                addedBlocks.push_back(block);
//            }
//            else if(Which_Type==4)
//            {
//                Block* block = Block::createBlock(BlockType::VERTICAL_BOOSTER_ORANGE, {pos.row, pos.col});
//                Vec2 newPosition = Vec2(pos.row * _BlockSize.width + _BlockSize.width * 0.5f,
//                                        pos.col * _BlockSize.height + _BlockSize.height * 0.5f);
//                block->gridPosition = {pos.row,pos.col};
//                block->setScale(0);
//                block->runAction(Sequence::create(EaseBounceOut::create(MoveTo::create(0.7f, newPosition)),ScaleTo::create(0.5, 1),NULL));
//
//                addChild(block, 1);
//                blocks[pos.row][pos.col] = block;
//                addedBlocks.push_back(block);
//            }
//
//
//
//        }
//        else
//        {
//            Block* block = Block::createBlock((BlockType)random((int)BlockType::APPLE, (int)BlockType::ORANGE), {pos.row, pos.col});
//
//            Vec2 newPosition = Vec2(pos.row * _BlockSize.width + _BlockSize.width * 0.5f,
//                                    pos.col * _BlockSize.height + _BlockSize.height * 0.5f);
//
//            block->setScale(0);
//            block->runAction(Sequence::create(EaseBounceOut::create(MoveTo::create(0.7f, newPosition)),ScaleTo::create(0.5, 1),NULL));
//
//            addChild(block, 1);
//            blocks[pos.row][pos.col] = block;
//            addedBlocks.push_back(block);
//        }
        
        
        
       
        
        //Generate new blocks at emoty place
        
        
        //check matches on newly added blocks
        runAction(Sequence::create(
                                   DelayTime::create(0.75+0.5),
                                   CallFunc::create(CC_CALLBACK_0(Grid::resolveMatchesForBlocks, this, addedBlocks)),
                                   NULL));
        
        //        removeChild(block, true);
        
    }
}
void Grid::fillBlanks()
{
        std::vector<Block*> addedBlocks;
    
        for (int row = 0; row < MAX_ROWS; row++)
        {
            int blanks = 0;
    
            // Find blank spots for the column
    
            for (int col = 0; col < MAX_COLS; col++)
            {
                if (blocks[row][col] == nullptr)
                {
                    blanks++;
                }
                else if (blanks > 0) // Move blocks above the blank spot down
                {
                    int new_col = col - blanks;
                    blocks[row][col]->runAction(EaseBounceOut::create(MoveTo::create(0.7f,
                                                                                     Vec2(blocks[row][col]->getPositionX(),
                                                                                          new_col * _BlockSize.height + _BlockSize.height * 0.5f)
                                                                                     )));
                    blocks[row][new_col] = blocks[row][col];
                    blocks[row][col] = nullptr;
                    blocks[row][new_col]->gridPosition = {row, new_col};
    
                    addedBlocks.push_back(blocks[row][new_col]);
                }
            }
    
            // Add new blocks to fill the missing ones
            while(blanks--)
            {
                int new_col = MAX_COLS - blanks - 1;
    
                if(SliderBooster)
                {
                    SliderBooster=false;
                    Block* block;
                    if(Which_Type==1)
                    {
                        block = Block::createBlock(BlockType::VERTICAL_BOOSTER_APPLE, {row, new_col});
                    }
                    else if(Which_Type==2)
                    {
                        block = Block::createBlock(BlockType::VERTICAL_BOOSTER_WATERMELON, {row, new_col});
                    }
                    else if(Which_Type==3)
                    {
                        block = Block::createBlock(BlockType::VERTICAL_BOOSTER_GUAVA, {row, new_col});
                    }
                    else if(Which_Type==4)
                    {
                        block = Block::createBlock(BlockType::VERTICAL_BOOSTER_ORANGE, {row, new_col});
                    }
//                    Block* block = Block::createBlock(BlockType::VERTICAL_BOOSTER_APPLE, {row, new_col});
    
                    Vec2 newPosition = Vec2(row * _BlockSize.width + _BlockSize.width * 0.5f,
                                            new_col * _BlockSize.height + _BlockSize.height * 0.5f);
    
                    block->setPositionX(newPosition.x);
                    block->setPositionY(getContentSize().height + (MAX_COLS - blanks) * _BlockSize.height * 0.5f);
                    block->gridPosition = {row, new_col};
                    block->runAction(EaseBounceOut::create(MoveTo::create(0.7f, newPosition)));
    
                    addChild(block, 1);
                    blocks[row][new_col] = block;
                    addedBlocks.push_back(block);
    
    
    
                }
                else
                {
                    Block* block = Block::createBlock((BlockType)random((int)BlockType::APPLE, (int)BlockType::ORANGE), {row, new_col});
    
                    Vec2 newPosition = Vec2(row * _BlockSize.width + _BlockSize.width * 0.5f,
                                            new_col * _BlockSize.height + _BlockSize.height * 0.5f);
    
                    block->setPositionX(newPosition.x);
                    block->setPositionY(getContentSize().height + (MAX_COLS - blanks) * _BlockSize.height * 0.5f);
                    block->gridPosition = {row, new_col};
                    block->runAction(EaseBounceOut::create(MoveTo::create(0.7f, newPosition)));
    
                    addChild(block, 1);
                    blocks[row][new_col] = block;
                    addedBlocks.push_back(block);
                }
    
            }
        }
        //checkimg the new added blocks runtime if they are matched
        runAction(Sequence::create(
                                   DelayTime::create(0.75f),
                                   CallFunc::create(CC_CALLBACK_0(Grid::resolveMatchesForBlocks, this, addedBlocks)),
                                   NULL));
}
Grid* Grid::createGrid(int rows, int cols)
{
    
    Grid* grid = new (std::nothrow) Grid();
    
    if (grid && grid->init())
    {
        grid->autorelease();
        grid->setContentSize(Size(rows * _BlockSize.width, cols * _BlockSize.height));
        grid->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        return grid;
    }
    else
    {
        CC_SAFE_DELETE(grid);
    }
    
    
    return nullptr;
};

