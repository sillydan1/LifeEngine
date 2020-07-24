#ifndef LIFE_ENGINE_ANIMATION_HPP
#define LIFE_ENGINE_ANIMATION_HPP
#include <string>
#include <unordered_map>
#include <map>
#include <vector>
#include <kuma/openglmath>
#include <life_core.h>
#include <glm/gtc/quaternion.hpp>

namespace life::kuma {
    template<typename T>
    using time_map = std::map<double, T>;

    struct Animation {
        struct AnimationNode {
            time_map<glm::vec3> PositionKeys;
            time_map<glm::quat> RotationKeys;
            time_map<glm::vec3> ScalingKeys;
            // PositionKeys.lower_bound(3.0)->second.value;
            // Note: For RotationKeys, use glm::slerp
        };
        std::string Name;
        double Duration, Speed;
        std::unordered_map<std::string, AnimationNode> AnimationChannels;
        // TODO: Post- and Pre-state blending properties

        void AddAnimationNode(const std::string& node_key, AnimationNode&& animnode) {
            AnimationChannels.insert({node_key, std::move(animnode)});
        }

        static glm::vec3 interpolate_vec3(time_map<glm::vec3>::const_iterator& a, time_map<glm::vec3>::const_iterator& b, double time_point) {
            if(a == b) return a->second;
            auto dtime  = static_cast<float>(b->first - a->first);
            float factor = static_cast<float>(time_point - a->first) / dtime;
            if(!(factor >= 0.0f && factor <= 1.0f)) {
                LIFE_ERR("Critical Error! Animation Factor is out of bounds: %.5f", factor);
                LIFE_ASSERT(factor >= 0.0f && factor <= 1.0f);
            }
            glm::vec3 d = b->second - a->second;
            return a->second + factor * d;
        }

        static glm::quat interpolate_quat(time_map<glm::quat>::const_iterator& a, time_map<glm::quat>::const_iterator& b, double time_point) {
            if(a == b) return a->second;
            auto dtime   = static_cast<float>(b->first - a->first);
            float factor = static_cast<float>(time_point - a->first) / dtime;
            if(!(factor >= 0.0f && factor <= 1.0f)) {
                LIFE_ERR("Critical Error! Animation Factor is out of bounds: %.5f", factor);
                LIFE_ASSERT(factor >= 0.0f && factor <= 1.0f);
            }
            return glm::normalize(glm::mix(a->second, b->second, factor));
        }
    };
}

#endif //LIFE_ENGINE_ANIMATION_HPP
