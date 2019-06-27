//
//  Global.h
//  MatchThree
//
//  Created by Dimitar Dimitrov on 5/2/16.
//
//

#ifndef __MATCH_THREE_APP_GLOBAL_H_
#define __MATCH_THREE_APP_GLOBAL_H_

#include <cocos2d.h>

static bool isVertical=false;
static bool SliderBooster=false;
static int Box_check_odd=1;
const cocos2d::Size _BlockSize = cocos2d::Size(60,60);
const char* const EVENT_GRID_READY = "event.custom.grid_is_ready";
const char* const EVENT_GRID_HAVE_MATCHES = "event.custom.grid_have_matches";
// Size of the grid
const int MAX_ROWS = 5;
const int MAX_COLS = 5;
const int SWIPE_TRESHOLD = 5; // Minimum pixels for swipe move

const int MAX_MATCHES = 3;
struct EventMatchesData {
    /** the count of the matches */
    int matches;
};
enum class BlockType : int
{
    APPLE = 0,
    WATERMELON,
    GAVAVA,
    ORANGE,
    VERTICAL_BOOSTER,
    HORIZONTAL_BOOSTER

};
const char* const BlockTypeToFrameName[]
{
    "fruit-1.png",
    "fruit-2.png",
    "fruit-3.png",
    "fruit-4.png",
    "fruit-1-effect-2.png",
    "fruit-1-effect-1.png"
};
    
    


struct GridPosition {
    /** Row index in the grid */
    int row;
    /** Column index in the grid */
    int col;
};


#endif /* __MATCH_THREE_APP_GLOBAL_H_ */
