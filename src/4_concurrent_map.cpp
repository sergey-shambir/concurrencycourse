#include "concurrent_map.h"
#include <string>

constexpr int MAP_SIZE = 100;

int main(void)
{
	/**
	 * TODO: Исправить падение (или зависание) приложения
	 */

	srand(0);
	concurrent_map<int, std::string> stringMap;
	concurrent_map<int, std::string>::FetchHandler fetchHandler = [&](const auto& valuePair) {
		stringMap.Update(valuePair.first + 1, std::to_string(rand()));
		if (valuePair.first < MAP_SIZE)
		{
			stringMap.Fetch(valuePair.first + 1, fetchHandler);
		}
	};
	stringMap.Update(0, std::to_string(0));
	stringMap.Fetch(0, fetchHandler);
}