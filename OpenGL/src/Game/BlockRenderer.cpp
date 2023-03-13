﻿#include "BlockRenderer.h"
#include "BlockBase.h"

namespace Game
{
    Maths::Vector3 BlockRenderer::BLOCK_VERTICES[8] = {
        { -0.5f, -0.5f, -0.5f }, 
        { -0.5f, -0.5f, +0.5f }, 
        { -0.5f, +0.5f, -0.5f }, 
        { -0.5f, +0.5f, +0.5f }, 
        { +0.5f, -0.5f, -0.5f }, 
        { +0.5f, -0.5f, +0.5f }, 
        { +0.5f, +0.5f, -0.5f }, 
        { +0.5f, +0.5f, +0.5f }, 
    };

    constexpr float TEX = 0;
    
    BlockRenderer::Quad BlockRenderer::BLOCK_FACES[6] = { // VERT / COL / TEXID / CORNER
        Quad { { BLOCK_VERTICES[0], { 1, 1, 1, 1 }, TEX, 0 }, { BLOCK_VERTICES[1], { 1, 1, 1, 1 }, TEX, 1 },    // D
               { BLOCK_VERTICES[2], { 1, 1, 1, 1 }, TEX, 2 }, { BLOCK_VERTICES[3], { 1, 1, 1, 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[4], { 1, 1, 1, 1 }, TEX, 0 }, { BLOCK_VERTICES[5], { 1, 1, 1, 1 }, TEX, 1 },    // 
               { BLOCK_VERTICES[6], { 1, 1, 1, 1 }, TEX, 2 }, { BLOCK_VERTICES[7], { 1, 1, 1, 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[0], { 1, 1, 1, 1 }, TEX, 0 }, { BLOCK_VERTICES[1], { 1, 1, 1, 1 }, TEX, 1 },
               { BLOCK_VERTICES[4], { 1, 1, 1, 1 }, TEX, 2 }, { BLOCK_VERTICES[5], { 1, 1, 1, 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[2], { 1, 1, 1, 1 }, TEX, 0 }, { BLOCK_VERTICES[3], { 1, 1, 1, 1 }, TEX, 1 },
               { BLOCK_VERTICES[6], { 1, 1, 1, 1 }, TEX, 2 }, { BLOCK_VERTICES[7], { 1, 1, 1, 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[0], { 1, 1, 1, 1 }, TEX, 0 }, { BLOCK_VERTICES[2], { 1, 1, 1, 1 }, TEX, 1 },
               { BLOCK_VERTICES[4], { 1, 1, 1, 1 }, TEX, 2 }, { BLOCK_VERTICES[6], { 1, 1, 1, 1 }, TEX, 3 }, },
        Quad { { BLOCK_VERTICES[1], { 1, 1, 1, 1 }, TEX, 0 }, { BLOCK_VERTICES[3], { 1, 1, 1, 1 }, TEX, 1 },
               { BLOCK_VERTICES[5], { 1, 1, 1, 1 }, TEX, 2 }, { BLOCK_VERTICES[7], { 1, 1, 1, 1 }, TEX, 3 }, },
    };

    BlockRenderer::BlockRenderer(BlockBase& parent, unsigned enabledFlags) : enabledFlags(enabledFlags), parentBlock(&parent) {}

    BlockRenderer::BlockRenderer(const BlockRenderer& copy) : enabledFlags(copy.enabledFlags), textureID(copy.textureID), parentBlock(copy.parentBlock) {}
    BlockRenderer::BlockRenderer(BlockRenderer&& copy) noexcept : enabledFlags(copy.enabledFlags), textureID(copy.textureID), parentBlock(copy.parentBlock) {
        copy.parentBlock = nullptr;
    }
    BlockRenderer& BlockRenderer::operator=(const BlockRenderer& copy) noexcept {
        if (this == &copy) return *this;
        enabledFlags = copy.enabledFlags;
        textureID = copy.textureID;
        parentBlock = copy.parentBlock;
        return *this;
    }
    BlockRenderer& BlockRenderer::operator=(BlockRenderer&& copy) noexcept {
        enabledFlags = copy.enabledFlags;
        textureID = copy.textureID;
        parentBlock = copy.parentBlock;
        copy.parentBlock = nullptr;
        return *this;
    }

    const Maths::Vec3Int& BlockRenderer::GetPosition() const { return parentBlock->Position; }

    void BlockRenderer::SetTexture(Graphics::QuadMesh<Vertex>& mesh, int textureID) {
        Vertex* subMeshStart = mesh.GetVertices();
        // we know its 4 corners per mesh. probably.
        // decrement first so pre-incr doesnt overflow to index 4.
        // could use post-incr but for optimization purposes
        // (its probably gonna get optim'd anyway but whatever)
        --subMeshStart;
        for (int i = 0; i < 4; ++i)
            (++subMeshStart)->TextureAtlID = (intf)textureID;
    }

    Graphics::MeshObject& BlockRenderer::GetMeshObjectForm()
    {
        std::vector<Quad*> faces;
        
        for (int i = 0; i < 6; ++i)
            if (enabledFlags & 1 << i) {
                faces.emplace_back(new Graphics::QuadMesh(BLOCK_FACES[i]));
                SetTexture(*faces.back(), textureID[i]);
            }

        meshObj = Graphics::MeshObject::Make<Graphics::QuadMesh>(faces.data(), faces.size());
        meshObj.Transform(Maths::Matrix3D::TranslateMat(parentBlock->Position));
        return meshObj;
    }
}
