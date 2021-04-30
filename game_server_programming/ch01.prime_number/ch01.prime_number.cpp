// ch01.prime_number.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <chrono>

const int g_maxCount = 150000;

bool primeNumber(int _num)
{
    if (_num == 1)
    {
        return false;
    }
    if (_num == 2 || _num == 3)
    {
        return true;
    }

    for (auto i = 2; i < _num; ++i)
    {
        if (_num % i == 0)
        {
            return false;
        }
    }

    return true;
}

void printNumbers(std::vector<int> const& _nums)
{
    for (auto n : _nums)
    {
        std::cout << n << ',';
    }
    std::cout << std::endl;
}

int main()
{
    std::vector<int> primes;
    std::vector<std::shared_ptr<std::thread>> pool;

    auto t0 = std::chrono::system_clock::now();

    for (auto i = 1; i <= g_maxCount; ++i)
    {
        if (primeNumber(i))
        {
            primes.push_back(i);
        }
    }

    auto t1 = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << "Took " << duration << " millisecons" << std::endl;

    printNumbers(primes);
}
