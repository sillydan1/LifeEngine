#ifndef LAYERSTACK_HPP
#define LAYERSTACK_HPP
namespace life { class LayerStack; }
#include <layers/Layer.hpp>
#include <vector>
#include <map>

namespace life {
    // TODO: Make this a collection of std::unique_ptr instead
    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_layers.end(); }
        inline size_t size() const { return m_layers.size(); }
        
        bool StoreLayerResource(const std::string& resource_name, unsigned int data_size, void* data, bool override_if_exists = false);
        std::pair<unsigned int, void*> GetLayerResource(const std::string& resource_name) const;
		bool RemoveLayerResource(const std::string& resource_name);
	private:
        std::vector<Layer*> m_layers;
        unsigned int m_insertionPoint = 0;
        std::map<std::string, std::pair<unsigned int, void*>> m_layerResources;
    };

}

#endif 
