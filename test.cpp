#include <iostream>
#include <iomanip>
#include <cmath>
#include <chrono>

using namespace std;

class muTimer
{
    using Clock = std::chrono::high_resolution_clock;
    bool active = false;
    Clock::duration   duration_;
    Clock::time_point start_ = Clock::now(), stop_ = Clock::now();

    muTimer(const muTimer&)             = delete;
    muTimer& operator=(const muTimer&)  = delete;
public:
    using ns       = std::chrono::nanoseconds;
    using mks      = std::chrono::microseconds;
    using ms       = std::chrono::milliseconds;
    muTimer() { reset(); start(); }
    ~muTimer() = default;
    muTimer& reset()
    {
        duration_ = std::chrono::nanoseconds(0);
        active    = false;
        return *this;
    }
    muTimer& start()
    {
        if (!active)
        {
            start_ = Clock::now();
            active = true;
        }
        return *this;
    }
    muTimer& stop()
    {
        if (active)
        {
            stop_      = Clock::now();
            duration_ += stop_ - start_;
            active     = false;
        }
        return *this;
    }
    template<typename T = mks>
        unsigned long long duration()
    {
        return static_cast<unsigned long long>
            (std::chrono::duration_cast<T>(stop_-start_).count());
    }
};

const int Count = 1000000;

inline unsigned long long powpow(double& res, int N)
{
    res = 0;
    muTimer mt;
    for(int i = 0; i < Count; ++i)
    {
        res += pow((i+Count/2)/double(Count + i/2),N);
    }
    mt.stop();
    return mt.duration<>();
}
inline unsigned long long powexp(double& res, int N)
{
    res = 0;
    muTimer mt;
    for(int i = 0; i < Count; ++i)
    {
        res += exp(log((i+Count/2)/double(Count + i/2))*N);
    }
    mt.stop();
    return mt.duration<>();
}
inline unsigned long long powmul(double& res, int N)
{
    res = 0;
    muTimer mt;
    for(int i = 0; i < Count; ++i)
    {
        double s = 1, x = (i+Count/2)/double(Count + i/2);
        for(int j = 0; j < N; ++j) s*=x;
        res += s;
    }
    mt.stop();
    return mt.duration<>();
}

int main(int argc, const char * argv[])
{

    double s = 0;
    for(int N = 2; N < 100; ++N)
    {
        double r1,r2,r3;
        cout << N << " " << powpow(r1,N)/1000.0
                  << " " << powexp(r2,N)/1000.0
                  << " " << powmul(r3,N)/1000.0 << endl;
        s += r1+r2+r3;
    }
    cout << s;

}
