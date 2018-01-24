//#define RANDOM_TEST_ON
//#define JSON_TEST_ON

#if defined(RANDOM_TEST_ON) || defined(JSON_TEST_ON)
#define TEST_ON
#endif

#ifdef RANDOM_TEST_ON
#include <iostream>
#include <string>
#include "fast-rand.h"

int main(int argc, char * argv[])
{
    for (int i = 0; i != 10; ++i)
    {
        std::cout << "Seed: " << g_seed << ",\t";
        std::cout << fastRand() << std::endl;
    }

    //srand(static_cast<unsigned int>(time(NULL)));
    //auto size = std::stoi(argv[1]);
    //randomTest(size);

    return 0;
}
#endif

#ifdef JSON_TEST_ON
#include <iostream>
#include "json.h"

int main(int argc, char * argv[])
{
    JSON json("{\n\t\"Size\": 100,\n\t\"Iterations\": 1e6\n}");
    std::cout << "Size       = " << json.getNumberValue("Size") << std::endl;
    std::cout << "Iterations = " << json.getNumberValue("Iterations") << std::endl;
}
#endif

#ifndef TEST_ON
#include <array>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <omp.h>
#include "ising-2d.h"
#include "win-timing.h"

#define $ENSEMBLE_NUMBER    120
#define $ENSEMBLE_DELTA     4
#define $REPEAT_NUMBER      100

using namespace std;

Quantity eval(const size_t & size, const size_t & steps, const double & temperature)
{
    Ising2D s{ size,size };
    // Set magnetic_b = 0.
    return s.evaluate(1.0 / temperature, 0.0, steps, $ENSEMBLE_NUMBER, $ENSEMBLE_DELTA);
}

struct EvalCell
{
    double temperature;
    array<Quantity, $REPEAT_NUMBER> quantity;

    friend ostream & operator<<(ostream & os, const EvalCell & cell)
    {
        os << cell.temperature;
        for (auto i : cell.quantity)
            os << "," << i.magnetic_dipole << "," << i.energy;
        return os;
    }
};

int main(int argc, char * argv[])
{
    auto size = stoi(argv[1]);
    auto steps = stoi(argv[2]);
    auto t_begin = stod(argv[3]);
    auto t_end = stod(argv[4]);
    auto t_num = stoi(argv[5]);

    vector<EvalCell> result_arr(t_num + 1);
    for (auto i = 0; i < t_num + 1; ++i)
        result_arr[i].temperature = static_cast<double>(i) / static_cast<double>(t_num)
            * (t_end - t_begin) + t_begin;

    cerr << "0% ";

    Timing clock;

    clock.timingStart();
#pragma omp parallel for
    for (auto i = 0; i < t_num + 1; ++i)
    {
        for(auto & j : result_arr[i].quantity)
            j = eval(size, steps, result_arr[i].temperature);
        if (t_num < 10) { cerr << "=="; }
        else
            if ((i + 1) % (t_num / 10) == 0)
                cerr << "==";
    }
    clock.timingEnd();

    cerr << "> 100%  Finished!" << endl
         << "Computation time: " << clock.getRunTime() << "s." << endl;

    for (auto i = result_arr.begin(); i != result_arr.end(); ++i)
        cout << *i << endl;

    return 0;
}

#endif
