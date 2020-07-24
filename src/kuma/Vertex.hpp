#ifndef VERTEX_HPP
#define VERTEX_HPP
#include "openglmath"
#include "VertexBufferLayout.hpp"
#include <unordered_map>

namespace life::kuma {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		
		static VertexBufferLayout GetLayout() {
			VertexBufferLayout layout;
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);
			return layout;
		}
	};

	template<unsigned int MaxJointAmount>
	struct AnimatedVertex {
        glm::vec3 Position; /// Note: We cannot inherit from Vertex, since the v-table will eff up everything
        glm::vec3 Normal;
        glm::vec2 TexCoords;
	    static const unsigned int JointAmount = MaxJointAmount;
        unsigned int IDs[MaxJointAmount]; // Bone IDs
        float Weights   [MaxJointAmount]; // Weight to ↑ that bone

        AnimatedVertex() : Position{0}, Normal{0}, TexCoords{0}, IDs{}, Weights{}
        {
        // TODO: -Wno-memset-elt-size
#pragma GCC diagnostic ignored "-Wmemset-elt-size"
#pragma GCC diagnostic push
            memset(IDs, 0, MaxJointAmount);
#pragma GCC diagnostic pop
            for (int i = 0; i < MaxJointAmount; ++i) {
                Weights[i] = 0.0f;
            }
        }

        static VertexBufferLayout GetLayout() {
			VertexBufferLayout layout = Vertex::GetLayout();
			layout.Push<unsigned int>(MaxJointAmount);
			layout.Push<float>       (MaxJointAmount);
			return layout;
		}
        void AddBoneData(unsigned int BoneID, float Weight) {
            for (unsigned int i = 0; i < MaxJointAmount; ++i) {
                if(Weights[i] == 0.0f) {
                    IDs[i]      = BoneID;
                    Weights[i]  = Weight;
                    return;
                }
            }
            LIFE_WARN("Vertex: Bone amount exceeded supported amount!");
        }
	};
	using BaseAnimationVertex       = AnimatedVertex<4>;
	using HighresAnimationVertex    = AnimatedVertex<8>;

}

#endif //CLASSNAME_HPP
