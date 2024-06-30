#include "intensity_segment.h"
#include <iostream>
#include <cassert>
int main(int argc, const char* argv[]) {

    IntensitySegments segments;
    assert(segments.toString() == "[]"); 
    segments.add(10, 30, 1);
    assert(segments.toString() == "[[10,1],[30,0]]"); 
    segments.add(20, 40, 1);
    assert(segments.toString() == "[[10,1],[20,2],[30,1],[40,0]]"); 
    segments.add(10, 40, -2);
    assert(segments.toString() == "[[10,-1],[20,0],[30,-1],[40,0]]"); 

    segments.set(10, 40, 0);
    assert(segments.toString() == "[]"); 
    segments.add(10, 30, 1);
    assert(segments.toString() == "[[10,1],[30,0]]");
    segments.add(20, 40, 1);
    assert(segments.toString() ==  "[[10,1],[20,2],[30,1],[40,0]]");
    segments.add(10, 40, -1);
    assert(segments.toString() ==  "[[20,1],[30,0]]");
    segments.add(10, 40, -1);
    assert(segments.toString() ==  "[[10,-1],[20,0],[30,-1],[40,0]]");

    segments.set(10, 40, 0);
    segments.add(10, 20, 1);
    assert(segments.toString() == "[[10,1],[20,0]]");
    
    segments.add(5, 8, 1);
    assert(segments.toString() ==  "[[5,1],[8,0],[10,1],[20,0]]");

    segments.add(30, 40, -1);
    assert(segments.toString() ==  "[[5,1],[8,0],[10,1],[20,0],[30,-1],[40,0]]");

    segments.add(0, 5, 1);
    assert(segments.toString() ==  "[[0,1],[8,0],[10,1],[20,0],[30,-1],[40,0]]");

    segments.add(-1, 11, 1);
    assert(segments.toString() ==  "[[-1,1],[0,2],[8,1],[10,2],[11,1],[20,0],[30,-1],[40,0]]");

    segments.set(-1, 40, 0);
    segments.add(10, 20, 1);
    assert(segments.toString() == "[[10,1],[20,0]]");
    
    segments.add(10, 20, 1);
    assert(segments.toString() ==  "[[10,2],[20,0]]");

    segments.add(20, 30, -1);
    assert(segments.toString() ==  "[[10,2],[20,-1],[30,0]]");

    segments.add(11, 15, 1);
    assert(segments.toString() ==  "[[10,2],[11,3],[15,2],[20,-1],[30,0]]");

    segments.set(0, 5, 0);
    assert(segments.toString() ==  "[[10,2],[11,3],[15,2],[20,-1],[30,0]]");

    segments.set(30, 10, 0);
    assert(segments.toString() ==  "[[10,2],[11,3],[15,2],[20,-1],[30,0]]");

    segments.set(5, 15, 0);
    assert(segments.toString() ==  "[[15,2],[20,-1],[30,0]]");

    segments.set(20, 30, 0);
    assert(segments.toString() ==  "[[15,2],[20,0]]");

    segments.set(8, 18, 5);
    assert(segments.toString() ==  "[[8,5],[18,2],[20,0]]");

    return 0;
}