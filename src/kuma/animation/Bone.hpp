#ifndef LIFE_ENGINE_BONE_HPP
#define LIFE_ENGINE_BONE_HPP
#include <life_core.h>
#include <kuma/Vertex.hpp>
#include <unordered_map>
#include <map>
#include <functional>

namespace life::kuma {
    struct Bone {
        int bone_index;
        std::string BoneName;
        glm::mat4 Offset;
        glm::mat4 FinalTransform;
        glm::mat4 BaseTransform;
        Bone* parent; // raw_ptr, because nullable

        Bone(int i, std::string&& bonename, Bone* parent = nullptr)
                : bone_index(i), BoneName{std::move(bonename)}, Offset{0}, FinalTransform{0}, BaseTransform{0}, parent{parent}
        { }

        Bone* get_root_bone() {
            auto s = parent; // 's' for 'search'
            auto r = this;   // 'r' for 'result'
            while(s) {
                r = s;
                s = s->parent;
            }
            return r;
        }
    };
    using Skeleton = std::unordered_map<std::string, Bone>;

    template<typename T>
    void BoneApplyFuncTopDown(std::function<T(Bone&, const T&)> f, Bone& b, const T& root_start, std::map<Bone*, T>& visited) {
        T val{};
        if(!b.parent) {
            val = f(b, root_start);
        } else if(visited.find(b.parent) != visited.end()) {
            val = f(b, visited.at(b.parent)); // No recursion here (this is the dynamic programming part)
        } else {
            BoneApplyFuncTopDown(f, *b.parent, root_start, visited);
            val = f(b, visited.at(b.parent));
        }
        visited.insert({&b, val});
    }

    template<typename T>
    void SkeletonApplyFunctionTopDown(std::function<T(Bone&, const T&)> f, Skeleton& sk, const T& start_val) {
        std::map<Bone*, T> visited{};
        for(auto& b : sk) {
            if(visited.find(&b.second) != visited.end()) continue;
            BoneApplyFuncTopDown<T>(f, b.second, start_val, visited);
        }
    }

}

#endif //LIFE_ENGINE_BONE_HPP
