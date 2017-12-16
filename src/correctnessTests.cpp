#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE PrimeFactorsCorrectnessTests
#include <boost/test/unit_test.hpp>

#include <list>
#include <numeric>
#include <iostream>

#include "primeFactors.h"


/* It is bad practice to add things to the standard namespace,
 * but it is the only easy work-around here.
 * TODO: find a better solution.
 */
namespace std
{
    // Define stream insertion to allow use of BOOST_CHECK_EQUAL().
    std::ostream & operator<<(std::ostream & os, const std::list<unsigned long int> l)
    {
        os << '{';
        for (auto it = l.begin(); it != l.end(); ++it)
        {
            if (it != l.begin()) os << ',';
            os << *it;
        }
        os << '}';
        return os;
    }
}

BOOST_AUTO_TEST_CASE( FactorsOfZero )
{
    const std::list<unsigned long int> factors {};

    BOOST_CHECK_EQUAL( primeFactors(0) , factors );
}

BOOST_AUTO_TEST_CASE( FactorsOfOne )
{
    const std::list<unsigned long int> factors {};

    BOOST_CHECK_EQUAL( primeFactors(1) , factors );
}

BOOST_AUTO_TEST_CASE( FactorsOfTwo )
{
    const std::list<unsigned long int> factors {2};

    BOOST_CHECK_EQUAL( primeFactors(2) , factors );
}

BOOST_AUTO_TEST_CASE( FactorsOfSmallInt )
{
    const unsigned long int n {15};
    const std::list<unsigned long int> factors {3,5};

    BOOST_CHECK_EQUAL( primeFactors(n) , factors );
}

BOOST_AUTO_TEST_CASE( FactorsOfMediumInt )
{
    const unsigned long int n {3825476ul};
    const std::list<unsigned long int> factors {2,2,17,101,557};

    BOOST_REQUIRE_EQUAL( n , std::accumulate(factors.begin(), factors.end(), 1ul, std::multiplies<unsigned long int>()) );
    BOOST_CHECK_EQUAL( primeFactors(n) , factors );
}

BOOST_AUTO_TEST_CASE( FactorsOfLargeInt )
{
    const unsigned long int n {246819835539726757ul};
    const std::list<unsigned long int> factors {7,11,37,131,557,18379,64601};

    // Test only valid for 8-byte (64-bit) integers
    if( sizeof(unsigned long int) >= 8 )
    {
        BOOST_REQUIRE_EQUAL( n , std::accumulate(factors.begin(), factors.end(), 1ul, std::multiplies<unsigned long int>()) );
        BOOST_CHECK_EQUAL( primeFactors(n) , factors );
    }
}

BOOST_AUTO_TEST_CASE( FactorsOfMaxInt )
// This is the std::numeric_limits<unsigned long int>::max() value for a 64-bit unsigned integer.
{
    const unsigned long int n {18446744073709551615ul};
    const std::list<unsigned long int> factors {3,5,17,257,641,65537ul,6700417ul};

    // Test only valid for 8-byte (64-bit) integers
    if( sizeof(unsigned long int) >= 8 )
    {
        BOOST_REQUIRE_EQUAL( n , std::accumulate(factors.begin(), factors.end(), 1ul, std::multiplies<unsigned long int>()) );
        BOOST_CHECK_EQUAL( primeFactors(n) , factors );
    }
}

BOOST_AUTO_TEST_CASE( FactorsOfPrimeInt )
{
    const unsigned long int n {13713779ul};
    const std::list<unsigned long int> factors {n};

    BOOST_CHECK_EQUAL( primeFactors(n) , factors );
}

BOOST_AUTO_TEST_CASE( RepeatedFactors )
{
    const unsigned long int n {71672ul};
    const std::list<unsigned long int> factors {2,2,2,17,17,31};

    BOOST_REQUIRE_EQUAL( n , std::accumulate(factors.begin(), factors.end(), 1ul, std::multiplies<unsigned long int>()) );
    BOOST_CHECK_EQUAL( primeFactors(n) , factors );
}
