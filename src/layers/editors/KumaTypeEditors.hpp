#ifndef LIFE_ENGINE_KUMATYPEEDITORS_HPP
#define LIFE_ENGINE_KUMATYPEEDITORS_HPP
#include <kuma/kuma>
#include <portable-file-dialogs/portable-file-dialogs.h>
#include <systems/rendering/RenderSystem.hpp>
#include <imgui/imgui.h>
#include <util/overload>

#include <layers/staging/ComponentStaging.hpp>
#include <components/components>
#include <layers/PrimitiveConstructor.hpp>

namespace life::editor {

    struct TransformEditor {
        static float drag_speed;
        static bool edit_drag_speed;

        static void Draw(const TYPE_PTR(kuma::Transform)& transform) {
            static bool edit_quaternion_directly = false;
            ImGui::Checkbox("Toggle dragspeed edit", &edit_drag_speed);
            ImGui::Checkbox("Toggle quaternion edit",&edit_quaternion_directly);
            if(edit_drag_speed)
                ImGui::DragFloat("Drag Speed", &drag_speed);
            ImGui::DragFloat3("Position",      &transform->position[0], drag_speed);
            if(edit_quaternion_directly)
                ImGui::DragFloat4("Orientation",   &transform->orientation[0], drag_speed);
            glm::vec3 rotation = glm::eulerAngles(transform->orientation);
            ImGui::DragFloat3("Orientation", &rotation[0], drag_speed);
            transform->orientation = glm::quat(rotation);
            ImGui::DragFloat3("Scale",         &transform->scale[0], drag_speed);
            transform->UpdateModelMatrix();
        }
    };

    struct TextureEditor {
        static void Draw(TYPE_PTR(kuma::Texture)& texture, TYPE_PTR(kuma::Material)& material, bool* enabled, const glm::vec2& size = {150,150}) {
            if (ImGui::ImageButton(reinterpret_cast<ImTextureID>(texture->GetRendererID()),
                                   ImVec2(100, 100))) {
                auto f = PrimitiveConstructor::FileDialogueSingle(FileType::Image);
                texture = kuma::Texture::FromFilePath(f);
                material->SetProperty("u_material.diffuse", texture); // update the property
                LIFE_DBG("Selected File: %s", f.c_str());
            }
            if(ImGui::BeginDragDropTarget()) {
                if(const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("LIFE_TEXTURE_CELL")) {
                    LIFE_ASSERT(payload->DataSize == sizeof(kuma::Texture));
                    std::shared_ptr<kuma::Texture> pl = *static_cast<std::shared_ptr<kuma::Texture>*>(payload->Data);
                    material->SetProperty("u_material.diffuse", pl); // update the property
                }
                ImGui::EndDragDropTarget();
            }
        }
    };

    struct MaterialEditor {
        static void Draw(TYPE_PTR(kuma::Material)& material) {
            static bool yes = true;
            ImGui::Text("Material with shader: %s", material->GetShader().GetFilePath());
            for(auto& t : material->GetProperties()) {
                std::visit(
                        overload(
                                [&material](TYPE_PTR(kuma::Texture)& v)    { TextureEditor::Draw(v, material, &yes); },
                                /// Empty cases
                                [](TYPE_PTR(kuma::Transform)&) {},
                                [](auto&& v) {
                                    ImGui::Text("Material Property not supported yet"); // indented, so you can break here
                                }
                        ), t.second);
            }
        }
    };
    
    struct GameCardEditor {
    	static void Draw(GameCard& card);
    };
    
    struct LifeCameraEditor {
    	static void Draw(LifeCamera& c);
    };

    struct SkeletonEditor {
        static void Draw(TYPE_PTR(kuma::Skeleton) skeleton) {
            for(auto& bone : *skeleton) {
                ImGui::PushID(bone.second.BoneName.c_str());
                bool node_open = ImGui::TreeNode("Something", "Bone: %s", bone.second.BoneName.c_str());
                if(node_open) {
                    ImGui::DragFloat4("a", &bone.second.FinalTransform[0][0], 0.0001);
                    ImGui::DragFloat4("b", &bone.second.FinalTransform[1][0], 0.0001);
                    ImGui::DragFloat4("c", &bone.second.FinalTransform[2][0], 0.0001);
                    ImGui::DragFloat4("d", &bone.second.FinalTransform[3][0], 0.0001);
                    ImGui::TreePop();
                }
                ImGui::PopID();
            }
        }
    };

    struct MeshEditor {
        static void Draw(kuma::StaticMesh& mesh) {
            ImGui::Text("<<Mesh>>"); // TODO: Make a mini-display port of the mesh
            MaterialEditor::Draw(mesh.material);
        }
        static void DrawA(kuma::AnimatedMesh& mesh) {
            ImGui::Text("<<Animated Mesh>>"); // TODO: Make a mini-display port of the mesh
            MaterialEditor::Draw(mesh.material);
        }
    };
	
	struct RigidBodyEditor {
		static btTransform bttransform_from_kuma(const kuma::Transform& b) {
			btTransform a{};
			a.setIdentity();
			auto p = b.position; p*=2;
			auto o = b.orientation;
			a.setOrigin({p.x, p.y, p.z});
			a.setRotation({o.x,o.y,o.z,o.w});
			return a;
		}
		static void Draw(BulletRigidBodyComponent& r) {
			ImGui::Text("BulletPhysics RigidBody");
			TransformEditor::Draw(r.transform);
			r.rigibody->setWorldTransform(bttransform_from_kuma(*r.transform));
			bool isa = r.rigibody->isActive();
			ImGui::Checkbox("Is Active", &isa);
			auto as = r.rigibody->getActivationState();
			ImGui::InputInt("Activation State", &as);
			if(ImGui::Button("Activate")) {
				r.rigibody->activate(true);
			}
		}
	};

    struct ModelEditor {
        static void Draw(TYPE_PTR(kuma::Model)& model) {
            // Draw a MeshEditor for each mesh
            for(auto& m : model->GetMeshes()) {
                MeshEditor::Draw(m);
                ImGui::Separator();
            }
        }
        static void DrawAnimated(TYPE_PTR(kuma::AnimatedModel)& model) {
            // Draw a MeshEditor for each mesh
            bool (* getter)(void*, int, const char**) = [](void* animations, int idx, const char** out) {
                *out = static_cast<kuma::Animation*>(animations)[idx].Name.c_str();
                return true;
            };
            ImGui::Combo("Animation", &model->m_selected_animation, getter, model->m_animations.data(), model->m_animations.size());
            if(ImGui::Button("+")) {
                auto fs = PrimitiveConstructor::FileDialogueMultiple(FileType::Model);
                for (const auto &name : fs) {
                    // material->SetProperty("u_material.diffuse", kuma::Texture::FromFilePath(name));
                    model->AddAnimation(name);
                    LIFE_DBG("Selected File: %s", name.c_str());
                }
            }
            ImGui::SliderFloat("Animation Timepoint", &model->dbg_time_point, 0.0f, model->m_animations[model->m_selected_animation].Duration);
            for(auto& m : model->GetMeshes()) {
                MeshEditor::DrawA(m);
            }
        }
    };

    struct TemporaryDebuggingEditor {
        static void DrawWorld(ecs::ECS& world, bool* enabled, LifeCamera* game_camera) {
            if(ImGui::Begin("Component Selector", enabled)) {
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
                ImGui::Columns(2);
                ImGui::Separator();
                if(game_camera) {
                	LifeCameraEditor::Draw(*game_camera);
                }
                for(auto& c : world.GetAllComponents()) {
					ShowComponent(c.second);
                }
                ImGui::Columns(1);
                ImGui::Separator();
                ImGui::PopStyleVar();
            }
            ImGui::End();
        }
        static void ShowComponent(std::shared_ptr<ecs::BaseComponent>& component) {
            ImGui::PushID(&component);
            ImGui::AlignTextToFramePadding();
			auto id = component->GetID();
			std::string cname = component->GetComponentName();
			bool node_open = ImGui::TreeNode("Entity Object", "%s", cname.c_str());
            ImGui::NextColumn();
            ImGui::AlignTextToFramePadding();
            // ...
            if(node_open) {
                if(id == TransformComponent::ID) {
                    auto wc = std::dynamic_pointer_cast<TransformComponent>(component);
                    TransformEditor::Draw(wc->transform);
                } else if(id == ModelComponent::ID) {
                    auto wc = std::dynamic_pointer_cast<ModelComponent>(component);
                    ModelEditor::DrawAnimated(wc->model);
                } else if(id == StaticModelComponent::ID) {
                    auto wc = std::dynamic_pointer_cast<StaticModelComponent>(component);
                    ModelEditor::Draw(wc->model);
                } else if(id== StaticMeshComponent::ID) {
                    auto wc = std::dynamic_pointer_cast<StaticMeshComponent>(component);
                    MeshEditor::Draw(*wc->mesh);
                } else if(id== BulletRigidBodyComponent::ID) {
					auto wc = std::dynamic_pointer_cast<BulletRigidBodyComponent>(component);
					RigidBodyEditor::Draw(*wc);
				} else if(id== GameCard::ID) {
                	auto gc = std::dynamic_pointer_cast<GameCard>(component);
                	GameCardEditor::Draw(*gc);
                } else
                    ImGui::Text("<WIP>");

                ImGui::TreePop();
            }
            ImGui::NextColumn();
            ImGui::PopID();
        }
    };
	
	struct EntityGUISelector {
		static void DrawWorld(ecs::ECS& world, LayerStack* owner, bool* enabled);
	};

}

#endif //LIFE_ENGINE_KUMATYPEEDITORS_HPP
