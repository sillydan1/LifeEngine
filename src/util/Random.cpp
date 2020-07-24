#include "Random.hpp"

namespace life {
	std::default_random_engine* generator = nullptr;
	
	void Random::Init() {
		generator = new std::default_random_engine();
	}
	
	void Random::Destroy() {
		delete generator;
		generator = nullptr;
	}
	
	float Random::GenerateFloat() {
		std::uniform_real_distribution<float> d{std::numeric_limits<float>::min(), std::numeric_limits<float>::max()};
		return d(*generator);
	}
	
	float Random::GenerateFloat(float range_start, float range_end) {
		std::uniform_real_distribution<float> d{range_start, range_end};
		return d(*generator);
	}
	
	int Random::GenerateInt() {
		std::uniform_int_distribution<int> d{std::numeric_limits<int>::min(), std::numeric_limits<int>::max()};
		return d(*generator);
	}
	
	int Random::GenerateInt(int range_start, int range_end) {
		std::uniform_int_distribution<int> d{range_start, range_end};
		return d(*generator);
	}

    // TODO: Right now this is just bogo-unique. It shouldn't be, but this is what I have so far.
	glm::vec3 Random::GenerateVec3(float range_start, float range_end) {
		return glm::vec3(Random::GenerateFloat(range_start,range_end),
						 Random::GenerateFloat(range_start,range_end),
						 Random::GenerateFloat(range_start,range_end));
	}
}
