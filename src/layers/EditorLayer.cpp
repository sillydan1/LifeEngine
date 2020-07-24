#include "EditorLayer.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <applications/Application.hpp>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>
#include <layers/editors/KumaTypeEditors.hpp>
#include <util/DebugInfo.h>
#include <events/ApplicationEvents.hpp>
#include "editors/EditorMenus.hpp"

#include <filesystem>
#include <events/KeyEvents.hpp>

namespace life {

	struct EditorLayer::Impl {
		struct SceneViewerData {
			float wsizex, wsizey;
		};

		std::shared_ptr<ecs::ECS> world_ref;
		SceneViewerData m_sceneviewdata, m_gameviewdata, m_selectorviewdata;
		std::shared_ptr<std::ostringstream> m_log_output;
		
		Impl(std::shared_ptr<ecs::ECS>& world_reference)
		 : world_ref(world_reference), m_sceneviewdata{}, m_log_output{new std::ostringstream()}
		{

		}
	};
	
    EditorLayer::EditorLayer(std::shared_ptr<ecs::ECS>& world_reference)
     : Layer("EditorLayer"), m_pimpl{std::make_unique<EditorLayer::Impl>(world_reference)} {

    }
 
	void EditorLayer::DrawDockspaceAndMenuBar(bool *p_open)
	{
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;
		
		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		
		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", p_open, window_flags);
		ImGui::PopStyleVar();
		
		if (opt_fullscreen)
			ImGui::PopStyleVar(2);
		
		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("LifeEditorDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}
		else {
			LIFE_ERR("Docking disabled?");
		}
		
		editor::EditorMenus::DrawMenuBar(m_pimpl->world_ref);
		
		ImGui::End();
	}
	
	void EditorLayer::OnImGuiRender() {
        static bool show = true;
		{ /// We will always have a dockspace.
			DrawDockspaceAndMenuBar(&show);
		}
		{ /// Entity selector
			editor::EntityGUISelector::DrawWorld(*m_pimpl->world_ref, owner, &show);
		}
        { /// ImGUI Demo Window
            //ImGui::ShowDemoWindow(&show);
        }
		{ /// Entity Editor
			editor::EditorMenus::DrawEntityInspector(owner);
		}
		{ /// Scene viewport
			editor::EditorMenus::DrawSceneViewer(owner, m_pimpl->m_sceneviewdata.wsizex, m_pimpl->m_sceneviewdata.wsizey);
		}
		{ /// Game viewport
			editor::EditorMenus::DrawGameViewer( owner, m_pimpl->m_gameviewdata.wsizex, m_pimpl->m_gameviewdata.wsizey);
		}
        { /// Asset selector (Drag n' drop interface)
            editor::EditorMenus::DrawAssetSelector();
        }
        { /// Debugging Console
            DrawDebugConsole();
        }
        { /// TEMP Selector buffer viewer
            ImGui::Begin("Selector buffer");
            editor::EditorMenus::DrawViewport(owner, m_pimpl->m_selectorviewdata.wsizex, m_pimpl->m_selectorviewdata.wsizey, "TEST_FRAMEBUFFER_COLORATTACHMENT");
            ImGui::End();
        }
    }

    void EditorLayer::OnAttach() {
        // set_output_stream(m_pimpl->m_log_output);
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void) io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        // Gamepad controls (not sure how IMGUI is supposed to use gamepad controls though..)
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; 
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        io.ConfigFlags |= ImGuiDragDropFlags_SourceAllowNullID;
	
		io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        ImGuiStyle& style = ImGui::GetStyle();
        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
        style.FrameRounding = 0.5f;

        { /// Load the font specified in the config file.
            char default_life_font[100] = ""; // TODO: Filepaths can be long, so maybe increase this
            LIFE_CONF_GET_VAL("editor_font", default_life_font);
            LIFE_DBG("Editor Font from config: '%s'", default_life_font);
            if(!default_life_font[0]) { // There's nothing in the config.
                // Set the value to whatever is loaded.
                LIFE_CONF_SET_VAL("editor_font", "res/fonts/DroidSans.ttf");
                LIFE_CONF_SAVE();
            } else {
                // Actually set the font we want
                float fontsize = 12;
                LIFE_CONF_GET_VAL("editor_font_size", fontsize);
                io.Fonts->AddFontFromFileTTF(default_life_font, fontsize);
            }
        }

        auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetAPIWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init();
        // By default, no object is selected.
        owner->StoreLayerResource("LIFE_SELECTED_OBJ", 0, nullptr);

        // Drawing the game as wireframe setting.
        void* draw_wireframe = owner->GetLayerResource("DRAW_WIREFRAME").second;
        if(!draw_wireframe) {
            // TODO: What to do with the deallocation of this?
            owner->StoreLayerResource("DRAW_WIREFRAME", sizeof(bool), new bool(false));
            draw_wireframe = owner->GetLayerResource("DRAW_WIREFRAME").second;
        }
        LIFE_LOG("Wireframe layer resource: %s", *(bool*)draw_wireframe ? "true" : "false");
        kuma::Material::SetGlobalProperty("u_draw_wireframe", TYPE_PTR(bool)((bool*)draw_wireframe));
    }

    void EditorLayer::OnDetach() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void EditorLayer::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void EditorLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        auto& w = Application::Get().GetWindow();
        io.DisplaySize = ImVec2(w.GetWidth(), w.GetHeight());

        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void EditorLayer::OnEvent(Event& event) {
        switch(event.GetEventType()) {
            case EventType::KeyPressed: {
                KeyPressedEvent* ev = &static_cast<KeyPressedEvent&>(event);
                switch (ev->getKeyCode()) {
                    case 261: // Delete
                        DeleteSelectedObject();
                        break;
                    default:
                        break;
                }
                break;
            }
            default:
                break;
        }
    }

    void EditorLayer::DeleteSelectedObject() {
        auto obj = owner->GetLayerResource("LIFE_SELECTED_OBJ");
        if(obj.second) {
            LIFE_DBG("Deleting object with hash: %d", obj.first);
            if(m_pimpl->world_ref->RemoveEntity(obj.first, static_cast<std::vector<std::shared_ptr<ecs::BaseComponent>>*>(obj.second))) {
                owner->RemoveLayerResource("LIFE_SELECTED_OBJ");
                // Deselect it.
                if (owner->GetLayerResource("LIFE_SELECTED_INDEX").second) {
                    delete static_cast<int*>(owner->GetLayerResource("LIFE_SELECTED_INDEX").second);
                    owner->RemoveLayerResource("LIFE_SELECTED_INDEX"); // Don't worry, it'll get bootstrapped again
                }
            } else {
                LIFE_WARN("Unable to delete selected object.");
            }
        }
    }

    void EditorLayer::DrawDebugConsole() {
        ImGui::Begin("Console");
        // TODO: Make this formatted...
        ImGui::Text("%s", m_pimpl->m_log_output->str().c_str());
        ImGui::End();
    }
}
