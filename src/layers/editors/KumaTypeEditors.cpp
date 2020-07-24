#include <layers/LayerStack.hpp>
#include "KumaTypeEditors.hpp"

namespace life::editor {
    float TransformEditor::drag_speed = 0.01f;
    bool TransformEditor::edit_drag_speed = false;

    
    void DrawStringEditor(const std::string& label, std::string& s) {
		char namebuffer[s.size()+1];
		std::strcpy(namebuffer, s.c_str());
		ImGui::InputText("Card name", namebuffer, s.size());
		if(strcmp(namebuffer, s.c_str()) != 0) {
			s = namebuffer;
		}
    }
    
	void GameCardEditor::Draw(life::GameCard& card) {
		// Name
		DrawStringEditor("Card name", card.cardname);
		// transform;
		TransformEditor::Draw(card.transform);
		// mesh;
		MeshEditor::Draw(*card.mesh);
		// card_texture;
		bool yes = true;
		TextureEditor::Draw(card.card_texture, card.mesh->material, &yes, {85,100});
	}
	
	void LifeCameraEditor::Draw(LifeCamera &c) {
		// uhh...
        ImGui::PushID(&c);
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Game Camera!");
        ImGui::NextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::NextColumn();
        ImGui::PopID();
	}

    void EntityGUISelector::DrawWorld(ecs::ECS& world, LayerStack* owner, bool* enabled) {
        if(ImGui::Begin("Entity Selector <WIP>", enabled)) {
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2,2));
            //ImGui::Columns(2);
            //ImGui::Separator();
            auto& ee = world.GetAllEntities();
            int i = 0;

            if(!owner->GetLayerResource("LIFE_SELECTED_INDEX").second) {
                owner->StoreLayerResource("LIFE_SELECTED_INDEX", sizeof(int), new int(-1));
            }
            int* selected_index = static_cast<int*>(owner->GetLayerResource("LIFE_SELECTED_INDEX").second);
            for(auto& e : ee)  {
                char buf[32];
                sprintf(buf, "Entity %d", i);
                if(ImGui::Selectable(buf, *selected_index == i)) {
                    *selected_index = i;
                    owner->StoreLayerResource("LIFE_SELECTED_OBJ", e.first, static_cast<void*>(&e.second), true);
                }
                i++;
            }
            // ImGui::Columns(1);
            //ImGui::Separator();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }
}
