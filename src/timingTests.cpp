#include <iostream>
#include <chrono>
#include <vector>
#include <list>
#include <numeric>
#include <functional>

// #define NDEBUG
#include <cassert>

#include "primeFactors.h"

using std::cout;
using std::endl;

using std::chrono::steady_clock;
using std::chrono::milliseconds;
using std::chrono::duration_cast;


void testPrimeFactors(unsigned long int n)
{
    steady_clock::time_point startTime = steady_clock::now();

    std::list<unsigned long int> factors = primeFactors(n);

    steady_clock::time_point finishTime = steady_clock::now();

    milliseconds timeTaken = duration_cast<milliseconds>(finishTime - startTime);

    assert( n == std::accumulate(factors.begin(), factors.end(), 1ul, std::multiplies<unsigned long int>()) );

    cout << "Prime factors of " << n << " are: ";
    for (unsigned long int f : factors)
    {
        cout << f << ' ';
    }
    cout << endl;
    cout << "Time taken: " << timeTaken.count() << "ms" << endl;
    cout << endl;
}

int main()
{
    std::vector<unsigned long int> testData{
      12ul,
      392846ul,
      246819835539726757ul,
      92746287377ul,
      916284625384739736ul,
      872640841ul,
      3853279973726ul,
      9731347823637ul,
      18264978256367123689ul,
      8934894742462477081ul,
      7349634624267476927ul,
      935724987531993881ul,
      348051774975651917ul,
      6588122883467696985ul,
      18446744073709551557ul
    };

    for (unsigned long int n : testData)
    {
        testPrimeFactors(n);
    }

    return 0;
}
