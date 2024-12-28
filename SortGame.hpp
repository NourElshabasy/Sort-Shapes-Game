#ifndef SortGame_H
#define SortGame_H

#include <vector>
#include <string>
class SortGame {
public:
    SortGame(); //2 bins, capacity 4 (each bin has at most 4 elements), 1 empty bin 
    SortGame(int numDiffElms, int capacity, int emptyBins);
    SortGame(std::vector<std::vector<int>> binData, int emptyBins);

    void randomize();
    void display() const;

    bool move(std::vector<std::vector<int>>& binBalls, int& numBinsFinished);
    
    void play();
    
private:
    std::vector<std::vector<int>> bins;
    int numDiffElms;
    int numEmptyBins;
    int capacity;
};
#endif
