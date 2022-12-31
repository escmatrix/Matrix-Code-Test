#include "micromouseserver.h"
#include <stack>
using namespace std;

// location
array<int, 2> loc;

// direction - 0: up, 1: right, 2: down, 3: left
int dir;

// moves todo
stack<int> moves;

// location togo
list<array<int, 2>> togo;

// location -> walls and if visited
map<array<int, 2>, array<int, 5>> mazeMap;

// starting iteration
bool start = true;

// reset maze
void reset(){
    loc = {0,0};
    dir = 0;
    moves = {};
    togo = {};
    mazeMap = {};
}

// location, direction -> reulting location
array<int, 2> resLoc(array<int, 2> currLoc, int newDir){
    if(newDir == 0) currLoc[1]++;
    else if(newDir == 1) currLoc[0]++;
    else if(newDir == 2) currLoc[1]--;
    else if(newDir == 3) currLoc[0]--;
    return currLoc;
}

// walls -> update maze walls, add places togo
void updateWalls( array<int, 2> currLoc, int currDir, vector<bool> walls){

    // iterate through locations and walls
    for(int i = 0; i < 4; i++){
        int wall = (i - currDir + 4)%4;
        array<int, 2> newLoc = resLoc(currLoc, i);

        // wall does not exist
        if(!walls[wall]){

            // add to togo
            if(wall != 2 && (!mazeMap.count(newLoc) || mazeMap[newLoc][4] != 1))
                togo.push_front(newLoc);

            // create walls
            if(!mazeMap.count(currLoc)) mazeMap[currLoc] = {0};
            if(!mazeMap.count(newLoc)) mazeMap[newLoc] = {0};

            // set walls
            mazeMap[currLoc][i] = 2;
            mazeMap[newLoc][(i+2)%4] = 2;
        }

        // wall does exist
        else{

            // create walls
            if(!mazeMap.count(currLoc)) mazeMap[currLoc] = {0};
            if(!mazeMap.count(newLoc)) mazeMap[newLoc] = {0};

            // set walls
            mazeMap[currLoc][i] = 1;
            mazeMap[newLoc][(i+2)%4] = 1;
        }
    }
}

// moves to reach shortest location togo
stack<int> movesTogo(array<int, 2> currLoc){

    // shortest path
    stack<int> shortest;

    // iterate through locations togo
    for(array<int, 2> togoLoc: togo){

        // location -> move to get there
        map<array<int, 2>, int> moveToLoc;

        // list to check
        list<array<int, 2>> check;

        // set current location
        check.push_back(currLoc);
        moveToLoc[currLoc] = -1;

        // iterate through list to check
        while(!check.empty()){
            array<int, 2> checkLoc = check.front();
            check.pop_front();

            // location found
            if (checkLoc == togoLoc){

                // last location while backtracking
                array<int, 2> last = togoLoc;

                // shortest path test
                stack<int> shortestTest;

                // backtrack to start
                while (last != currLoc){
                    shortestTest.push(moveToLoc[last]);
                    last = resLoc(last, (moveToLoc[last]+2)%4);
                }

                // find shortest path
                if(shortestTest.size() < shortest.size() || shortest.empty()){
                    shortest = shortestTest;
                }

                break;
            }

            // location is in map
            if(mazeMap.count(checkLoc)){

                // iterate through wall directions
                for(int i = 0; i < 4; i++){

                    // wall exists
                    if(mazeMap[checkLoc][i] == 2){

                        // adjacent location based on direction
                        array<int, 2> adjLoc = resLoc(checkLoc,i);

                        // location does not already exist
                        if(!moveToLoc.count(adjLoc)){

                            // add location
                            moveToLoc[adjLoc] = i;
                            check.push_back(adjLoc);
                        }
                    }
                }
            }
        }
    }

    return shortest;
}

// direction, new direction -> move todo
int moveTodo(int currDir, int newDir){
    if(abs(newDir - currDir) == 2) return 2;
    else if((newDir - currDir + 4) % 4 == 3) return 3;
    else if((newDir - currDir + 4) % 4 == 1) return 1;
    return 0;
}

// if end found
bool foundEnd(array<int, 2> currLoc){

    // iterate through bottom left location of possible solutions
    for(array<int, 2> blLoc: {currLoc, resLoc(currLoc, 2), resLoc(currLoc, 3), resLoc(resLoc(currLoc, 2), 3)}){

        // 4 locations in a square to check
        vector<array<int, 2>> checkLocs = {blLoc, resLoc(blLoc, 0), resLoc(blLoc, 1), resLoc(resLoc(blLoc, 0), 1)};

        // all location exist
        bool locsExist = true;

        // iterate through locations
        for(array<int, 2> check: checkLocs){

            // not all locations exists
            if(!mazeMap.count(check)){
                locsExist = false;
                break;
            }
        }

        // all locations exist
        if(locsExist){

            // walls inside the square
            list<array<int, 2>> insideWalls = {{0,0}, {0,1}, {1,1}, {1,2}, {2,3}, {2,0}, {3,2}, {3,3}};

            // all walls do not exist
            bool inside = true;

            // iterate through inside walls
            for(array<int, 2> wall: insideWalls){

                // if wall exists
                if(mazeMap[checkLocs[wall[0]]][wall[1]] != 2){
                    inside = false;
                    break;
                }
            }

            // all walls do not exist
            if(inside){

                // walls outside the square
                list<array<int, 2>> outsideWalls = {{0,2}, {0,3}, {1,3}, {1,0}, {2,1}, {2,2}, {3,0}, {3,1}};

                // total missing walls
                int outside = 0;

                // iterate through outside walls
                for(array<int, 2> outsideWall: outsideWalls){
                    int wall = mazeMap[checkLocs[outsideWall[0]]][outsideWall[1]];

                    // wall is unknown or multiple missing
                    if(wall == 0 || outside > 1){
                        outside = 0;
                        break;
                    }

                    // wall does not exist
                    if(wall == 2){
                        outside += 1;
                    }
                }

                // exactly one wall is missing
                if(outside == 1){

                    // end found
                    return true;
                }
            }
        }
    }

    // end not found
    return false;
}

void microMouseServer::studentAI(){

    // starting iteration
    if(start) {
        printUI("Starting...");
        reset();
    }

    // update location
    updateWalls(loc, dir, {isWallForward(), isWallRight(), start, isWallLeft()});
    togo.remove(loc);
    mazeMap[loc][4] = 1;

    // update moves
    if(moves.empty()){
        moves = movesTogo(loc);
    }

    // if end found
    if (foundEnd(loc)){
        start = true;
        printUI("End found.");
        foundFinish();
        return;
    }

    // if end not found
    if(togo.empty() && moves.empty()){
        start = true;
        printUI("No end found.");
        foundFinish();
        return;
    }

    // do next move
    if(!moves.empty()){

        // find new location
        int newDir = moves.top();
        int move = moveTodo(dir, newDir);
        moves.pop();

        // turn to new direction
        if(move == 2) {turnRight();turnRight();}
        else if(move == 1) turnRight();
        else if(move == 3) turnLeft();

        // update location
        moveForward();
        dir = newDir;
        loc = resLoc(loc, newDir);
    }

    // end starting iteration
    if(start) start = false;
}
