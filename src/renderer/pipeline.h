#pragma once

#include "engine/lumix.h"
#include "engine/delegate.h"


struct lua_State;


namespace bgfx
{
	struct DynamicVertexBufferHandle;
	struct DynamicIndexBufferHandle;
	struct TextureHandle;
	struct UniformHandle;
	struct ProgramHandle;
	struct TransientVertexBuffer;
	struct TransientIndexBuffer;
	struct VertexDecl;
	struct VertexBufferHandle;
	struct IndexBufferHandle;
	struct InstanceDataBuffer;
}


namespace Lumix
{


struct Draw2D;
class FrameBuffer;
struct IAllocator;
struct Matrix;
class Material;
class Model;
class Path;
struct Pose;
class Renderer;
class RenderScene;
struct Vec4;


struct CommandBufferGenerator
{
	CommandBufferGenerator();

	void setTexture(u8 stage,
		const bgfx::UniformHandle& uniform,
		const bgfx::TextureHandle& texture,
		u32 flags = 0xffffFFFF);
	void setUniform(const bgfx::UniformHandle& uniform, const Vec4& value);
	void setUniform(const bgfx::UniformHandle& uniform, const Vec4* values, int count);
	void setUniform(const bgfx::UniformHandle& uniform, const Matrix* values, int count);
	void setTimeUniform(const bgfx::UniformHandle& uniform);
	void setLocalShadowmap(const bgfx::TextureHandle& shadowmap);
	void setGlobalShadowmap();
	int getSize() const { return int(pointer - buffer); }
	void getData(u8* data);
	void clear();
	void beginAppend();
	void end();

	u8 buffer[1024];
	u8* pointer;
};


class LUMIX_RENDERER_API Pipeline
{
	public:
		struct Stats
		{
			int draw_call_count;
			int instance_count;
			int triangle_count;
		};

		struct CustomCommandHandler
		{
			Delegate<void> callback;
			char name[30];
			u32 hash;
		};

	public:
		static void registerLuaAPI(lua_State* state);

		virtual ~Pipeline() {}

		static Pipeline* create(Renderer& renderer, const Path& path, const char* define, IAllocator& allocator);
		static void destroy(Pipeline* pipeline);

		virtual void load() = 0;
		virtual bool render() = 0;
		virtual void resize(int width, int height) = 0;
		virtual bgfx::TextureHandle& getRenderbuffer(const char* framebuffer_name, int renderbuffer_idx) = 0;
		virtual void setScene(RenderScene* scene) = 0;
		virtual RenderScene* getScene() = 0;
		virtual int getWidth() = 0;
		virtual int getHeight() = 0;
		virtual CustomCommandHandler& addCustomCommandHandler(const char* name) = 0;
		virtual void setViewProjection(const Matrix& mtx, int width, int height) = 0;
		virtual void setScissor(int x, int y, int width, int height) = 0;
		virtual bool checkAvailTransientBuffers(u32 num_vertices,
			const bgfx::VertexDecl& decl,
			u32 num_indices) = 0;
		virtual void allocTransientBuffers(bgfx::TransientVertexBuffer* tvb,
			u32 num_vertices,
			const bgfx::VertexDecl& decl,
			bgfx::TransientIndexBuffer* tib,
			u32 num_indices) = 0;
		virtual bgfx::TextureHandle createTexture(int width, int height, const u32* rgba_data) = 0;
		virtual void destroyTexture(bgfx::TextureHandle texture) = 0;
		virtual void setTexture(int slot,
			bgfx::TextureHandle texture,
			bgfx::UniformHandle uniform) = 0;
		virtual void destroyUniform(bgfx::UniformHandle uniform) = 0;
		virtual bgfx::UniformHandle createTextureUniform(const char* name) = 0;
		virtual void render(const bgfx::DynamicVertexBufferHandle& vertex_buffer,
			const bgfx::DynamicIndexBufferHandle& index_buffer,
			int first_index,
			int num_indices,
			int first_vertex,
			int num_vertices,
			u64 render_states,
			struct ShaderInstance& shader_instance) = 0;
		virtual void render(const bgfx::TransientVertexBuffer& vertex_buffer,
			const bgfx::TransientIndexBuffer& index_buffer,
			const Matrix& mtx,
			int first_index,
			int num_indices,
			u64 render_states,
			ShaderInstance& shader_instance) = 0;
		virtual void renderModel(Model& model, Pose* pose, const Matrix& mtx) = 0;
		virtual void toggleStats() = 0;
		virtual void setWindowHandle(void* data) = 0;
		virtual bool isReady() const = 0;
		virtual const Stats& getStats() = 0;
		virtual Path& getPath() = 0;
		virtual void callLuaFunction(const char* func) = 0;

		virtual Entity getAppliedCamera() const = 0;
		virtual void render(const bgfx::VertexBufferHandle& vertex_buffer,
			const bgfx::IndexBufferHandle& index_buffer,
			const bgfx::InstanceDataBuffer& instance_buffer,
			int count,
			Material& material) = 0;

		virtual Draw2D& getDraw2D() = 0;
};


} // namespace Lumix