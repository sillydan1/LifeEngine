#include "RenderLayer.hpp"
#include <kuma/kuma> // Include the kuma Rendering API
#include <config/lifeconfig.hpp>
#include <applications/Application.hpp>
#include <events/MouseEvents.hpp>
#include <events/KeyEvents.hpp>
#include <imgui/imgui.h>
// Rendering System(s)
#include "systems/rendering/RenderSystem.hpp"
#include "systems/rendering/GizmosRenderSystem.hpp"
#include "kuma/Skybox.hpp"
#include <systems/rendering/AnimationSystem.hpp>
#include <events/ApplicationEvents.hpp>

namespace life {
    /// Render layer
    /// This layer handles all that is needed for rendering the game world into
    /// a frame buffer. The base Render layer does not render an anti-aliased image
	struct RenderLayer::Impl {
		kuma::FrameBuffer 			framebuffer;
		kuma::FrameBuffer 			selector_framebuffer; // TODO: Gamerendering doesnt support selecting yet... Should it?
		kuma::AAFrameBuffer 		antialiasFrameBuffer;
		kuma::Renderer				renderer;
		std::shared_ptr<ecs::ECS> 	world_ref;
		ecs::SystemList				renderSystems;
		ecs::SystemList				editorRenderSystems;
		GizmosRenderSystem			gizmosRendersystem;
		RenderSystem				mainRenderSystem;
		// This shouldn't be here..
		kuma::Skybox                skybox;
		LifeCamera 					cam;
		bool 						left, right, forward, backward, up, down, camera_enabled, is_editor_camera, is_main_camera;
		
		Impl(std::shared_ptr<ecs::ECS>& world_reference, bool is_editor_camera = false, bool is_main_camera = false)
			: framebuffer{Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), SHADERDIR "/postprocess.glsl"},
			  selector_framebuffer{Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight(), SHADERDIR "/postprocess.glsl"},
			  antialiasFrameBuffer{Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight()},
			  renderer{},
			  world_ref{world_reference},
			  renderSystems{},
			  gizmosRendersystem{renderer},
			  mainRenderSystem{renderer},
			  skybox{},
			  cam{},
			  left{false}, right{false}, forward{false}, backward{false}, up{false}, down{false}, camera_enabled{false},
			  is_editor_camera{is_editor_camera}, is_main_camera{is_main_camera}
		{
			renderSystems.PushSystem(mainRenderSystem);
			renderSystems.PushSystem<AnimationSystem>();
			cam.Bind(); // We render the editor view by default.
		}

		void UpdateECSSystems() {
			UpdateCamera();
			world_ref->UpdateSystems(renderSystems);
		}
		float cameraSpeed = 0.01f;
		void UpdateCamera() {
			if(camera_enabled) {
				if(left)  	 cam.camera.ProcessKeyboard(Camera_Movement::LEFT, 	   cameraSpeed);
				if(right) 	 cam.camera.ProcessKeyboard(Camera_Movement::RIGHT,    cameraSpeed);
				if(forward)	 cam.camera.ProcessKeyboard(Camera_Movement::FORWARD,  cameraSpeed);
				if(backward) cam.camera.ProcessKeyboard(Camera_Movement::BACKWARD, cameraSpeed);
				if(up)  	 cam.camera.ProcessKeyboard(Camera_Movement::UP, 	   cameraSpeed);
				if(down)  	 cam.camera.ProcessKeyboard(Camera_Movement::DOWN, 	   cameraSpeed);
			}
			cam.Update();
		}
		
		void ProcessMouseMovement(float x, float y) {
			cam.camera.ProcessMouseMovement(x, y);
		}
		
		void ResizeWindow(const glm::vec<2, unsigned int, glm::defaultp>& new_dimensions) {
			if(new_dimensions.x > framebuffer.GetWidth() || new_dimensions.y > framebuffer.GetHeight()) {
				/// The pixels are now black, (or rather, they are whatever the GPU memory is) because
				/// the allocated memory for the framebuffer is not big enough, so we are beginning to
				/// bleed into unallocated memory, which can give funky results, but should essentially
				/// be relatively harmless. TODO: Re-alloc the framebuffers to be of the correct size now.
			}
			framebuffer.Resize(new_dimensions.x, new_dimensions.y);
			antialiasFrameBuffer.Resize(new_dimensions.x, new_dimensions.y);
			GLCall(glViewport(0, 0, new_dimensions.x, new_dimensions.y));
			cam.Update(new_dimensions.x, new_dimensions.y);
			Application::Get().GetWindow().SetDimensions(new_dimensions.x, new_dimensions.y);
		}
		
		void ResizeRenderTexture(const float nx, const float ny) {
            cam.Update(nx, ny);
		}
	};
	
    void RenderLayer::OnUpdate() {
    	pimpl->cam.Bind();
    	pimpl->antialiasFrameBuffer.Bind();
        // pimpl->framebuffer.Bind();
        GLCall(glEnable(GL_DEPTH_TEST));
        // ================================================================== //

		pimpl->renderer.Clear();
        pimpl->UpdateECSSystems();
        pimpl->renderer.Draw(pimpl->skybox);

        // ================================================================== //
		if(pimpl->is_editor_camera) {
			// Draw the selected object
			pimpl->gizmosRendersystem.RenderSelectedObject(owner, pimpl->mainRenderSystem);
			
			// Draw the world a second time for the selector
			kuma::Material::SetGlobalProperty("u_selectorpass", true);
			pimpl->selector_framebuffer.Bind();
			GLCall(glEnable(GL_DEPTH_TEST));
			pimpl->renderer.Clear();
			pimpl->UpdateECSSystems();
			pimpl->renderer.Draw(pimpl->skybox);
			kuma::Material::SetGlobalProperty("u_selectorpass", false);
		}
        
        pimpl->antialiasFrameBuffer.Blit(pimpl->framebuffer);
        pimpl->antialiasFrameBuffer.Unbind();

        /// Uncomment this to render to the entire window (Behind all the ImGui stuff)
        //pimpl->framebuffer.BindResources();
        //GLCall(glDrawArrays(GL_TRIANGLES, 0, 6)); // TODO: This should maybe not be done here.
        pimpl->cam.Unbind(); // Remembering to unbind because we're nice people
    }

    void RenderLayer::OnImGuiRender() {
    
    }

    void RenderLayer::OnAttach() {

    	if(pimpl->is_editor_camera) {
    	    kuma::Material::SetGlobalProperty("u_selectorpass", false);

			owner->StoreLayerResource("GL_FBO", sizeof(GLuint), pimpl->framebuffer.GetFBO());
			LIFE_LOG("GL_FBO \t= %d", *pimpl->framebuffer.GetFBO());
			owner->StoreLayerResource("GL_AAFBO", sizeof(GLuint), pimpl->antialiasFrameBuffer.GetFBO());
			owner->StoreLayerResource("GL_FBO_COLORATTACHMENT", sizeof(GLuint),
									  pimpl->framebuffer.GetColorAttachment());
			LIFE_LOG("GL_AAFBO \t= %d", *pimpl->antialiasFrameBuffer.GetFBO());
		
			owner->StoreLayerResource("TEST_FRAMEBUFFER", sizeof(kuma::FrameBuffer), &pimpl->selector_framebuffer);
            owner->StoreLayerResource("TEST_FRAMEBUFFER_COLORATTACHMENT", sizeof(GLuint), pimpl->selector_framebuffer.GetColorAttachment());
		}
		
		// TODO: Multiple game cameras, what do we do about that??
		if(pimpl->is_main_camera) {
			owner->StoreLayerResource("MAIN_CAMERA", sizeof(LifeCamera), &pimpl->cam);
			owner->StoreLayerResource("GAME_GL_FBO_COLORATTACHMENT", sizeof(GLuint),
									  pimpl->framebuffer.GetColorAttachment());
		}


		
		pimpl->renderer.SetClearColor(0,0,0);
		pimpl->renderer.Clear();
	}
	
    void RenderLayer::OnDetach() {

	}
	
	void RenderLayer::OnEvent(Event& event) {
		static float ox,oy; // old mouse x and y values
		switch(event.GetEventType()) {
			case EventType::WindowResize: {
				auto ev = static_cast<WindowResizeEvent&>(event);
				pimpl->ResizeWindow({ ev.GetWidth(), ev.GetHeight() });
				break; // Do not set it to handled = true, because other layers might need to react
			}
			case EventType::RenderResize: {
				auto ev = static_cast<RenderResizeEvent&>(event);
				pimpl->ResizeRenderTexture(ev.GetWidth(),ev.GetHeight());
				event.SetHandled();
				break; // Do not set it to handled = true, because other layers might need to react
			}
			case EventType::MouseButtonPressed: {
				if(static_cast<MouseButtonPressedEvent&>(event).getButton() == 1 && !pimpl->camera_enabled) {
					pimpl->camera_enabled = true;
					event.SetHandled(); // TODO: Other layers might need the mousebutton events too...
				}
				break;
			}
			case EventType::MouseButtonReleased: {
				if(static_cast<MouseButtonReleasedEvent&>(event).getButton() == 1 && pimpl->camera_enabled) {
					pimpl->camera_enabled = false;
					event.SetHandled();
				}
				break;
			}
			case EventType::MouseMoved: {
				MouseMovedEvent* mme = &static_cast<MouseMovedEvent&>(event);
				
				if(pimpl->camera_enabled) {
					float offsetx = mme->GetMouseX() - ox;
					float offsety = oy - mme->GetMouseY();
					pimpl->ProcessMouseMovement(offsetx, offsety);
					event.SetHandled();
				}
				ox = mme->GetMouseX();
				oy = mme->GetMouseY();
				break;
			}
			case EventType::KeyPressed: {
				KeyPressedEvent* ev = &static_cast<KeyPressedEvent&>(event);
				if(pimpl->camera_enabled) {
					switch(ev->getKeyCode()) {
						case 87: // W
							pimpl->forward = true;
							break;
						case 83: // S
							pimpl->backward = true;
							break;
						case 65: // A
							pimpl->left = true;
							break;
						case 68: // D
							pimpl->right = true;
							break;
						case 69:
							pimpl->up = true;
							break;
						case 81:
							pimpl->down = true;
							break;
					}
				}
				break;
			}
			case EventType::KeyReleased: {
				KeyReleasedEvent* ev = &static_cast<KeyReleasedEvent&>(event);
				switch(ev->getKeyCode()) {
					case 87: // W
						pimpl->forward = false;
						break;
					case 83: // S
						pimpl->backward = false;
						break;
					case 65: // A
						pimpl->left = false;
						break;
					case 68: // D
						pimpl->right = false;
						break;
					case 69: // E
						pimpl->up = false;
						break;
					case 81: // Q
						pimpl->down = false;
						break;
				}
				break;
			}
			default:
				break; // Do nothing
		}
	}
	
	RenderLayer::RenderLayer(std::shared_ptr<ecs::ECS>& world_reference, bool is_editor_pipeline, bool is_main_camera)
	 : Layer("RenderLayer"), 
	   pimpl{std::make_unique<RenderLayer::Impl>(world_reference, is_editor_pipeline, is_main_camera)}
	{
	
	}
	
}
