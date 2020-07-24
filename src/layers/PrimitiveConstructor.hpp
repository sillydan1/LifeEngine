#ifndef LIFE_ENGINE_PRIMITIVECONSTRUCTOR_HPP
#define LIFE_ENGINE_PRIMITIVECONSTRUCTOR_HPP

#include <ecs/ecs>
#include <map>
#include <util/FileSystem.hpp>

namespace life {

    enum class FileType : unsigned int {
        Model,
        Animation,
        Image,
        // TODO Add support for more filetypes.
        Any // It is important to asserts that Any is lowest...
    };

    class PrimitiveConstructor {
    public:
        PrimitiveConstructor() = delete;
        ~PrimitiveConstructor() = delete;

        static std::string FileDialogueSingle(const FileType& type, const std::string& base_dir = getrespath());
        static std::vector<std::string> FileDialogueMultiple(const FileType& type, const std::string& base_dir = getrespath());
        static bool CreateCube(ecs::ECS& world_reference);
        static bool CreateSphere(ecs::ECS& world_reference);
        static bool CreateQuad(ecs::ECS& world_reference);

        // More complex stuff
        static bool CreateGameCard(ecs::ECS& world_reference);
        static bool CreateStaticModel(ecs::ECS& world_reference);
        static bool CreateAnimatedModel(ecs::ECS& world_reference);
        static bool CreateParticleSystem(ecs::ECS& world_reference);
    };

}

#endif //LIFE_ENGINE_PRIMITIVECONSTRUCTOR_HPP
