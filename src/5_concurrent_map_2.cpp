#include <benchmark/benchmark.h>
#include "concurrent_map.h"

// optimize me
template <class Key, class Value>
class optimized_concurrent_map
{
public:
	using Pair = std::pair<Key, Value>;
	using FetchHandler = std::function<void(const Pair&)>;

	void Update(const Key& key, const Value& value)
	{
		std::lock_guard lk(m_mutex);
		m_data[key] = value;
	}

	std::optional<Value> Get(const Key& key) const
	{
		std::lock_guard lk(m_mutex);
		if (auto it = m_data.find(key); it != m_data.end())
		{
			return it->second;
		}
		return {};
	}

	void Remove(const Key& key)
	{
		std::lock_guard lk(m_mutex);
		m_data.erase(key);
	}

	void Fetch(const Key& key, const FetchHandler& fetchHandler) const
	{
		std::lock_guard lk(m_mutex);
		if (auto it = m_data.find(key); it != m_data.end())
		{
			fetchHandler(*it);
		}
	}

	std::map<Key, Value> Dump() const
	{
		std::lock_guard lk(m_mutex);
		return m_data;
	}

private:
	std::map<Key, Value> m_data;
	mutable std::mutex m_mutex;
};

constexpr int MAP_SIZE = 100;

// Do not modify
template<class T>
void bench_body(benchmark::State& state)
{
	// Do not modify
	srand(0);
	T stringMap;
	for (int i = 0; i < MAP_SIZE; ++i)
	{
		stringMap.Update(i, std::to_string(rand()));
	}
	for (auto _ : state)
	{
		int64_t sum = 0;
		for (int i = 0; i < state.range(); ++i)
		{
			sum += std::stoi(stringMap.Get(i % MAP_SIZE).value());
		}
	}
}

void unoptimized(benchmark::State& state)
{
	bench_body<concurrent_map<int, std::string>>(state);
}

void optimized(benchmark::State& state)
{
	bench_body<optimized_concurrent_map<int, std::string>>(state);
}

BENCHMARK(unoptimized)->Range(8, 8 << 10);
BENCHMARK(optimized)->Range(8, 8 << 10);