#ifndef DOTSGENERATOR
#define DOTSGENERATOR

#include <random>
#include <vector>

class DotsGenerator
{
public:
    DotsGenerator(int _xRange, int _yRange, int _n);
    void getDots(std::vector< std::pair< int, int > >& verticies);

private:
    int xRange = 0;
    int yRange = 0;
    int n = 0;
};

#endif