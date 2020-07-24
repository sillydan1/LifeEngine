#ifndef LIFE_ENGINE_LIFESELECTABLE_HPP
#define LIFE_ENGINE_LIFESELECTABLE_HPP

#include <util/Random.hpp>
#include "kuma_math_types.hpp"

namespace life::kuma {

    struct LifeSelectable {
        TYPE_PTR(glm::vec3) selectorcolor;
        LifeSelectable()
         : selectorcolor{std::make_shared<glm::vec3>(Random::GenerateVec3(0,1))}
        {}
    };

}

#endif //LIFE_ENGINE_LIFESELECTABLE_HPP
