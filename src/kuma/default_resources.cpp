#include "default_resources.hpp"
#include <util/FileSystem.hpp>

namespace life::kuma::default_resources {
	
	TYPE_PTR(kuma::Texture) diffuse_tex;
	TYPE_PTR(kuma::Texture) get_diffuse() { return diffuse_tex; }
	
	TYPE_PTR(kuma::Texture) sprite_tex;
	TYPE_PTR(kuma::Texture) get_sprite() { return sprite_tex; }
	
	void load_default_resources() {
		diffuse_tex = kuma::Texture::Create(getexepath()+"/res/256x256.jpg"); // TODO: Settle on a default diffuse texture
		sprite_tex  = kuma::Texture::Create(getexepath()+"/res/sprite.png");
	}
	
}
