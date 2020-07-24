#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "kuma_math_types.hpp"
#include <life_core.h>
#include <unordered_map>
#include "Shader.hpp"

namespace life::kuma {
	
	class Material {
	public:
        using propertymap_t = std::unordered_map<GLint, KumaTypes>;

	    Material() = default;
		explicit Material(const std::string& shader);
		Material(const Material& other) = default; // TODO: Implement cpy ctor
		Material(Material&& other) = default;

		void Bind() const;
		void Unbind() const;
		
		static std::shared_ptr<Material> Create(const std::string& shader);
		void BindGlobalProperties() const;
		bool SetProperty(const std::string& uniform_name, const KumaTypes& dataptr);
		propertymap_t& GetProperties() { return m_pmap; }
		static bool SetGlobalProperty(const std::string& uniform_name,  const KumaTypes& dataptr);
		static std::optional<KumaTypes> GetGlobalProperty(const std::string& uniform_name);
		Shader& GetShader() const { return *m_shader; }
		
	private:
		void ParseShader(const std::string& shader_filename);

		propertymap_t m_pmap;
		std::shared_ptr<Shader> m_shader;
		int m_texture_counter{};
	};
	
}

#endif //MATERIAL_HPP
