#include <LT/LyntraNet.h>

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>

using namespace LyntraNet;
using namespace std;

template<size_t DataSize>
void Benchmark()
{
    constexpr size_t BufferSize = 65536;
    constexpr size_t Iteration = 10'000'000;

    
    
    RingBuffer<FastSPSC> rbuf(BufferSize);

    array<std::byte, DataSize> src{};
    array<std::byte, DataSize> dst{};

    for (size_t i = 0; i < DataSize; ++i)
        src[i] = static_cast<std::byte>(i);

    // Warm-up
    for (size_t i = 0; i < 100'000; ++i)
    {
        rbuf.TryWrite<DataSize>(src.data());
        rbuf.TryRead<DataSize>(dst.data());
    }

    auto start = chrono::steady_clock::now();

    for (size_t i = 0; i < Iteration; ++i)
    {
        rbuf.TryWrite<DataSize>(src.data());
        rbuf.TryRead<DataSize>(dst.data());
    }

    auto end = chrono::steady_clock::now();

    double totalNs =
        chrono::duration_cast<chrono::nanoseconds>(end - start).count();

    double avgNs = totalNs / (Iteration * 2);

    double totalMB =
        (double)(Iteration * DataSize * 2) /
        (1024.0 * 1024.0);

    double throughput =
        totalMB /
        (totalNs / 1'000'000'000.0);

    cout << left
        << setw(8) << DataSize
        << setw(15) << fixed << setprecision(2) << avgNs
        << setw(15) << fixed << setprecision(2) << throughput
        << '\n';
}

int main()
{
    cout << "==============================================\n";
    cout << "      LyntraNet SPSC RingBuffer Benchmark\n";
    cout << "==============================================\n\n";

    cout << left
        << setw(8) << "Size"
        << setw(15) << "Avg(ns/op)"
        << setw(15) << "MB/s"
        << '\n';

    cout << "----------------------------------------------\n";

    Benchmark<16>();
    Benchmark<32>();
    Benchmark<64>();
    Benchmark<128>();
    Benchmark<256>();
    Benchmark<512>();
    Benchmark<1024>();
}