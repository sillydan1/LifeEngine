#ifndef LIFE_ENGINE_DEFAULT_RESOURCES_H
#define LIFE_ENGINE_DEFAULT_RESOURCES_H
#include "Texture.hpp"
#include "kuma_math_types.hpp"

namespace life::kuma::default_resources {

	/// The default texture.
	TYPE_PTR(kuma::Texture) get_diffuse();
	/// The default transparent texture (for e.g. particles)
	TYPE_PTR(kuma::Texture) get_sprite();
	
	void load_default_resources();
}

#endif //LIFE_ENGINE_DEFAULT_RESOURCES_H
