#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector>
#include "Mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "assetdefines.h"

namespace life::kuma {
	class Model {
	private:
		std::vector<StaticMesh> m_meshes;

	public:
		Model() = delete;
		Model(const Model& other) = default;
		explicit Model(const std::string& file_path, int import_process_steps = KUMA_ASSETIMPORTSETTING_DEFAULT);
        Model(Model&&) = default;
        ~Model() = default;

		std::vector<StaticMesh>& GetMeshes() { return m_meshes; }
	private:
		void LoadModel(const std::string& filepath, int import_process_steps);
	};
}

#endif //CLASSNAME_HPP
