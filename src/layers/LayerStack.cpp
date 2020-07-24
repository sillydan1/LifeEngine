#include "LayerStack.hpp"
#include <algorithm>

namespace life {

    LayerStack::LayerStack() : m_layers{}, m_insertionPoint{0}, m_layerResources{} {
    }

    LayerStack::~LayerStack() {
        for(Layer* l : m_layers)
            delete l;
    }

    void LayerStack::PushLayer(Layer* layer) {
        m_layers.emplace(m_layers.begin() + m_insertionPoint, layer);
        m_insertionPoint++;
		layer->owner = this;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer* overlay) {
        m_layers.emplace_back(overlay);
		overlay->owner = this;
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer* layer) {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if(it != m_layers.end()) {
            m_layers.erase(it);
            m_insertionPoint--;
        }
        layer->OnDetach();
		layer->owner = nullptr;
    }

    void LayerStack::PopOverlay(Layer* overlay) {
        auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
        if(it != m_layers.end())
            m_layers.erase(it);
        overlay->OnDetach();
		overlay->owner = nullptr;
    }
	
	bool LayerStack::StoreLayerResource(const std::string &resource_name, unsigned int data_size, void *data, bool override_if_exists) {
    	// if(!data) { LIFE_WARN("Storing a nullptr to the LayerResource store"); }
    	if(m_layerResources.find(resource_name) != m_layerResources.end()) {
    		LIFE_WARN("Attempted to store a LayerResource with the name '%s' but the resource already exists. %s",
    				resource_name.c_str(), override_if_exists ? "Overriding" : "");
    		if(!override_if_exists) return false;
    	}
    	m_layerResources[resource_name] = {data_size, data};
		return true;
	}
	
	std::pair<unsigned int, void*> LayerStack::GetLayerResource(const std::string &resource_name) const {
		if(m_layerResources.find(resource_name) != m_layerResources.end()) {
			return m_layerResources.at(resource_name);
		}
		// LIFE_WARN("LayerResource '%s' does not exist.", resource_name.c_str());

		return {0, nullptr};
	}
	
	bool LayerStack::RemoveLayerResource(const std::string& resource_name) {
		return m_layerResources.erase(resource_name) > 0;
    }
	
}
