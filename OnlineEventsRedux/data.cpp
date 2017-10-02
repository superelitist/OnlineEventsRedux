#include <string>
#include <vector>
#include "..\..\inc\types.h"


static constexpr uint32_t JenkinsOneAtATime(const char* str) {
	uint32_t hash = 0;

	for (; *str; ++str)	{
		hash += ::tolower(*str); // RAGE specific
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);

	return hash;
}

class HashList {
public:
	HashList(std::initializer_list<std::string> list) {
		for (auto& entry : list) {
			m_list.push_back({ JenkinsOneAtATime(entry.c_str()), entry });
		}
	}

	// begin(), end(), GetName() etc...

private:
	struct HashPair	{
		Hash hash;
		std::string name;
	};

	std::vector<HashPair> m_list;
};

static HashList pedHashes = { "hash_one", "hash_two", "hash_three" };

