#pragma once
#include "K_Renderable.h"
#include "puppy\TextureBlendMaterial.h"
#include "puppy\VertexEnvironment.h"

namespace kitten
{
	class QuadRenderableRepeat : public K_Renderable
	{
	private:
		puppy::VertexEnvironment* m_vao;

		puppy::Material m_mat;

		bool m_isStatic, m_isRenderingStatic, m_texRepeat;
		GLfloat m_uRepeat, m_vRepeat;
		puppy::Texture* m_staticTex;

		void addToStaticRender();

		virtual void start() override;
		virtual void onDisabled() override;
		virtual void onEnabled() override;
	public:
		QuadRenderableRepeat(const std::string& p_texPath = "", bool p_isStatic = false, bool p_texRepeat = false, GLfloat p_uRepeat = 1.0f, GLfloat p_vRepeat = 1.0f);
		virtual ~QuadRenderableRepeat();

		void setTexture(const char* p_pathToTex);

		void render(const glm::mat4& p_viewProj) override;
	};
}
