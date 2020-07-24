#ifndef LIFE_ENGINE_ASSIMPLOADER_HPP
#define LIFE_ENGINE_ASSIMPLOADER_HPP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector>
#include <kuma/animation/Animation.hpp>
#include "Mesh.hpp"
#include "animation/Bone.hpp"
#include "default_shapes.hpp"
#include <algorithm>
#include <util/Tree.hpp>

namespace life::kuma {

    inline glm::mat4 from_assimp_matrix4x4(const aiMatrix4x4& m) {
        /*return {{m[0][0],m[1][0],m[2][0],m[3][0]},
                {m[0][1],m[1][1],m[2][1],m[3][1]},
                {m[0][2],m[1][2],m[2][2],m[3][2]},
                {m[0][3],m[1][3],m[2][3],m[3][3]}};*/
        return {{m.a1,m.b1,m.c1,m.d1},
                {m.a2,m.b2,m.c2,m.d2},
                {m.a3,m.b3,m.c3,m.d3},
                {m.a4,m.b4,m.c4,m.d4}};
    }

    inline glm::vec3 from_assimp_vec3(const aiVector3D& v) {
        return {v.x,v.y,v.z};
    }

    inline glm::vec2 from_assimp_vec2(const aiVector2D& v) {
        return {v.x,v.y};
    }

    inline glm::quat from_assimp_quat(const aiQuaternion& q) {
        return {q.w,q.x,q.y,q.z};
    }

    template<typename MeshType>
    struct AssimpLoader {
        using vert_t = typename MeshType::VertexT;
        glm::mat4 m_ginverse;
        Skeleton* bones;
        const aiScene* m_sceneref;
        int bone_counter;

        AssimpLoader() : m_ginverse{0}, bones{nullptr} {}
        explicit AssimpLoader(Skeleton* bones) : m_ginverse{0}, bones{bones}, bone_counter{0} { }
        void ProcessScene(const aiScene* scene, std::vector<MeshType>& meshes) {
            m_sceneref = scene;
            ProcessNode(scene->mRootNode, scene, meshes);
            if(bones) {
                LoadBoneHierarchy(scene->mRootNode);
                BoneTransformDefaultPose(scene->mRootNode, scene, default_shapes::mat4_identity_matrix);
            }
            // DBG_Count_root_bones();
        }
        void DBG_Count_root_bones() {
            unsigned int nmbr = 0;
            std::map<Bone*, int> b_ptrs{};
            for (auto& b : *bones) {
                Bone* k = &b.second;
                lpp:
                if(k->parent) {
                    k = k->parent;
                    goto lpp;
                }
                if(b_ptrs.find(k) == b_ptrs.end()) {
                    nmbr++;
                    b_ptrs.insert({k,0});
                }
            }
            for(auto& b : b_ptrs) {
                LIFE_DBG("%d : '%s'", nmbr, b.first->BoneName.c_str());
            }
        }
        void ProcessAnimations(const aiScene* scene, std::vector<Animation>& animation_collection) {
            for (unsigned int i = 0; i < scene->mNumAnimations; ++i) {
                Animation anim{};
                anim.Name = scene->mAnimations[i]->mName.C_Str();
                anim.Duration = scene->mAnimations[i]->mDuration;
                anim.Speed = scene->mAnimations[i]->mTicksPerSecond;
                anim.AnimationChannels.reserve(scene->mAnimations[i]->mNumChannels);
                for (unsigned int j = 0; j < scene->mAnimations[i]->mNumChannels; ++j) {
                    auto chnls = scene->mAnimations[i]->mChannels[j];
                    time_map<glm::vec3> pkeys;
                    time_map<glm::quat> rkeys;
                    time_map<glm::vec3> skeys;
                    auto vec3_conv = [](const aiVectorKey& v) -> std::pair<double,glm::vec3> {return {v.mTime, from_assimp_vec3(v.mValue)}; };
                    auto quat_conv = [](const aiQuatKey&   v) -> std::pair<double,glm::quat> {return {v.mTime, from_assimp_quat(v.mValue)}; };
                    for (unsigned int l = 0; l < chnls->mNumPositionKeys ; ++l) pkeys.insert(vec3_conv(chnls->mPositionKeys[l]));
                    for (unsigned int l = 0; l < chnls->mNumRotationKeys ; ++l) rkeys.insert(quat_conv(chnls->mRotationKeys[l]));
                    for (unsigned int l = 0; l < chnls->mNumScalingKeys  ; ++l) skeys.insert(vec3_conv(chnls->mScalingKeys [l]));
                    anim.AddAnimationNode(
                        scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str(),
                        { std::move(pkeys),std::move(rkeys),std::move(skeys) }
                    );
                }
                animation_collection.emplace_back(std::move(anim));
            }
        }
        void ProcessNode(aiNode* node, const aiScene* scene, std::vector<MeshType>& meshes)  {
            auto m_GlobalInverseTransform = scene->mRootNode->mTransformation;
            m_GlobalInverseTransform.Inverse();
            m_ginverse = from_assimp_matrix4x4(m_GlobalInverseTransform);

            // process all the node's meshes (if any)
            for(unsigned int i = 0; i < node->mNumMeshes; i++) {
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                auto vertices = ProcessVertices(mesh);
                auto indices  = ProcessIndices(mesh);
                auto mat  	  = ProcessMaterial(mesh, scene);
                meshes.emplace_back(vertices, indices, mat);
            }
            // then do the same for each of its children
            for(unsigned int i = 0; i < node->mNumChildren; i++) {
                ProcessNode(node->mChildren[i], scene, meshes);
            }
        }
        std::vector<unsigned int> ProcessIndices(aiMesh* assimpMesh) const {
            std::vector<unsigned int> indices;
            /* INDICES */
            for (unsigned int i = 0; i < assimpMesh->mNumFaces; i++) {
                aiFace face = assimpMesh->mFaces[i];
                // get all indices of the face and store them in the list
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }
            return indices;
        }
        std::vector<vert_t> ProcessVertices(aiMesh* assimpMesh) {
            std::vector<vert_t> vertices;
            /* VERTICES */
            for (unsigned int i = 0; i < assimpMesh->mNumVertices; i++) {
                vert_t vertexToAdd;
                /* POSITION */
                vertexToAdd.Position = from_assimp_vec3(assimpMesh->mVertices[i]);
                /* NORMALS */
                vertexToAdd.Normal = from_assimp_vec3(assimpMesh->mNormals[i]);
                /* TEXTURE COORDS */
                if(assimpMesh->mTextureCoords[0]) {
                    glm::vec2 uv;
                    uv.x = assimpMesh->mTextureCoords[0][i].x; 						// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                    uv.y = assimpMesh->mTextureCoords[0][i].y; 						// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vertexToAdd.TexCoords = uv;
                } else
                    vertexToAdd.TexCoords = glm::vec2(0.0f);

                // TODO: Add support for tangents and other things that makes sense.

                vertices.push_back(vertexToAdd);
            }
            if constexpr (std::is_same_v<MeshType, AnimatedMesh>) {
                // Load the bones
                LoadBones(assimpMesh, vertices);
                
            }
            return vertices;
        }

        void LoadBoneHierarchy(aiNode* rootnode) {
            for (auto& b : *bones) {
                auto bn = rootnode->FindNode(b.second.BoneName.c_str());
                if (bn && bn->mParent) {
                    auto bp = bones->find(bn->mParent->mName.C_Str());
                    if (bp != (*bones).end())
                        b.second.parent = &bp->second;
                    else
                        LIFE_WARN("Could not find bone '%s' in the kuma::Skeleton data structure!",
                                  bn->mParent->mName.C_Str());
                }
            }
        }

        void BoneTransformDefaultPose(aiNode* node, const aiScene* scene, const glm::mat4& transform_offset) {
            if(bones) {
                glm::mat4 NodeTransform = from_assimp_matrix4x4(node->mTransformation);
                glm::mat4 gtrans = transform_offset * NodeTransform;
                auto b = bones->find(node->mName.C_Str());
                if(b != bones->end()) {
                    (*b).second.FinalTransform = m_ginverse * gtrans * b->second.Offset;
                    (*b).second.BaseTransform = NodeTransform;
                }
                for (unsigned int i = 0; i < node->mNumChildren; ++i) {
                    BoneTransformDefaultPose(node->mChildren[i], scene, gtrans);
                }
            }
        }

        void LoadBones(aiMesh* assimpMesh, std::vector<vert_t>& vertices) {
            if(bones) {
                for (unsigned int i = 0; i < assimpMesh->mNumBones; ++i) {
                    auto b = bones->find(assimpMesh->mBones[i]->mName.C_Str());
                    if (b == bones->end()) {
                        Bone bone{bone_counter++, assimpMesh->mBones[i]->mName.C_Str()};
                        bone.Offset = from_assimp_matrix4x4(assimpMesh->mBones[i]->mOffsetMatrix);
                        bones->insert({bone.BoneName, bone});
                        b = bones->find(bone.BoneName);
                    }
                    for (unsigned int w = 0; w < assimpMesh->mBones[i]->mNumWeights; ++w) {
                        unsigned int VertexID = assimpMesh->mBones[i]->mWeights[w].mVertexId;
                        float Weight = assimpMesh->mBones[i]->mWeights[w].mWeight;
                        vertices[VertexID].AddBoneData(b->second.bone_index, Weight);
                    }
                }
            }
        }

        std::shared_ptr<Material> ProcessMaterial(aiMesh* assimpMesh, const aiScene* scene) const {
            //Material mat = ; // TODO: Implement better material loading.
            std::string shader_to_load = SHADERDIR;
            if constexpr(std::is_same_v<MeshType, AnimatedMesh>) shader_to_load += "/default_animated.glsl";
            else                                                 shader_to_load += "/default.glsl";
            auto mat = Material::Create(shader_to_load);
            /* MATERIAL */
            // aiMaterial* aimat = scene->mMaterials[assimpMesh->mMaterialIndex];
            // TODO: Implement aiMaterial -> kuma material support.
            if(assimpMesh->mMaterialIndex >= 0) {
                // This looping only works because aiTextureType starts at NONE (0x0) and ends at UNKNOWN (0xC)
                for (int i = aiTextureType_NONE; i <= aiTextureType_UNKNOWN; i++) {
                    aiTextureType texType = static_cast<aiTextureType>(i);
                    aiString str;
                    aiMaterial* assimpMaterial = scene->mMaterials[assimpMesh->mMaterialIndex];
                    for (unsigned int j = 0; j < assimpMaterial->GetTextureCount(texType); j++) {
                        assimpMaterial->GetTexture(texType, j, &str);
                        if(str.length > 0) {
                            std::string file = TEXTUREDIR "/";
                            mat->SetProperty("u_material." + GetShaderMaterialUniformName(texType),
                                             Texture::FromFilePath(file + str.C_Str()));
                        }
                    }
                }
            }
            return mat;
        }

        [[nodiscard]] std::string GetShaderMaterialUniformName(const aiTextureType& type) const {
            switch(type) {
                case aiTextureType_NONE: 		return "notsupported";	// 0x0,
                case aiTextureType_DIFFUSE: 	return "diffuse";		// 0x1,
                case aiTextureType_SPECULAR: 	return "specular";		// 0x2,
                case aiTextureType_AMBIENT: 	return "ambient_occlusion";	// 0x3,
                case aiTextureType_EMISSIVE: 	return "emissive";		// 0x4,
                case aiTextureType_HEIGHT: 		return "height";		// 0x5,
                case aiTextureType_NORMALS: 	return "normal";		// 0x6,
                case aiTextureType_SHININESS: 	return "shininess";		// 0x7,
                case aiTextureType_OPACITY: 	return "opacity";		// 0x8,
                case aiTextureType_DISPLACEMENT:return "displacement";	// 0x9,
                case aiTextureType_LIGHTMAP: 	return "lightmap";		// 0xA,
                case aiTextureType_REFLECTION: 	return "reflection";	// 0xB,
                case aiTextureType_UNKNOWN: 	return "notsupported";	// 0xC,
                default:						return "notsupported";	// 0x?,
            }
        }

    };
}
#endif //LIFE_ENGINE_ASSIMPLOADER_HPP
