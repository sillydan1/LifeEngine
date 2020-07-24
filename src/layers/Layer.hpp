#ifndef LAYER_HPP
#define LAYER_HPP
#include <string>
#include <events/Event.hpp>
namespace life { class Layer; }
#include "LayerStack.hpp"

namespace life {

    class Layer {
    	friend class LayerStack;
    public:
        explicit Layer(const std::string& layer_name)
                : m_dbgname(layer_name), owner{nullptr} {}
        virtual ~Layer() {}

        virtual void OnEvent(Event& event) {}
        virtual void OnUpdate() {}
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnImGuiRender() {}

        inline const std::string& GetName() { return m_dbgname; }
    protected:
        std::string m_dbgname;
        LayerStack* owner;
    };

}
#endif //CLASSNAME_HPP
