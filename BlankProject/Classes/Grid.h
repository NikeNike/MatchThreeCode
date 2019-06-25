//
//  Grid.h
//  MatchThree
//
//  Created by Dimitar Dimitrov on 5/2/16.
//
//

#ifndef MATCH_THREE_GRID_H
#define MATCH_THREE_GRID_H

#include <cocos2d.h>
#include "Block.h"

class Grid: public cocos2d::Layer
{
public:
    
    static Grid* createGrid(int rows, int cols);
    Block* getBlockForPosition(cocos2d::Vec2 position); // Create Grid With Rows and Cols
    Block* blocks[MAX_ROWS][MAX_COLS];
    
    
    Block* store_block[100];


    
    void rest_store_value();
    
    
    
    
    void generateRandomBlocks();
    bool checkForMatches(Block* block);
    
    void fillBlanks();
    void removeBlockAt(GridPosition pos);
    bool areNeighbours(Block* first, Block* second);
    void swapBlocks(Block* first, Block* second);
    void resolveMatchesForBlocks(const std::vector<Block*>& blocks);
    int findMatches(Block* block, std::vector<Block*>& matches, bool isRoot = true);
};

#endif /* _MATCH_THREE_GRID_H_ */
