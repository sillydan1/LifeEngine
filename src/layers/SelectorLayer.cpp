#include "SelectorLayer.hpp"
#include <kuma/kuma>
#include <applications/Application.hpp>
#include <events/MouseEvents.hpp>
#include <kuma/LifeSelectable.hpp>

namespace life::editor {
	
	struct SelectorLayer::Impl {
		std::shared_ptr<ecs::ECS> world_refference;
		kuma::Renderer renderer;
		kuma::FrameBuffer selection_framebuffer;
		LayerStack* owner;
		
		Impl(std::shared_ptr<ecs::ECS>& wref, LayerStack* owner)
		 : world_refference{wref},
		   renderer{},
		   selection_framebuffer{Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), SHADERDIR "/selection_buffer.glsl"},
		   owner{owner}
		{
		
		}
		
		void SelectionRoutine(float mousex, float mousey) {
			// Figure out where this resides within the primary frame buffer
			// -- This is no trivial task, since the scene view in the editor
			//    may start somewhere in the middle of the window.
			//    Maybe it will even be outside the window -- In a detached ImGui window
			
			// Check if the click was outside the rendered framebuffer.
			
			
			// Sample color from the selection_framebuffer
			auto test_framebuffer_resource = owner->GetLayerResource("TEST_FRAMEBUFFER"); // There's also for testing: TEST_FRAMEBUFFER_COLORATTACHMENT (GLuint)
			auto primary_selection_window = owner->GetLayerResource("SELECTION_WINDOW_SIZE_AND_LOCATION");
			if(test_framebuffer_resource.second && primary_selection_window.second) {
            	auto* framebuffer = static_cast<kuma::FrameBuffer*>(test_framebuffer_resource.second);
            	// ----
            	glm::vec2 framebuffer_dimensions = glm::vec2(framebuffer->GetWidth(), framebuffer->GetHeight());
            	// Get the rendered image size and location
            	auto* window_size_loc = static_cast<glm::vec4*>(primary_selection_window.second);
            	bool clicked_inside_window = mousex > window_size_loc->x && mousex < window_size_loc->x+window_size_loc->z &&
						mousey > window_size_loc->y && mousey < window_size_loc->y+window_size_loc->w;
            	if(!clicked_inside_window) return;
            	
				auto ratiox = framebuffer_dimensions.x / window_size_loc->z;
				auto ratioy = framebuffer_dimensions.y / window_size_loc->w;
				auto moved_mousex = mousex - window_size_loc->x;
				auto moved_mousey = mousey - window_size_loc->y;
				auto converted_mx = static_cast<unsigned int>(moved_mousex * ratiox);
				auto converted_my = static_cast<unsigned int>(moved_mousey * ratioy);
				// ----
                auto sample = framebuffer->SampleColor(converted_mx, converted_my);
                auto& entities = world_refference->GetAllEntities();
                for(auto& entity : entities) {
					for(auto& component : entity.second) {
						auto test = std::dynamic_pointer_cast<kuma::LifeSelectable>(component);
						if(test) {
							glm::vec3 tc = *test->selectorcolor;
							float min_dif = 0.009f; // TODO: This should be a configurable thing later on...
							if(Vec3NumericDiff(tc, sample, min_dif)) {
								owner->StoreLayerResource("LIFE_SELECTED_OBJ", entity.first,
														  static_cast<void *>(&entity.second), true);
								return;
							}
						}
					}
                }
                owner->RemoveLayerResource("LIFE_SELECTED_OBJ");
            }
		}
	};
	
	SelectorLayer::SelectorLayer(std::shared_ptr<ecs::ECS>& world_reference)
	 : Layer("SelectorLayer"), m_pimpl{std::make_unique<SelectorLayer::Impl>(world_reference, nullptr)}
	{
	    kuma::Material::SetGlobalProperty("u_isselected", false);
	}
	
	void SelectorLayer::OnEvent(Event &event) {
		Layer::OnEvent(event);
		switch(event.GetEventType()) {
			case EventType::MouseButtonReleased: {
				auto ev = static_cast<MouseButtonReleasedEvent &>(event);
				// TODO: Check if the button press was within the viewport
				if (ev.getButton() == 0) { // LMB up
					m_pimpl->SelectionRoutine(ev.GetMouseX(), ev.GetMouseY());
					event.SetHandled();
				}
				break;
			}
			default:
				break;
		}
	}
	
	void SelectorLayer::OnUpdate() {
		// Bind the selection_framebuffer
		// render the world.
		
	}
	
	void SelectorLayer::OnAttach() {
		Layer::OnAttach();
		m_pimpl->owner = owner;
	}
	
	void SelectorLayer::OnDetach() {
		Layer::OnDetach();
	}
}
