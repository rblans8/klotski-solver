#include <iostream>
#include <string>
#include <set>

/* Klotski solver
 * 
 * I got stuck trying to solve Klotski at "Trail" 
 *  - 8th puzzle phase along the "Huarong Trail"
 *   - after "Red Donkey", but before "Ambush".
 * 
 * It looks something like this:
 *  __________
 * | _  __  _ |    I've assigned numbers to block by type:
 * || ||  || || =  1 2 2 1  block type 1 is 1x2
 * ||_||__||_|| =  1 2 2 1  block type 2 is 2x2 (special)
 * |[_][__][_]| =  3 4 4 3  block type 3 is 1x1
 * |[_][__][_]| =  3 4 4 3  block type 4 is 2x1
 * |   [__]   | =  0 4 4 0  block type 0 is 1x1 SPACE
 * +---<><>---+
 * 
 * Go install the Klotski game to see what it really looks like.
 * 
*/

struct block 
{
    int t; // type
    int w; // width 
    int h; // height
    int x;
    int y;
};

#define ARRAY_COUNT(A) (sizeof(A)/sizeof(A[0]))

block blockList[] = 
{
   // T, w, h, x, y
    { 1, 1, 2, 1, 1 }, // block 0 is type 1 - 1x2
    { 2, 2, 2, 2, 1 }, // block 1 is type 2 - 2x2 - The special block.
    { 1, 1, 2, 4, 1 }, // block 2 is type 1 - 1x2
    { 3, 1, 1, 1, 3 }, // block 3 is type 3 - 1x1
    { 4, 2, 1, 2, 3 }, // block 4 is type 4 - 2x1
    { 3, 1, 1, 4, 3 }, // block 5 is type 3 - 1x1
    { 3, 1, 1, 1, 4 }, // block 6 is type 3 - 1x1
    { 4, 2, 1, 2, 4 }, // block 7 is type 4 - 2x1
    { 3, 1, 1, 4, 4 }, // block 8 is type 3 - 1x1
    { 4, 2, 1, 2, 5 }, // block 9 is type 4 - 2x1
    { 0, 1, 1, 1, 5 }, // "block" 10 type 0 - 1x1 SPACE    
    { 0, 1, 1, 4, 5 }, // "block" 11 type 0 - 1x1 SPACE    
    
};

const int numBlocks = ARRAY_COUNT(blockList);

int klotskiMap[7][6]; 
const int mapHeight = ARRAY_COUNT(klotskiMap);
const int mapWidth = ARRAY_COUNT(klotskiMap[0]);

int abs(int x)
{
    if (x < 0) return -x;
    return x;
}

// Check to see if there is enough space for block N at location (x,y)
bool CheckForSpace(int blockN, int x, int y)
{
    bool success = true;
    int h = blockList[blockN].h;
    int w = blockList[blockN].w;

    for (int row = y; (row < y+h) && success; row++)
    {
        for (int col = x; (col < x+w) && success; col++)
        {
            if (klotskiMap[row][col] != 0) // 0 == space
            {
                success = false;
            }
        }
    }    
    
    return success;
}

// Place the block at a new location - without checking for space.
void PlaceBlock(int blockN, int x, int y)
{
    int t = blockList[blockN].t;
    int h = blockList[blockN].h;
    int w = blockList[blockN].w;

    for (int row = y; row < y+h; row++)
    {
        for (int col = x; col < x+w; col++)
        {
            klotskiMap[row][col] = t;
        }
    }        

    blockList[blockN].x = x;
    blockList[blockN].y = y;
}

// Place SPACE where a block is now.
void RemoveBlock(int blockN)
{
    int x = blockList[blockN].x;
    int y = blockList[blockN].y;
    int h = blockList[blockN].h;
    int w = blockList[blockN].w;

    for (int row = y; row < y+h; row++)
    {
        for (int col = x; col < x+w; col++)
        {
            klotskiMap[row][col] = 0;
        }
    }        

    // Not really needed. But, may help debug - if something goes wrong.
    blockList[blockN].x = -1;
    blockList[blockN].y = -1;
}


// ATTEMPT to move a block 
// Note only moves by 1 are allowed.
bool MoveBlock(int blockN, int xTo, int yTo)
{
    bool success = false;
    int x = blockList[blockN].x;
    int y = blockList[blockN].y;

    // if new location is +/- 1 away:
    if ((abs(x - xTo) <= 1) && (abs(y - yTo) <= 1))
    {
        // 1) Put SPACE where the block is now
        RemoveBlock(blockN);

        // 2) Ensure there is now space for the block at new location
        success = CheckForSpace(blockN, xTo, yTo);
    }
   
    if (success)
    {
        // Place the block at the new location
        PlaceBlock(blockN, xTo, yTo);
    }
    else
    {
        // Restore old block location
        PlaceBlock(blockN, x, y);
    }
    
    return success;
}


void SetupMap()
{
    std::cout << "Doing Initial Setup of Klotski map:" << std::endl;
    
    // Start with all locations -1 (indicating immobile wall)
    for (int row = 0; row < mapHeight; row++)
    {
        for (int col = 0; col < mapWidth; col++)
        {
            klotskiMap[row][col] = -1;
        }
    }
      
    // Fill in block (and space) locations. 
    for (int b = 0; b <= numBlocks; b++)
    {
        PlaceBlock(b, blockList[b].x, blockList[b].y);
    }
}



void ShowMap()
{
    std::cout << "Klotski map:" << std::endl;
    // start 1 and end max-1 to avoid the -1 at start and end in the map.
    for (int y = 1; y < mapHeight-1; y++)
    {
        for (int x = 1; x < mapWidth-1; x++)
        {
            std::cout << klotskiMap[y][x] << " "; 
        }   
        std::cout << std::endl;
    }
}


std::string GetMapState()
{
    std::string mapState;
    // start 1 and end max-1 to avoid the -1 at start and end in the map.
    for (int y = 1; y < mapHeight-1; y++)
    {
        for (int x = 1; x < mapWidth-1; x++)
        {
            mapState.append(1, '0' + klotskiMap[y][x]);
        }   
    }    
    //std::cout << "MapState = " << mapState << std::endl;   
    return mapState;
}

int main(int argc, char * argv[])
{
    std::set<std::string> setOfMoves;
    
    SetupMap();
    
    ShowMap();
    std::string mapState = GetMapState();
    setOfMoves.insert(mapState);  
    
    // Make a valid move
    for (int b = 0; b < numBlocks; b++)
    {
        if (blockList[b].t == 0)  // SPACEs (0) are at end of block list.
            break;
      
        int x = blockList[b].x;
        int y = blockList[b].y;
        
        if (MoveBlock(b, x+1, y) ||
            MoveBlock(b, x-1, y) ||
            MoveBlock(b, x, y+1) ||
            MoveBlock(b, x, y-1))
        {
            std::cout << "Found a valid move for block " << b << std::endl;
            break;
        }
    }

    // Show the map again
    ShowMap();
    mapState = GetMapState();
    setOfMoves.insert(mapState);  
   
   return 0;
}


