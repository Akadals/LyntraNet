
#include <LT/Common/Utility/Concurrency/ConcurrentRingBuffer.h>
#include <LT/LyntraNet.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace LT;
using namespace std;

namespace
{
    constexpr size_t BufferSize = 65536;
    constexpr size_t Iteration = 10'000'000;
    constexpr size_t WarmupIteration = 100'000;
    constexpr size_t BenchmarkRepeat = 7;

    using Clock = chrono::steady_clock;

    struct BenchmarkResult
    {
        double avgNs = 0.0;
        double throughputMBps = 0.0;
        double elapsedMs = 0.0;
    };

    template<size_t DataSize>
    bool ValidateRingBuffer(
        RingBuffer<SPSC>& rbuf,
        const array<std::byte, DataSize>& src,
        array<std::byte, DataSize>& dst,
        size_t iteration)
    {
        for (size_t i = 0; i < iteration; ++i)
        {
            if (!rbuf.template TryWrite<DataSize>(src.data()))
                return false;

            if (!rbuf.template TryRead<DataSize>(dst.data()))
                return false;

            if (dst != src)
                return false;
        }

        return true;
    }

    template<size_t DataSize>
    bool RunWarmup(
        RingBuffer<SPSC>& rbuf,
        const array<std::byte, DataSize>& src,
        array<std::byte, DataSize>& dst)
    {
        for (size_t i = 0; i < WarmupIteration; ++i)
        {
            if (!rbuf.template TryWrite<DataSize>(src.data()))
                return false;

            if (!rbuf.template TryRead<DataSize>(dst.data()))
                return false;
        }

        return dst == src;
    }

    template<size_t DataSize>
    bool MeasureOnce(
        BenchmarkResult& result,
        const array<std::byte, DataSize>& src,
        array<std::byte, DataSize>& dst)
    {
        RingBuffer<SPSC> rbuf(BufferSize);

        if (!RunWarmup<DataSize>(rbuf, src, dst))
            return false;

        // Correctness check is outside the timed section.
        if (!ValidateRingBuffer<DataSize>(rbuf, src, dst, 1))
            return false;

        const auto start = Clock::now();

        for (size_t i = 0; i < Iteration; ++i)
        {
            if (!rbuf.template TryWrite<DataSize>(src.data()))
                return false;

            if (!rbuf.template TryRead<DataSize>(dst.data()))
                return false;
        }

        const auto end = Clock::now();

        if (dst != src)
            return false;

        const double elapsedNs =
            static_cast<double>(
                chrono::duration_cast<chrono::nanoseconds>(
                    end - start).count());

        result.elapsedMs = elapsedNs / 1'000'000.0;

        // One write or one read counts as one operation.
        result.avgNs = elapsedNs /
            static_cast<double>(Iteration * 2);

        // Count both writes and reads as data processed.
        const double totalBytes =
            static_cast<double>(Iteration) *
            static_cast<double>(DataSize) * 2.0;

        const double totalMB = totalBytes / (1024.0 * 1024.0);

        result.throughputMBps =
            totalMB / (elapsedNs / 1'000'000'000.0);

        return true;
    }

    template<size_t DataSize>
    void Benchmark()
    {
        array<std::byte, DataSize> src{};
        array<std::byte, DataSize> dst{};

        for (size_t i = 0; i < DataSize; ++i)
            src[i] = static_cast<std::byte>(i & 0xFF);

        vector<BenchmarkResult> results;
        results.reserve(BenchmarkRepeat);

        for (size_t repeat = 0; repeat < BenchmarkRepeat; ++repeat)
        {
            BenchmarkResult result{};

            if (!MeasureOnce<DataSize>(result, src, dst))
            {
                cerr << "Benchmark failed: DataSize="
                    << DataSize
                    << ", repeat=" << repeat + 1
                    << '\n';
                return;
            }

            results.push_back(result);
        }

        // Use median to reduce the impact of occasional outliers.
        sort(results.begin(), results.end(),
            [](const BenchmarkResult& a, const BenchmarkResult& b)
            {
                return a.elapsedMs < b.elapsedMs;
            });

        const BenchmarkResult& median =
            results[results.size() / 2];

        cout << left
            << setw(10) << DataSize
            << setw(18) << fixed << setprecision(2)
            << median.avgNs
            << setw(18) << fixed << setprecision(2)
            << median.throughputMBps
            << setw(18) << fixed << setprecision(2)
            << median.elapsedMs
            << '\n';
    }
}

int main()
{
    cout << "============================================================\n";
    cout << "          LyntraNet SPSC RingBuffer Benchmark\n";
    cout << "============================================================\n\n";

#ifdef NDEBUG
    cout << "Build Configuration : Release (NDEBUG defined)\n";
#else
    cout << "Build Configuration : Debug or NDEBUG not defined\n";
#endif

    cout << "Buffer Size         : " << BufferSize << " bytes\n";
    cout << "Iterations          : " << Iteration << '\n';
    cout << "Warmup Iterations   : " << WarmupIteration << '\n';
    cout << "Benchmark Repeats   : " << BenchmarkRepeat << '\n';
    cout << "Thread Model        : Single-threaded\n\n";

    cout << left
        << setw(10) << "Size(B)"
        << setw(18) << "Avg(ns/op)"
        << setw(18) << "MB/s"
        << setw(18) << "Time(ms)"
        << '\n';

    cout << "------------------------------------------------------------\n";

    Benchmark<16>();
    Benchmark<32>();
    Benchmark<64>();
    Benchmark<128>();
    Benchmark<256>();
    Benchmark<512>();
    Benchmark<1024>();
    Benchmark<2048>();
    Benchmark<4096>();

    cout << "============================================================\n";

    return 0;
}
