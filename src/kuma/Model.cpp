#include "Model.hpp"
#include "AssimpLoader.hpp"

namespace life::kuma {
	
	Model::Model(const std::string& file_path, int import_process_steps) {
		 LoadModel(file_path, import_process_steps);
	}
	
	void Model::LoadModel(const std::string& filename, int import_process_steps) {
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(filename, (aiPostProcessSteps)import_process_steps);//aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenNormals);
		
		if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			LIFE_ERR("ASSIMP ERROR: %s", importer.GetErrorString());
			return;
		}
        m_meshes.reserve(scene->mNumMeshes);
		AssimpLoader<StaticMesh>{}.ProcessScene(scene, m_meshes);
	}
}
