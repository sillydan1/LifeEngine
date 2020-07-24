#include <layers/staging/ComponentStaging.hpp>
#include <components/TransformComponent.hpp>
#include <components/ModelComponent.hpp>
#include <portable-file-dialogs/portable-file-dialogs.h>
#include <components/StaticMeshComponent.hpp>
#include <components/MaterialComponent.hpp>
#include "PrimitiveConstructor.hpp"

namespace life {

    bool PrimitiveConstructor::CreateCube(life::ecs::ECS& world_reference) {
        MaterialComponent mat{};
        TransformComponent t{};
        auto m = StaticMeshComponent{mat.material,
                                     t.transform,
                                     kuma::StaticMesh::Create(
                                             kuma::default_shapes::box,
                                             kuma::default_shapes::box_indices,
                                             mat.material)
                                    };
        auto e = world_reference.makeEntity<TransformComponent, StaticMeshComponent>(std::move(t), std::move(m));
        return !e.IsEmpty();
    }

    bool PrimitiveConstructor::CreateSphere(ecs::ECS &world_reference) {
        return false;
    }

    bool PrimitiveConstructor::CreateQuad(life::ecs::ECS& world_reference) {
        MaterialComponent mat{};
        TransformComponent t{};
        auto m = StaticMeshComponent{mat.material,
                                     t.transform,
                                     kuma::StaticMesh::Create(
                                             kuma::default_shapes::quad,
                                             kuma::default_shapes::quad_indices,
                                             mat.material)
        };
        auto e = world_reference.makeEntity<TransformComponent, StaticMeshComponent>(std::move(t), std::move(m));
        return !e.IsEmpty();
    }

    bool PrimitiveConstructor::CreateGameCard(life::ecs::ECS& world_reference) {
        auto e = world_reference.makeEntity<GameCard>({});
        return !e.IsEmpty();
    }

    bool PrimitiveConstructor::CreateStaticModel(ecs::ECS& world_reference) {
        auto f = FileDialogueSingle(FileType::Model);
        LIFE_DBG("Loaded '%s'", f.c_str());
        TransformComponent t{};
        auto e = world_reference.makeEntity<TransformComponent, StaticModelComponent>(std::move(t),{f});
        auto tt = e.GetComponent<TransformComponent>();
        for(auto& m : e.GetComponent<StaticModelComponent>()->model->GetMeshes()) {
            m.GetMaterial().SetProperty("u_model", tt->transform);
        }
        return !e.IsEmpty();
    }

    bool PrimitiveConstructor::CreateAnimatedModel(ecs::ECS& world_reference) {
        auto f = FileDialogueSingle(FileType::Model);
        LIFE_DBG("Loaded '%s'", f.c_str());
        TransformComponent t{};
        ModelComponent model{f};
        if(model.model->m_meshes.empty()) { return false; }
        auto e = world_reference.makeEntity<TransformComponent, ModelComponent>(std::move(t),std::move(model));
        auto tt = e.GetComponent<TransformComponent>();
        for(auto& m : e.GetComponent<ModelComponent>()->model->GetMeshes()) {
            m.GetMaterial().SetProperty("u_model", tt->transform);
        }
        return !e.IsEmpty();
    }

    bool PrimitiveConstructor::CreateParticleSystem(ecs::ECS& world_reference) {
        TransformComponent t{};
        MaterialComponent m{getrespath()+"/shaders/particle_default.glsl"};
        ParticleEmitterComponent e{m.material, t.transform};
        auto ent = world_reference.makeEntity<TransformComponent, MaterialComponent, ParticleEmitterComponent>(std::move(t), std::move(m), std::move(e));
        return !ent.IsEmpty();
    }

    // -------------------------------------------------------------------------------------------------------------- //
    static std::map<FileType, std::pair<const char*,const char*>> FileDialogueFileTypes = {
            {FileType::Model    , {"3D Model Files (*.fbx,*.obj,*.3ds,*.md5mesh,*.blend)","*.fbx *.obj *.3ds *.md5mesh *.blend"}},
            {FileType::Animation, {"Animation Files","*.fbx *.obj *.3ds *.md5mesh *.blend"}},
            {FileType::Image    , {"Image Files (.png .jpg)", "*.png *.jpg"}},
            // Add more here.
            {FileType::Any      , {"All Files", "*"}}
    };
    std::string PrimitiveConstructor::FileDialogueSingle(const FileType& ft, const std::string& base_dir) {
        // TODO: Make loading threaded
        LIFE_ASSERT(FileDialogueFileTypes.size() == (unsigned int)FileType::Any+1, "Not all FileType enumerations are supported in File Dialogues yet!");
        auto f = pfd::open_file("Choose File", base_dir,
                                { FileDialogueFileTypes[ft].first, FileDialogueFileTypes[ft].second },
                                false);
        if(!f.result().empty())
            return f.result()[0];
        return std::string();
    }
    std::vector<std::string> PrimitiveConstructor::FileDialogueMultiple(const FileType& ft, const std::string& base_dir) {
        // TODO: Make loading threaded
        LIFE_ASSERT(FileDialogueFileTypes.size() == (unsigned int)FileType::Any+1, "Not all FileType enumerations are supported in File Dialogues yet!");
        auto f = pfd::open_file("Choose File", base_dir,
                                { FileDialogueFileTypes[ft].first, FileDialogueFileTypes[ft].second },
                                true);
        if(!f.result().empty())
            return f.result();
        return {};
    }

}
