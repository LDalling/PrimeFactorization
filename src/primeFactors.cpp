#include "primeFactors.h"
#include <list>
#include <cmath>
#include <iostream>
#include <vector>
#include <boost/multiprecision/cpp_int.hpp>
using boost::multiprecision::cpp_int_backend;


std::vector<unsigned long int> G_primes;

//Optimised from last implementation 

//Greatest common divisor: Euclidean method (https://en.wikipedia.org/wiki/Greatest_common_divisor)
//Finds the modulo of value 1 and value 2, then finds the modulo of value 2 and the remainder, repeating until the final remainder is zero.
//Returns the greatest number that divides both values 1 and 2

int gcd(unsigned long int  value1, unsigned long int value2)
{
	if (value1 < 0) value1 = 0 - value1;
	if (value2 < 0) value2 = 0 - value2;
	// ensure value1 is the greater value
	if (value2 > value1) { int transfer = value1; value1 = value2; value2 = transfer; }
	if (value2 <= 1) return 1;
	long int lastremainder = value2;
	long int remainder = value1%value2;
	while (remainder != 0) 
	{
		lastremainder = remainder;
		remainder = value1%value2;
		value1 = value2;
		value2 = remainder;

	}
	
	return lastremainder;
}




// Modular exponentiation, used to calculate modulo of massive numbers (more than 2^64)
// Works based on the fact that (A%X * B%X)%X == (A*B)%X
// Additionally utilises the fact X^N == (X*X)^N/2, dividing the power in half when it's an even number
// since x*x*x*x*x*x*x*x is much slower to calculate compared to finding (x*x) then running (x*x)*(x*x)*(x*x)*(x*x)
unsigned long int powerMod(unsigned long base, unsigned long power, unsigned long modulo)
{

	//values need to be very large as these exponents can reach some insane values and overflow
	//(technically 1024 bit is overkill but hasn't affected times much)
	boost::multiprecision::uint1024_t value = 1; 
	boost::multiprecision::uint1024_t baseLong = base;	
	int reps = 0;
	while (power > 0)
	{	
		reps += 1;
		if (power%2 == 1) //if the exponent is odd
		{
			value = (value * baseLong)%modulo; 		
			power -=1;	
		}
		power = power/2; // half the power
		baseLong = (baseLong*baseLong)%modulo;//square the base value
		
	}
	base = (long unsigned) value; 	// since the final output is guaranteed to be below the value of the modulo (which is unsigned long)
					// we can safely map it back to an unsigned long variable
	return base;
}


unsigned long int powerInt(unsigned long base, unsigned long power)
{
	boost::multiprecision::uint1024_t value = 1;
	boost::multiprecision::uint1024_t baseLong = base;	

	while (power > 0)
	{	
		if (power%2 == 1) //if the exponent is odd
		{
			value = (value * baseLong); 		
			power -=1;	
		}
		power = power/2; 		// similarly to the powerMod function, half the power
		baseLong = (baseLong*baseLong);	// square the value (X^N == (X*X)^N/2)
		
	}
	base = (long unsigned) value;
	return base;
}


// I did like living under the illusion single-base tests would be fine, but I guess being truly deterministic is a lot more useful than 'generally sort-of correct'.
// Though it's really hard to test as there isn't much documentation on easily accessible pseudoprimes!)
// As documented on https://miller-rabin.appspot.com, these 7 bases are accurate for any number up to 'at least 2^64'
int millerBases[7] = {2,325,9375,28178,450775,9780504,1795265022};





// Probable Prime using deterministic Miller-Rabin's primality test https://en.wikipedia.org/wiki/Miller-Rabin_primality_test
// Bases from miller-rabin.appspot.com, these 7 bases are deterministic up to 2^64.
// The Miller-Rabin test takes n, the value to be tested, then subtracts 1 from it (m)
// After that, it represents m in the form (2^s * d) with odd d by setting d to be equal to m, then repeatedly dividing d by 2
// For example, 156 is represented by  2^3 * 39
// then running this by 'fermat's little theorem', which states if ( base^d%n == 1) or (base^((2^s)*r))%n == m) for each value of r between 0~(s-1) then the number is probably prime
//
// However, it's only 'probably' prime, so now we repeat for the 7 different bases that are proven to provide deterministic results for any number between 2^64

bool millerRabin(unsigned long int n)
{
	int iterator = 0;
	bool correct = true;

	//std::cout << "Rabin on "<<n<<"\n";
	unsigned long m = n-1;  //We need N-1 for the calculations later
	unsigned long s = 0;	
	unsigned long d = n-1;

	while (d %2 == 0) // While our factor value is even (we need to find the odd value)
	{
		s++; // Increment the power
		d = d/2;
	}

	
	// This part loops over the 7 different bases to which a Miller-Rabin test is perfectly accurate

	while (iterator < 7 && correct)
	{
		int base = millerBases[iterator]%n;
		if (base == 0) return true;
		iterator++;

		if (powerMod(base,d,n) == 1) 
		{

			continue; // Checking to see if (base^factor%n) == 1, if true then we can skip the next part...
		}
		bool success = false; //used to detect if any of the values from 0~(s-1) are valid
		for (unsigned int r = 0; r < s; r++)
		{
			if (powerMod(base, powerInt(2,r)*d, n) == m)
			{
				success = true; //this flag is because I can't break out of two loops without some flag variable >:(
				break;		//(as breaking without the 'success' flag causes the code to think it's a non prime...)
			}	
		}
		if (success) continue;
		correct = false;
	}
	//std::cout << "Rabin over "<<n<<"\n";
	return correct;
}


unsigned int G_smallPrimes[10] = {2,3,5,7,11,13,17,19,23,29};




void appendPrime(unsigned long int factor)
{
	if (factor == 1) return;
	if (G_primes.size() == 0)
	{
		G_primes.push_back(factor);
	}
	else
	{
		unsigned int i = 0;
		while (i<G_primes.size() && factor > G_primes[i])
		{
			i += 1;
		}
		G_primes.insert(G_primes.begin() + i, factor);
	}

}


//Pollard's Rho Algorithm 
//Pollard's Rho uses a pseudo-random sequence generated with a polynomial modulo g(x) in order to find factors.
//Initially, it takes 3 different values to apply into a polynomial function  (polymod = (polymod*polymod+1)%toCalc) until the value (polymod-polyconst) has a common factor with toCalc.
//(this might be a bit vague for some compsci students but really it's not something that can be easily described)
unsigned long int G_maxCycles = 4096;
unsigned long int PollardRho(unsigned long int toCalc)
{

	unsigned long  int polymod = 2;
	unsigned long int factor = 1;
	unsigned long int  polyconst = 2;
	//std::cout << "Pollard on "<<toCalc<<"\n";

	int cycles = 2;
	while (factor == 1 )
	{
			for (int i = 1; i <= cycles && factor <= 1; i++)
			{
				polymod = (polymod*polymod + 1) % toCalc; // here the polynomial function is applied, aka x = (x*x+1)%n
				factor = gcd(polymod - polyconst, toCalc);// get greatest common divisor between this polynomial and the number we're calculating
			}
			cycles *= 2; //widen the range of the next search
			polyconst = polymod; 
	}

	//std::cout << "Pollard over "<<toCalc<<"\n";
	return factor;
}


bool isPrime(unsigned long int toCheck)
{
	if (toCheck%2 == 0 && toCheck > 2) return false; //Even numbers are all factors of 2
	if (toCheck <= 3) return true; // 1, 2 and 3 tend to mess with some of the algorithms
	//std::cout << "Trial on "<<toCheck<<"\n";

	// Simple trial division for small primes
	for (int i=0; i<10; i++)
	{
		if (toCheck%G_smallPrimes[i] == 0 && toCheck > G_smallPrimes[i]) return false; // if it's divisible by any of the small primes then it's obviously not prime
		else if (toCheck == G_smallPrimes[i]) return true; //if it is the exact same as a prime, it's probably a prime :^) 
	}
	//std::cout << "Trial over "<<toCheck<<"\n";
	// Miller Rabin test 	
	return millerRabin(toCheck);

}


// 
void factorFully(unsigned long int toFactor)
{

	unsigned long int newFactor = toFactor;
	
	//std::cout << "Starting on "<<toFactor<<"\n";
	//Now the preliminary Pollard operations have been completed, 
	while (!isPrime(toFactor))
	{
            bool smallPrime = false;
            for (int i=0; i<10; i++)
            {
                if (toFactor%G_smallPrimes[i] == 0)
                {
                    newFactor = G_smallPrimes[i];
                    smallPrime = true;
                }
            }
            if (!smallPrime)
            {
			    newFactor = PollardRho(toFactor);
            }
			toFactor /= newFactor;
			factorFully(newFactor);

	}
	appendPrime(toFactor);
}


std::list<unsigned long int> primeFactors(unsigned long int toCalc)
{
	G_primes.clear();
	if (toCalc > 3) //2 & 3 are primes so can just be thrown into the output
	{
	factorFully(toCalc);
	}
	else if (toCalc > 1) //no point putting factors of value 1 into the vector...
	{
		G_primes.push_back(toCalc);
	}

	std::list<unsigned long int> primeslist;
	for (unsigned int i = 0; i < G_primes.size(); i++)
	{
		primeslist.push_back(G_primes[i]);
	}
	return primeslist;
}

