#include <util/overload>
#include "Material.hpp"
#include "shader_compiler/LifeShaderCompiler.hpp"

namespace life::kuma {
	using gpropertymap_t = std::unordered_map<std::string, KumaTypes>;
	static gpropertymap_t global_resources;

	using shader_pool_t = std::unordered_map<std::string, std::shared_ptr<Shader>>;
	shader_pool_t pool;

	Material::Material(const std::string& shader)
	 : m_pmap{},
	   m_shader{},
	   m_texture_counter{}
	{
		ParseShader(shader);
	}
	
	std::shared_ptr<Material> Material::Create(const std::string& shader) {
		return std::make_shared<Material>(shader);
	}
	
	void Material::Bind() const  {
		m_shader->Bind();
		BindGlobalProperties();
		// Iterate through the property map
		for(const auto& elem : m_pmap) {
			m_shader->SetUniform(elem.first, elem.second);
		}
	}
	
	void Material::BindGlobalProperties() const {
		// Bind the global resources
		for(const auto& elem : global_resources) {
			m_shader->SetUniform(m_shader->GetUniformLocation(elem.first), elem.second);
		}
	}
	
	void Material::Unbind() const {
		
	}

	void Material::ParseShader(const std::string& shader_filename) {
	    auto pp = pool.find(shader_filename);
	    if(pp != pool.end()) {
	        m_shader = pp->second;
	        return;
	    }
		// Parse the shader.
		shader_compiler::ShaderSource shadersources = shader_compiler::LifeShaderCompiler::Parse(shader_filename);
		// Instantiate the shader (pool if we can)
		m_shader = std::make_shared<Shader>(shader_filename);
		// Compile the shader. This step might be unnecessary
		m_shader->Compile(shadersources);
		pool[shader_filename] = m_shader;
	}

	bool Material::SetProperty(const std::string& uniform_name, const KumaTypes& dataptr) {
        // Get the GPU address
        GLint gpuaddr = m_shader->GetUniformLocation(uniform_name);
        if(gpuaddr == -1) return false; // The error is already being printed
        auto it = m_pmap.find(gpuaddr);
        if(it != m_pmap.end()) {
            int texcnt = 1;
            if(is_kuma_type_texture(it->second)) {
                const KumaTypes& tt = it->second;
                std::visit(
                        overload(
                                [&texcnt](const TYPE_PTR(Texture) &v)      { texcnt = v->GetTextureID(); },
                                [&texcnt](const TYPE_PTR(CubeTexture) &v)  { texcnt = v->GetTextureID(); },
                                [](auto&& a) { LIFE_ERR("Could not recognize given type for Texture! - Or the type is not supported yet! - %s", typeid(decltype(a)).name()); }
                        ), tt);
            }
            m_pmap[gpuaddr] = dataptr;
            if(is_kuma_type_texture(dataptr)) {
                std::visit(
                        overload(
                                [&texcnt](const TYPE_PTR(Texture) &v)      { v->SetTextureID(texcnt); },
                                [&texcnt](const TYPE_PTR(CubeTexture) &v)  { v->SetTextureID(texcnt); },
                                [](auto&& a) { LIFE_ERR("Could not recognize given type for Texture! - Or the type is not supported yet! - %s", typeid(decltype(a)).name()); }
                        ), dataptr);
            }
            return true;
        }
        bool succ = m_pmap.emplace(std::make_pair(gpuaddr, dataptr)).second;
        if(!succ) LIFE_LOG("Failed to register Material property to uniform '%s'", uniform_name.c_str());
        else if(is_kuma_type_texture(dataptr)) {
            std::visit(
                    overload(
                            [this](const TYPE_PTR(Texture) &v)      { v->SetTextureID(m_texture_counter++); },
                            [this](const TYPE_PTR(CubeTexture) &v)  { v->SetTextureID(m_texture_counter++); },
                            [](auto&& a) { LIFE_ERR("Could not recognize given type for Texture! - Or the type is not supported yet! - %s", typeid(decltype(a)).name()); }
                    ), dataptr);
        }
        return succ;
	}
	
	bool Material::SetGlobalProperty(const std::string& uniform_name,  const KumaTypes& dataptr) {
		// auto mypair = std::make_pair(uniform_name, dataptr); //TODO: Have Global property TEXTURE support.
		auto succ = global_resources.insert_or_assign(uniform_name, dataptr).second;
		// if(!succ) LIFE_ERR("Failed to register global Material property to uniform '%s'", uniform_name.c_str());
		return succ;
	}
	
	std::optional<KumaTypes> Material::GetGlobalProperty(const std::string &uniform_name) {
		auto r = global_resources.find(uniform_name);
		if(r == global_resources.end()) {
			return {};
		}
		return {r->second};
	}
}
