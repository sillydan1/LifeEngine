#include "kuma_math_types.hpp"

bool is_kuma_type_texture(const KumaTypes& kt) {
    auto texture = std::get_if<TYPE_PTR(life::kuma::Texture)>(&kt);
    auto cubemap = std::get_if<TYPE_PTR(life::kuma::CubeTexture)>(&kt);
    return (texture || cubemap);
}
