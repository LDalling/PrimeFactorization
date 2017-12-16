#include <list>
#include <cmath>
#include <iostream> 	
#include "primeFactors.h"
#include <string>
int main()
{
	unsigned long n;
    while (n > 0){
    std::cout<<"Insert prime number (Less than 18446744073709551615, let's not go beyond 64-bits...):\n";
    std::cin >> n;
    std::list<unsigned long int> factors = primeFactors(n);
    std::cout << "Prime factors of " << n << " are: ";
    for (unsigned long int f : factors)
    {
        std::cout << f << ' ';
    }
    std::cout<<"\n";
    }
    return 0;
}
