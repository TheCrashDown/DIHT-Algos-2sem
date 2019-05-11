#include "../headers/DotsGenerator.h"

#include <random>
#include <vector>

DotsGenerator::DotsGenerator(int _xRange, int _yRange, int _n)
{
    xRange = _xRange;
    yRange = _yRange;
    n = _n;
}

void DotsGenerator::getDots(std::vector< std::pair< int, int > >& verticies)
{
    //  генерируем необходимое число точек и записываем в вектор их координаты
    static std::random_device randomDevice;
    static std::mt19937 generator(randomDevice());

    static std::normal_distribution< float > rndX(0.5 * xRange, 2);
    static std::normal_distribution< float > rndY(0.5 * yRange, 2);

    for(int i = 0; i < n; ++i){
        int tmpX = 0;
        int tmpY = 0;

        while(tmpX <= 0 || tmpX >= xRange){
            tmpX = rndX(generator);
        }

        while(tmpY <= 0 || tmpY >= yRange){
            tmpY = rndY(generator);
        }

        verticies.push_back(std::make_pair(tmpX, tmpY));
    }
}
