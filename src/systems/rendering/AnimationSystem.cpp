#include <util/Time.h>
#include "AnimationSystem.hpp"
#include "RenderSystem.hpp"
#include "layers/staging/ComponentStaging.hpp"
#include <components/components>

namespace life {

    AnimationSystem::AnimationSystem() {
        AddComponentType(ModelComponent::ID);
        FinalizeComponentAdding();
    }

    void AnimationSystem::Update(const std::shared_ptr<ecs::BaseComponent>& component) {
        auto animated_model = std::dynamic_pointer_cast<ModelComponent>(component)->model;
        if(animated_model->m_animations.empty()) return;
        // Update skeleton based off of time.#include "layers/staging/ComponentStaging.hpp"

        auto sel = animated_model->m_selected_animation;
        animated_model->dbg_time_point += Time::GetFrameTime() * animated_model->m_animations[sel].Speed;
        animated_model->dbg_time_point = fmod(animated_model->dbg_time_point, animated_model->m_animations[sel].Duration);
        auto func = [&animated_model = std::as_const(animated_model), sel](kuma::Bone& b, const glm::mat4& ptrans) -> glm::mat4 {
            float time_point = animated_model->dbg_time_point;
            const auto& anim = animated_model->m_animations[sel];
            glm::mat4 ntrans = b.BaseTransform;
            auto channel = anim.AnimationChannels.find(b.BoneName);
            if(channel != anim.AnimationChannels.end()) {
                // Scale
                auto sc_b = channel->second.ScalingKeys.upper_bound(time_point);
                auto sc_a = sc_b; sc_a--;
                if(sc_b == channel->second.ScalingKeys.end()) sc_b = channel->second.ScalingKeys.begin();
                auto scaling_matrix = glm::scale(glm::mat4(1), kuma::Animation::interpolate_vec3(sc_a, sc_b, time_point));

                // Rotation
                auto rq_b = channel->second.RotationKeys.upper_bound(time_point);
                auto rq_a = rq_b; rq_a--;
                if(rq_b == channel->second.RotationKeys.end()) rq_b = channel->second.RotationKeys.begin();
                auto rotation_matrix = glm::mat4_cast(kuma::Animation::interpolate_quat(rq_a, rq_b, time_point));

                // Translation
                auto tv_b = channel->second.PositionKeys.upper_bound(time_point);
                auto tv_a = tv_b; tv_a--;
                if(tv_b == channel->second.PositionKeys.end()) tv_b = channel->second.PositionKeys.begin();
                auto translation_matrix = glm::translate(glm::mat4(1), kuma::Animation::interpolate_vec3(tv_a, tv_b, time_point));

                ntrans = translation_matrix * rotation_matrix * scaling_matrix;
            }

            glm::mat4 gtrans = ptrans * ntrans; // gtrans = object-space animation transformation
            b.FinalTransform = animated_model->m_ginverse * gtrans * b.Offset; // FinalTransform = world-space animation transformation
            return gtrans;
        };
        kuma::SkeletonApplyFunctionTopDown<glm::mat4>(func, *animated_model->m_bones, kuma::default_shapes::mat4_identity_matrix);
    }
}