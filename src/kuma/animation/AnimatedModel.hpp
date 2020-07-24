#ifndef LIFE_ENGINE_ANIMATEDMODEL_HPP
#define LIFE_ENGINE_ANIMATEDMODEL_HPP
#include <vector>
#include <kuma/Mesh.hpp>
#include <kuma/assetdefines.h>
#include "Bone.hpp"
#include "Animation.hpp"

namespace life::kuma {
    class AnimatedModel {
    public:
        std::vector<AnimatedMesh> m_meshes;
        std::vector<Animation>    m_animations; // TODO: Should animations be on the GRAM?
        glm::mat4                 m_ginverse;
        TYPE_PTR(Skeleton) m_bones;
        float dbg_time_point = 0.0;
        int   m_selected_animation = 0;

        AnimatedModel() = delete;
        explicit AnimatedModel(const std::string& file_path, int import_process_steps = KUMA_ASSETIMPORTSETTING_DEFAULT);
        AnimatedModel(const AnimatedModel&) = default;
        AnimatedModel(AnimatedModel&&) = default;
        ~AnimatedModel() = default;

        std::vector<AnimatedMesh>& GetMeshes() { return m_meshes; }

        bool AddAnimation(const std::string& anim_file_path, int import_process_steps = KUMA_ASSETIMPORTSETTING_DEFAULT);
    private:
        void LoadModel(const std::string& f_path, int import_process_steps);
    };

}
#endif //LIFE_ENGINE_ANIMATEDMODEL_HPP
