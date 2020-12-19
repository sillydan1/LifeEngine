#include <imgui/imgui.h>
#include <life_core.h>
#include <GL/glew.h>
#include <events/ApplicationEvents.hpp>
#include <applications/Application.hpp>
#include <kuma/utils.h>
#include <layers/staging/ComponentStaging.hpp>
#include <components/TransformComponent.hpp>
#include <components/MaterialComponent.hpp>
#include <layers/PrimitiveConstructor.hpp>
#include <systems/rendering/GizmosRenderSystem.hpp>
#include "EditorMenus.hpp"
#include "KumaTypeEditors.hpp"

namespace life::editor {
	
	void EditorMenus::DrawMenuBar(const std::shared_ptr<ecs::ECS>& world_ref) {
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File")) {
				if(ImGui::BeginMenu("New")) {
					if(ImGui::MenuItem("Scene")) {
						LIFE_ERR("ECS World serialization has not started development yet");
					}
					if(ImGui::MenuItem("Entity Asset")) {
						LIFE_ERR("Entity prefabrication has not started development yet");
					}
					if(ImGui::MenuItem("Animation")) {
						LIFE_ERR("Animation editor has not started development");
					}
					if(ImGui::MenuItem("System")) {
						LIFE_ERR("System editor as not started development");
					}
					if(ImGui::MenuItem("Script")) {
						LIFE_ERR("No scripting system attached. You can use C++ scripting instead");
					}
					ImGui::EndMenu();
				}
				if(ImGui::MenuItem("Open")) {
				
				}
				if(ImGui::MenuItem("Save")) {
				
				}
				if(ImGui::MenuItem("Save As...")) {
				
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit")) {
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Project")) {
				if(ImGui::MenuItem("New Project")) {
				
				}
				if(ImGui::MenuItem("Open Project")) {
				
				}
				ImGui::EndMenu();
			}
			if(ImGui::BeginMenu("Scene")) {
				if(ImGui::BeginMenu("Add")) {
					ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_TextDisabled), "Geometry");
					if(ImGui::BeginMenu("Primitives")) {
						if(ImGui::MenuItem("Cube")) {
							PrimitiveConstructor::CreateCube(*world_ref);
						}
						if(ImGui::MenuItem("Sphere")) {
							LIFE_ERR("Adding this primitive does not work yet!");
						}
						if(ImGui::MenuItem("Quad")) {
							PrimitiveConstructor::CreateQuad(*world_ref);
						}
						ImGui::EndMenu();
					}
					if(ImGui::MenuItem("Static Model")) {
                        PrimitiveConstructor::CreateStaticModel(*world_ref);
					}
                    if(ImGui::MenuItem("Animated Model")) {
                        PrimitiveConstructor::CreateAnimatedModel(*world_ref);
                    }
					
					// ------
					ImGui::Separator();
					ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_TextDisabled), "Physics");
					if(ImGui::MenuItem("Wind Field")) {
						LIFE_ERR("Adding this primitive does not work yet!");
					}
					
					// ------
					ImGui::Separator();
					ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_Header), "Card Game");
					if(ImGui::MenuItem("Basic Card")) {
                        PrimitiveConstructor::CreateGameCard(*world_ref);
					}
					
					if(ImGui::MenuItem("Particle System")) {
                        PrimitiveConstructor::CreateParticleSystem(*world_ref);
					}
					
					ImGui::EndMenu();
				}
				if(ImGui::MenuItem("<TBD>")) {
				
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Window")) {
				
				ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_TextDisabled), "Editor Windows");
				
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help")) {
				ImGui::EndMenu();
			}
			HelpMarker(__life_ver__);
			
			ImGui::EndMenuBar();
		}
	}
	
	void EditorMenus::HelpMarker(const char *desc) {
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

	void EditorMenus::DrawEntityInspector(LayerStack* owner) {
		ImGui::Begin("Entity Inspector");
		auto obj = owner->GetLayerResource("LIFE_SELECTED_OBJ");
		if(obj.second) {
			// Enabled/disabled
			static bool yes = true; // TODO: Have a disable/enable system
			ImGui::Checkbox("Enabled", &yes);
			// Entity ID
            auto collection = static_cast<ecs::entity_ptr*>(obj.second);
            std::vector<char> entity_name;
            std::for_each(collection->m_name.begin(),collection->m_name.end(), [&entity_name](char a){entity_name.push_back(a);});
            // TODO: I really need a ImGui::InputText(std::string& s) overload...
            ImGui::InputText("", entity_name.data(), entity_name.size(),
                             ImGuiInputTextFlags_CallbackCharFilter,
                             [](ImGuiTextEditCallbackData* data)
                             {
                                 data->EventChar = 'A';
                                 return 0;
                             });
			ImGui::Text("<Entity Name>: '%s' - Editing this is fucking impossible and ImGui sucks", collection->m_name.c_str());
			ImGui::Separator();
            // This is basically just blind casting here...

			if(!collection->empty()) {
                for(auto& c : *collection) {
                    TemporaryDebuggingEditor::ShowComponent(c);
                }
            } else {
			    ImGui::Text("Entity is empty somehow...");
			}
		} else {
			ImGui::TextColored(ImGui::GetStyleColorVec4(ImGuiCol_::ImGuiCol_TextDisabled), "Nothing is selected");
		}
		ImGui::End();
	}

	void EditorMenus::DrawViewport(const LayerStack *owner, float &width, float &height, const std::string& buffer) {
        GLuint colorattachment = *(GLuint*)owner->GetLayerResource(buffer).second;
        GLCall(glBindTexture(GL_TEXTURE_2D, colorattachment));
        GLCall(glViewport(0, 0, width, height));
        auto window_size = ImGui::GetWindowSize();
        window_size.x -= 5;
        window_size.y -= 45;
        ImGui::Image((void*)(intptr_t)colorattachment, window_size, {0,1},{1,0});
        auto nx = ImGui::GetWindowSize().x;
        auto ny = ImGui::GetWindowSize().y;

        // Dispatch a RenderResizeEvent if the scene viewer changed.
        if(width != nx || height != ny) {
            // Dispatch a window resize event
            RenderResizeEvent re{nx, ny, buffer.c_str()};
            Application::Get().OnEvent(re);
            width  = nx;
            height = ny;
        }
	}
	static glm::vec4 window_dimensions;
	void EditorMenus::DrawSceneViewer(LayerStack *owner, float &width, float &height) {
		ImGui::Begin("Scene Viewer");
        DrawViewport(owner, width, height, "GL_FBO_COLORATTACHMENT");
        auto window_pos = ImGui::GetWindowPos();
        auto window_size= ImGui::GetWindowSize();
        window_size.x -= 5;
        window_size.y -= 45;
        window_dimensions = {window_pos.x, window_pos.y, window_size.x, window_size.y};
        static bool did = true;
        if(did) {
			owner->StoreLayerResource("SELECTION_WINDOW_SIZE_AND_LOCATION", sizeof(glm::vec4), &window_dimensions);
			did = false;
		}
        ImGui::End();
	}

    void EditorMenus::DrawGameViewer(const LayerStack* owner, float& width, float& height) {
        ImGui::Begin("Game Viewer");
        DrawViewport(owner, width, height, "GAME_GL_FBO_COLORATTACHMENT");
        ImGui::End();
    }

    bool toggle_wireframe = false;
	void EditorMenus::DrawWireframeOption(const LayerStack *owner) {
        void* draw_wireframe = owner->GetLayerResource("DRAW_WIREFRAME").second;
        if(draw_wireframe) {
            ImGui::Checkbox("Draw Wireframe", (bool*) draw_wireframe);

            if (!(toggle_wireframe && *(bool*) draw_wireframe)) {
                if (*(bool*) draw_wireframe) {
                    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
                } else {
                    GLCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
                }
                toggle_wireframe = *(bool*) draw_wireframe;
            }
        }
	}

    void EditorMenus::DrawAssetSelector() {
        ImGui::Begin("Asset Selector");
        if(ImGui::Button("Load Asset(s) <Images only so far>")) {
            auto strs = PrimitiveConstructor::FileDialogueMultiple(FileType::Image);
            for(auto& s : strs) {
                kuma::Texture::FromFilePath(s);
            }
        }
        ImGui::Separator();
        // Textures first
        auto tex_pool = kuma::Texture::GetTexturePool();
        int n = 0;
        int ww = (int)ImGui::GetWindowWidth();
        for(auto& tex : tex_pool) {
            if ((n % (ww <= 110 ? 1 : ww / 110)) != 0)
                ImGui::SameLine();
            n++;
            ImGui::Image(reinterpret_cast<ImTextureID>(tex.second->GetRendererID()), ImVec2(100,100));
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Texture: '%s'", tex.first.c_str());
            if(ImGui::BeginDragDropSource(ImGuiDragDropFlags_None | ImGuiDragDropFlags_SourceAllowNullID)) {
                ImGui::SetDragDropPayload("LIFE_TEXTURE_CELL", &tex.second, sizeof(kuma::Texture));
                ImGui::Text("Apply '%s'", tex.first.c_str());
                ImGui::Image(reinterpret_cast<ImTextureID>(tex.second->GetRendererID()), ImVec2(25,25));
                ImGui::EndDragDropSource();
            }
        }

        ImGui::End();
    }

}
