#include "ComponentStaging.hpp"

namespace life {
	
	WorldCube::WorldCube(glm::vec3 position)
			: Component(),
			  transform{std::make_shared<kuma::Transform>(position)},
			  mesh{	kuma::default_shapes::box,
					kuma::default_shapes::box_indices,
					kuma::Material::Create("res/shaders/default.glsl")},
			  texture{kuma::Texture::FromFilePath("res/256x256.jpg")}
	{
		mesh.GetMaterial().SetProperty("u_model", 			transform);
		mesh.GetMaterial().SetProperty("u_material.diffuse",  texture);
		mesh.GetMaterial().SetProperty("u_material.shininess",  32.0f);
	}
	
	WorldCube::WorldCube()
			: Component(),
			  transform{std::make_shared<kuma::Transform>(glm::vec3(0,0,0))},
			  mesh{	kuma::default_shapes::box,
		  			kuma::default_shapes::box_indices,
					kuma::Material::Create("res/shaders/default.glsl")},
			  texture{kuma::Texture::FromFilePath("res/256x256.jpg")}
	{
		mesh.GetMaterial().SetProperty("u_model", 			transform);
		mesh.GetMaterial().SetProperty("u_material.diffuse",  texture);
		mesh.GetMaterial().SetProperty("u_material.shininess",  32.0f);
	}
	
	
	GameCard::GameCard()
			: 	Component(),
				cardname{"unnamed"},
			 	transform{std::make_shared<kuma::Transform>(glm::vec3(0), glm::quat(0,0,0,0), glm::vec3(-1,1,1))},
			 	mesh{std::make_shared<kuma::StaticMesh>(
			 			kuma::default_shapes::quad,
					  kuma::default_shapes::quad_indices,
					  kuma::Material::Create("res/shaders/card.glsl"))},
			 	card_texture{kuma::Texture::FromFilePath("res/textures/cards/Normal.png")}
	{
		mesh->GetMaterial().SetProperty("u_model", transform);
		mesh->GetMaterial().SetProperty("u_material.diffuse", card_texture);
		mesh->GetMaterial().SetProperty("u_material.shininess", 32.0f); // Not needed, but okay
	}
	
	GameCard::GameCard(glm::vec3 start_pos)
	 : 	Component(),
	 	cardname{"unnamed"},
	 	transform{std::make_shared<kuma::Transform>(start_pos)},
	 	mesh{std::make_shared<kuma::StaticMesh>(
	 		 kuma::default_shapes::quad,
			 kuma::default_shapes::quad_indices,
			 kuma::Material::Create("res/shaders/card.glsl"))},
		card_texture{kuma::Texture::FromFilePath("res/textures/cards/Normal.png")}
	{
		mesh->GetMaterial().SetProperty("u_model", transform);
		mesh->GetMaterial().SetProperty("u_material.diffuse", card_texture);
		mesh->GetMaterial().SetProperty("u_material.shininess", 32.0f); // Not needed, but okay
	}
}
