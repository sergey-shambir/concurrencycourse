#include <algorithm>
#include <benchmark/benchmark.h>
#include <functional>
#include <future>
#include <vector>

using Container = std::list<int>;
using Iterator = Container::iterator;
using SorterFunc = std::function<Container(Container&& unsorted)>;

// Do not modify
Container std_sort(Container&& unsorted)
{
	unsorted.sort();
	return unsorted;
}

// Do not modify
Container unoptimized_sort(Container&& unsorted)
{
	if (unsorted.empty())
	{
		return unsorted;
	}

	Container result;
	result.splice(result.begin(), unsorted, unsorted.begin());
	auto const& partition_val = *result.begin();

	auto divide_point = std::partition(unsorted.begin(), unsorted.end(), [&](auto const& val) { return val < partition_val; });

	Container new_lower_chunk;
	new_lower_chunk.splice(new_lower_chunk.end(), unsorted, unsorted.begin(), divide_point);

	std::future<Container> new_lower = std::async(std::launch::async, unoptimized_sort, std::move(new_lower_chunk));

	Container new_higher(unoptimized_sort(std::move(unsorted)));

	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower.get());

	return result;
}

// Optimize me
Container optimized_sort(Container&& unsorted)
{
	if (unsorted.empty())
	{
		return unsorted;
	}

	Container result;
	result.splice(result.begin(), unsorted, unsorted.begin());
	auto const& partition_val = *result.begin();

	auto divide_point = std::partition(unsorted.begin(), unsorted.end(), [&](auto const& val) { return val < partition_val; });

	Container new_lower_chunk;
	new_lower_chunk.splice(new_lower_chunk.end(), unsorted, unsorted.begin(), divide_point);

	std::future<Container> new_lower = std::async(std::launch::async, unoptimized_sort, std::move(new_lower_chunk));

	Container new_higher(unoptimized_sort(std::move(unsorted)));

	result.splice(result.end(), new_higher);
	result.splice(result.begin(), new_lower.get());

	return result;
}

// Do not modify
void sort_impl(benchmark::State& state, const SorterFunc& sorterFunc)
{
	for (auto _ : state)
	{
		state.PauseTiming();
		Container data;
		srand(0);
		for (int i = 0; i < state.range(); ++i)
		{
			data.push_back(rand());
		}
		state.ResumeTiming();
		data = sorterFunc(std::move(data));
		state.PauseTiming();
		if (!std::is_sorted(data.begin(), data.end()))
		{
			throw std::logic_error("the range is not sorted correctly");
		}
	}
}

// Do not modify
void single_threaded(benchmark::State& state)
{
	sort_impl(state, std_sort);
}

// Do not modify
void unoptimized(benchmark::State& state)
{
	sort_impl(state, unoptimized_sort);
}

// Do not modify
void optimized(benchmark::State& state)
{
	sort_impl(state, optimized_sort);
}

BENCHMARK(single_threaded)->Range(8, 8 << 5)->Threads(1);
BENCHMARK(unoptimized)->Range(8, 8 << 5)->Threads(1);
BENCHMARK(optimized)->Range(8, 8 << 5)->Threads(1);