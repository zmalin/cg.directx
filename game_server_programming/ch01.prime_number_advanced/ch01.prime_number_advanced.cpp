// ch01.prime_number_advanced.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>

bool prime(int _num)
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
void print(std::vector<int> const& _primes)
{
    for (auto n : _primes)
    {
        std::cout << n << ',';
    }
    std::cout << std::endl;
}

int const g_maxCount = 150000;
int const g_maxThread = 4;

int main()
{
    auto number = 1;
    std::vector<int> primes;
    
    auto t0 = std::chrono::system_clock::now();

    std::vector<std::shared_ptr<std::thread>> pool;
    std::recursive_mutex numberMutex, primeMutex;

    for (auto t = 0; t < g_maxThread; ++t)
    {
        std::shared_ptr<std::thread> thread(new std::thread([&]() 
        {
            while (true)
            {
                int n;
                {
                    std::lock_guard<std::recursive_mutex> lock(numberMutex);
                    n = number;
                    ++number;
                }
                if (n > g_maxCount)
                {
                    break;
                }
                if (prime(n))
                {
                    std::lock_guard<std::recursive_mutex> lock(primeMutex);
                    primes.push_back(n);
                }
            }
        }));

        pool.push_back(thread);
    }

    for (auto th : pool)
    {
        th->join();
    }

    auto t1 = std::chrono::system_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    std::cout << "took " << duration << " milliseconds" << std::endl;
    std::cout << "total count = " << static_cast<int>(primes.size()) << std::endl;
    print(primes);
}

