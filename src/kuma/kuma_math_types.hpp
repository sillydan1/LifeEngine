/* math types *//*
enum class KumaTypeEnum {

texture,
cubemap,
transform,
custom,
dirlight,
spotlight,
pointlight,
};*/
#ifndef KUMA_MAP_TYPES_H
#define KUMA_MAP_TYPES_H
#include <glm/glm.hpp>
#include <type_traits>
#include <variant>
#include <tuple>
#include <memory>

template<typename T> 
struct is_glm_matrix : std::disjunction< 
							std::is_base_of<glm::mat2x2, T>,
							std::is_base_of<glm::mat2x3, T>,
							std::is_base_of<glm::mat2x4, T>,
							std::is_base_of<glm::mat3x2, T>,
							std::is_base_of<glm::mat3x3, T>,
							std::is_base_of<glm::mat3x4, T>,
							std::is_base_of<glm::mat4x2, T>,
							std::is_base_of<glm::mat4x3, T>,
							std::is_base_of<glm::mat4x4, T>
							>{};
template<typename T> struct is_glm_vector : std::disjunction<
							std::is_base_of<glm::vec1, T>,
							std::is_base_of<glm::vec2, T>,
							std::is_base_of<glm::vec3, T>,
							std::is_base_of<glm::vec4, T>
							>{};
template<typename T> constexpr auto is_glm_vector_v = is_glm_vector<T>::value;
template<typename T> constexpr auto is_glm_matrix_v = is_glm_matrix<T>::value;

using u_int = unsigned int;

#define TYPE_PTR(...) std::shared_ptr<__VA_ARGS__>
#include "Texture.hpp"
#include "Transform.hpp"
#include "CubeTexture.hpp"
#include "animation/Bone.hpp"

using KumaTypes = std::variant< // Life/Kuma specifics
                                TYPE_PTR(life::kuma::Transform),
                                TYPE_PTR(life::kuma::Texture),
								TYPE_PTR(life::kuma::CubeTexture),

								TYPE_PTR(life::kuma::Skeleton),
								// OpenGL things
								TYPE_PTR(glm::bvec2),
								TYPE_PTR(glm::bvec4),
								TYPE_PTR(glm::bvec3),
								TYPE_PTR(glm::dmat2),
								TYPE_PTR(glm::dmat2x3),
								TYPE_PTR(glm::dmat2x4),
								TYPE_PTR(glm::dmat3),
								TYPE_PTR(glm::dmat3x2),
								TYPE_PTR(glm::dmat3x4),
								TYPE_PTR(glm::dmat4),
								TYPE_PTR(glm::dmat4x2),
								TYPE_PTR(glm::dmat4x3),
								TYPE_PTR(glm::dvec2),
								TYPE_PTR(glm::dvec4),
								TYPE_PTR(glm::dvec3),
								TYPE_PTR(glm::ivec2),
								TYPE_PTR(glm::ivec3),
								TYPE_PTR(glm::ivec4),
								TYPE_PTR(glm::mat2),
								TYPE_PTR(glm::mat2x3),
								TYPE_PTR(glm::mat2x4),
								TYPE_PTR(glm::mat3),
								TYPE_PTR(glm::mat3x2),
								TYPE_PTR(glm::mat3x4),
								TYPE_PTR(glm::mat4),
								TYPE_PTR(glm::mat4x2),
								TYPE_PTR(glm::mat4x3),
								TYPE_PTR(glm::uvec2),
								TYPE_PTR(glm::uvec3),
								TYPE_PTR(glm::uvec4),
								TYPE_PTR(glm::vec2),
								TYPE_PTR(glm::vec3),
								TYPE_PTR(glm::vec4),
								TYPE_PTR(std::tuple<float>),
								TYPE_PTR(std::tuple<float,float>),
								TYPE_PTR(std::tuple<float,float,float>),
								TYPE_PTR(std::tuple<float,float,float,float>),
								TYPE_PTR(bool),
								TYPE_PTR(std::tuple<int>),
								TYPE_PTR(std::tuple<int, int>),
								TYPE_PTR(std::tuple<int, int, int>),
								TYPE_PTR(std::tuple<int, int, int, int>),
								TYPE_PTR(std::tuple<u_int>),
								TYPE_PTR(std::tuple<u_int, u_int>),
								TYPE_PTR(std::tuple<u_int, u_int, u_int>),
								TYPE_PTR(std::tuple<u_int, u_int, u_int, u_int>),
								// Immutable data
								float,
								bool,
								int,
								u_int
							  >;

bool is_kuma_type_texture(const KumaTypes& kt);

#endif // KUMA_MAP_TYPES_H
