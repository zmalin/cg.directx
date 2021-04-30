// ch01.deadlock.example.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <thread>

class CriticalSection
{
public:
    CriticalSection(void);
    ~CriticalSection(void);

    void lock(void);
    void unlock(void);

private:
    CRITICAL_SECTION m_cs;
};
class CriticalSectionLock
{
public:
    CriticalSectionLock(CriticalSection& _cs);
    ~CriticalSectionLock(void);

private:
    CriticalSection* m_csPtr;
};

CriticalSection::CriticalSection(void)
{
    ::InitializeCriticalSectionEx(&m_cs, 0, 0);
}
CriticalSection::~CriticalSection(void)
{
    ::DeleteCriticalSection(&m_cs);
}
void CriticalSection::lock(void)
{
    ::EnterCriticalSection(&m_cs);
}
void CriticalSection::unlock(void)
{
    ::LeaveCriticalSection(&m_cs);
}
CriticalSectionLock::CriticalSectionLock(CriticalSection& _cs)
{
    m_csPtr = &_cs;
    m_csPtr->lock();
}
CriticalSectionLock::~CriticalSectionLock(void)
{
    m_csPtr->unlock();
}

int main()
{
    int a, b;
    CriticalSection mutex1, mutex2;
    std::thread th1([&]()
    {
        while (true)
        {
            CriticalSectionLock lock1(mutex1);
            ++a;
            CriticalSectionLock lock2(mutex2);
            ++b;
            std::cout << "t1 done" << std::endl;
        }
    });

    std::thread th2([&]()
    {
        while (true)
        {
            CriticalSectionLock lock2(mutex2);
            ++b;
            CriticalSectionLock lock1(mutex1);
            ++a;
            std::cout << "t2 done" << std::endl;
        }
    });

    th1.join();
    th2.join();
}

