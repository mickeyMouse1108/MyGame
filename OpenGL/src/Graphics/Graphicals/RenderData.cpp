#include "RenderData.h"
#include "GraphicsDevice.h"

namespace Graphics {
	void RenderData::Transfer(RenderData& dest, RenderData&& from) {
		dest.varray = std::move(from.varray);
		dest.vbo = std::move(from.vbo);
		dest.ibo = std::move(from.ibo);

		dest.meshes = std::move(from.meshes);

		dest.device = from.device;
		from.device = nullptr;
		dest.deviceIndex = from.deviceIndex;
		from.deviceIndex = 0;
	}

	RenderData::~RenderData() {
		if (device)
			device->DeleteRender(deviceIndex);
	}

	void RenderData::Bind() const {
		varray.Bind();
		vbo.Bind();
		ibo.Bind();
	}

	void RenderData::Unbind() const {
		varray.Unbind();
		vbo.Unbind();
		ibo.Unbind();
	}

	void RenderData::ClearData(bool shallowClear) {
		vbo.ClearData(shallowClear);
		ibo.ClearData(shallowClear);
	}

	void RenderData::Render() {
		device->Render(*this);
	}

	void RenderData::UnbindMesh(int index) {
		meshes.erase(meshes.begin() + index);
		UpdateMeshIndicies();
	}

	void RenderData::UnbindMeshes(int indexStart, int indexEnd) {
		meshes.erase(meshes.begin() + indexStart, meshes.begin() + indexEnd);
		UpdateMeshIndicies();
	}

	void RenderData::UpdateMeshIndicies() {
		for (uint i = 0; i < meshes.size(); ++i)
			meshes[i].deviceIndex() = i;
	}

	void RenderData::Destroy() {
		GraphicsDevice* prev = device; // prevent infinte loop: deleterender -> erase renderdata -> destructor
		device = nullptr;
		prev->DeleteRender(deviceIndex);
		deviceIndex = 0;
	}
};