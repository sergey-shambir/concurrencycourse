#include "concurrent_map.h"
#include <string>

int main()
{
	concurrent_map<int, std::string> stringMap;
	stringMap.Update(1, std::to_string(1));
	stringMap.Fetch(1, [&](const auto& valuePair) {
		stringMap.Update(valuePair.first + 1, std::to_string(valuePair.first + 1));
	});
	return 0;
}