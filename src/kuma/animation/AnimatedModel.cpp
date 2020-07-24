#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <kuma/AssimpLoader.hpp>
#include <kuma/default_resources.hpp>
#include "AnimatedModel.hpp"

namespace life::kuma {

    AnimatedModel::AnimatedModel(const std::string& file_path, int import_process_steps) {
        m_bones = std::make_shared<Skeleton>();
        LoadModel(file_path, import_process_steps);
    }

    void AnimatedModel::LoadModel(const std::string& f_path, int import_process_steps) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(f_path, (aiPostProcessSteps)import_process_steps);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            const char* err = importer.GetErrorString();
            if(err[0]) {
                LIFE_ERR("ASSIMP ERROR: '%s'", err);
            } else {
                LIFE_ERR("ASSIMP ERROR: Please check if the model contains mesh data!");
            }
            return;
        }

        m_meshes.reserve(scene->mNumMeshes);
        m_animations.reserve(scene->mNumAnimations);
        m_bones->clear();
        auto _importer = AssimpLoader<AnimatedMesh>{m_bones.get()};
        _importer.ProcessScene(scene, m_meshes);
        _importer.ProcessAnimations(scene, m_animations);
        m_ginverse = _importer.m_ginverse;
        for (auto& mesh : m_meshes) {
        	auto props = mesh.GetMaterial().GetProperties();
            mesh.GetMaterial().SetProperty("u_bones", m_bones);
            auto a = props.find(mesh.GetMaterial().GetShader().GetUniformLocation("u_material.diffuse"));
            if(a == props.end()) {
            	mesh.GetMaterial().SetProperty("u_material.diffuse", kuma::default_resources::get_diffuse());
            }
        }
    }

    bool AnimatedModel::AddAnimation(const std::string& anim_file_path, int import_process_steps) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(anim_file_path, (aiPostProcessSteps)import_process_steps);
        if(!scene || !scene->mRootNode) {
            LIFE_ERR("ASSIMP ERROR: '%s'", importer.GetErrorString());
            return false;
        }
        auto cnt = m_animations.size();
        auto _importer = AssimpLoader<AnimatedMesh>{m_bones.get()};
        _importer.ProcessAnimations(scene, m_animations);
        bool isbigger = cnt < m_animations.size();
        if(!isbigger) {
            LIFE_WARN("No additional animations were loaded for some reason!");
        }
        return isbigger;
    }
}
