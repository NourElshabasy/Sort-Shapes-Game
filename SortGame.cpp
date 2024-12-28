#include <iostream>
#include <vector>
#include <string>
#include "SortGame.hpp"
using namespace std;

SortGame::SortGame()
{
    numDiffElms = 2;
    capacity = 4;
    numEmptyBins = 1;

    for (int i = 1; i <= numDiffElms; i++)
    {
        vector<int> row;

        for (int j = 0; j < capacity; j++)
        {
            row.push_back(i);
        }
        bins.push_back(row);
    }

    for (int i = 1; i <= numEmptyBins; i++)
    {
        vector<int> emptyRow;
        bins.push_back(emptyRow);
    }

    randomize();
}

SortGame::SortGame(int numDiffElms, int capacity, int emptyBins)
{
    if (numDiffElms < 1)
        this->numDiffElms = 1;
    else
        this->numDiffElms = numDiffElms;
    if (capacity < 2)
        this->capacity = 2;
    else
        this->capacity = capacity;
    if (emptyBins < 1)
        numEmptyBins = 1;
    else
        numEmptyBins = emptyBins;

    for (int i = 1; i <= this->numDiffElms; i++)
    {
        vector<int> row;

        for (int j = 1; j <= capacity; j++)
        {
            row.push_back(i);
        }
        bins.push_back(row);
    }

    for (int i = 1; i <= numEmptyBins; i++)
    {
        vector<int> emptyRow;
        bins.push_back(emptyRow);
    }

    randomize();
}

SortGame::SortGame(std::vector<std::vector<int>> binData, int emptyBins)
{
    numDiffElms = binData.size();
    capacity = binData[0].size();
    numEmptyBins = emptyBins;

    for (const auto &row : binData)
    {
        bins.push_back(row);
    }

    for (int i = 0; i < numEmptyBins; i++)
    {
        vector<int> emptyRow;
        bins.push_back(emptyRow);
    }
}

void SortGame::randomize()
{
    srand(1);
    // srand(11);
    int currLastIdx = numDiffElms * capacity - 1;

    while (currLastIdx > 0)
    {
        int k = rand() % (currLastIdx + 1);
        int temp = bins[k / capacity][k % capacity];
        bins[k / capacity][k % capacity] = bins[currLastIdx / capacity][currLastIdx % capacity];
        bins[currLastIdx / capacity][currLastIdx % capacity] = temp;
        currLastIdx--;
    }
}

void SortGame::display() const
{
    string shapes[] = {
        "\u2b24", // circle
        "\u2b1f", // pentagon
        "\u25fc", // medium square
        "\u272f", // star
        "\u2665", // heart
        "\u25b2", // triangle
    };

    int shapes_size = sizeof(shapes) / sizeof(shapes[0]);
    // colorMap[0] is "\033[31m", a const char, red text color
    // colorMap[1] is "\033[32m", a const char, green text color
    vector<string> colorShapeMap(numDiffElms);
    int colorCode = 31;
    for (int i = 0; i < numDiffElms; i++)
    {
        colorShapeMap[i] = "\033[" + to_string(colorCode) + "m" +
                           shapes[i % shapes_size] + "\033[0m";
        //\033[0m is a black color
        colorCode++;
    }

    for (int i = capacity - 1; i > -1; i--)
    {
        for (int j = 0; j < bins.size(); j++)
        {
            if (i < bins[j].size())
            {
                cout << "|  " << colorShapeMap[bins[j][i] - 1] << " |" << "   ";
                // cout << "|  " << bins[j][i] << " |" << "   ";
            }
            else
            {
                cout << "|    |   ";
            }
        }
        cout << endl;
    }
    for (int i = 0; i < numDiffElms + numEmptyBins; i++)
    {
        cout << "+----+   ";
    }
    cout << endl;
    for (int i = 0; i < numDiffElms + numEmptyBins; i++)
    {
        cout << "Bin  " << i + 1 << "   ";
    }
}

bool SortGame::move(std::vector<std::vector<int>> &shapesInBins, int &numFinishedBins)
{
    int in, out;
    cout << "Enter move out and move in bins (-1 -1 to stop): ";
    cin >> out >> in;

    if (out == -1 && in == -1)
        return false;
    else if (out < 1 || out > bins.size() || in < 1 || in > bins.size() || out == in)
    {
        cout << "wrong bin number" << endl;
        return true;
    }
    else if (bins[out - 1].size() == 0)
    {
        cout << "move out bin is empty" << endl;
        return true;
    }

    int topElement = bins[out - 1].back();

    if (bins[in - 1].size() != 0 && topElement != bins[in - 1].back())
    {
        cout << "not match" << endl;
        return true;
    }

    int numConsecutiveElemets = 1;
    for (int i = bins[out - 1].size() - 2; i >= 0; i--)
    {
        if (bins[out - 1][i] == bins[out - 1][i + 1])
            numConsecutiveElemets++;
        else
            break;
    }

    if (bins[in - 1].size() + numConsecutiveElemets > capacity)
    {
        cout << "The move in bin has no sufficient slots." << endl;
        return true;
    }

    for (int i = 0; i < numConsecutiveElemets; i++)
    {
        bins[in - 1].push_back(bins[out - 1].back());
        bins[out - 1].pop_back();
    }

    // Update shapesInBins
    shapesInBins[out - 1][topElement - 1] -= numConsecutiveElemets;
    shapesInBins[in - 1][topElement - 1] += numConsecutiveElemets;

    if (bins[in - 1].size() == capacity)
    {
        bool isFinished = true;
        for (int i = 1; i < bins[in - 1].size(); i++)
        {
            if (bins[in - 1][i] != bins[in - 1][0])
            {
                isFinished = false;
                break;
            }
        }
        if (isFinished)
        {
            numFinishedBins++;
        }
    }

    display();

    return true;
}

void SortGame::play()
{
    vector<vector<int>> shapesInTheBins(bins.size(), vector<int>(numDiffElms, 0));
    for (int i = 0; i < bins.size(); i++)
    {
        for (int j = 0; j < bins[i].size(); j++)
        {
            shapesInTheBins[i][bins[i][j] - 1]++;
        }
    }
    int numOfFinishedBins = 0;
    display();
    bool bContinue = true;
    int numMoves = 0;

    while (bContinue && numOfFinishedBins < numDiffElms)
    {
        numMoves++;
        cout << "\nMove " << numMoves << endl;
        bContinue = move(shapesInTheBins, numOfFinishedBins);
    }
    cout << "\nCongratulations! Finish the game in " << numMoves << " moves." << endl;
}
