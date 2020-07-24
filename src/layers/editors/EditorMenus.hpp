#ifndef LIFE_ENGINE_EDITORMENUS_H
#define LIFE_ENGINE_EDITORMENUS_H
#include <layers/LayerStack.hpp>
#include <ecs/ECS.hpp>

namespace life::editor {
	/***
	 * Special promise here: This is completely stateless!
	 * */
	class EditorMenus {
	public:
		/// <File, Edit, Window, Help, etc.>
		static void DrawMenuBar(const std::shared_ptr<ecs::ECS>& world_ref);
		/// Selected entity editor window
		static void DrawEntityInspector(LayerStack* owner);
		/// The primary scene viewport
		static void DrawSceneViewer(LayerStack* owner, float& width, float& height);
		/// Drawing the scene from the Gameview perspective
		static void DrawGameViewer(const LayerStack* owner, float& width, float& height);
        /// Asset drag n drop interface
        static void DrawAssetSelector();

		// ---------------------------------------------------------------------------------------------------------- //
		/// Helper to display a little (?) mark which shows a tooltip when hovered.
		static void HelpMarker(const char* desc);
		/// Helper function to draw a checkbox for enabling/disabling wireframe rendering
		static void DrawWireframeOption(const LayerStack *owner);
		/// Helper function that draws a frame buffer. Make sure to do a "ImGui::Begin()" before calling this
        static void DrawViewport(const LayerStack *owner, float &width, float &height, const std::string& buffer);
	};
	
}

#endif //LIFE_ENGINE_EDITORMENUS_H
