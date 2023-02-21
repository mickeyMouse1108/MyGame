﻿#pragma once

#include "DynamicIndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "VertexArray.h"

#include <GLFW/glfw3.h>

namespace Graphics
{
    class MeshObject;
    
    // TODO: make this changeable
    class GraphicsDevice
    {
        using                    Vertex = VertexColorTextureAtlas3D;
        template <class T> using ref = std::reference_wrapper<T>;
        static constexpr unsigned int MAX_VERTEX_COUNT = 1000;
        static constexpr unsigned int MAX_INDEX_COUNT = 1000;
    private:
        std::vector<ref<MeshObject>> _meshObjs;
        
        std::unique_ptr<VertexArray>                 _vertexArray;
        std::unique_ptr<DynamicVertexBuffer<Vertex>> _vertexBuffer;
        std::unique_ptr<DynamicIndexBuffer>          _indexBuffer;
        std::unique_ptr<Shader>                      _currentShader;
        
        std::unique_ptr<Renderer>                    _renderer;

        Maths::Matrix3D _projection = Maths::Matrix3D::OrthoProjection(-320, 320, -240, 240, -1000, 1000);
        Maths::Matrix3D _camera;

        GLFWwindow* _mainWindow;
    // public:
        public:
        GraphicsDevice(GLFWwindow* window);
        using string = const std::string&;

        ~GraphicsDevice();
        void BeginRender();
        void EndRender();
        
        void ClearRegistered() const;
        void RegisterElements() const;
        void RegisterNewElements(MeshObject* meshObjs, unsigned int objCount) const;
        void RenderRegistered() const;
        void AddMeshObject(MeshObject* meshObjs, unsigned int objCount);
        void Delete(int index);
        void Delete(int indexStart, int indexEnd);
        void UpdateMeshIndices() const;

        void ClearColor(const Maths::Vector3& color);
        
        bool WindowIsOpen() const { return !glfwWindowShouldClose(_mainWindow); }

        Renderer& GetRenderer() { return *_renderer; }
        void EnableShader();
        void DisableShader();
        void UseShader          (string shaderPath)                                 { _currentShader = std::make_unique<Shader>(shaderPath); }
#define SHADER_UNIF(x) _currentShader->Bind(); _currentShader->SetUniform##x; _currentShader->Unbind()
        void SetUniform1Int     (string unifName, int val)                    const { SHADER_UNIF(1I(unifName, val));                 }
        void SetUniform2Int     (string unifName, const Maths::Vec2Int& val)  const { SHADER_UNIF(2I(unifName, (const int*)&val));    }
        void SetUniform3Int     (string unifName, const Maths::Vec3Int& val)  const { SHADER_UNIF(3I(unifName, (const int*)&val));    }
        void SetUniform4Int     (string unifName, const Maths::Vec4Int& val)  const { SHADER_UNIF(4I(unifName, (const int*)&val));    }
        
        void SetUniform1IntArr  (string unifName, int vals[], unsigned num)   const { SHADER_UNIF(1IVec(unifName, vals, num));        }
        
        void SetUniform1Float   (string unifName, float val)                  const { SHADER_UNIF(1F(unifName, val));                 }
        void SetUniform2Float   (string unifName, const Maths::Vector2& val)  const { SHADER_UNIF(2F(unifName, (const float*)&val) ); }
        void SetUniform3Float   (string unifName, const Maths::Vector3& val)  const { SHADER_UNIF(3F(unifName, (const float*)&val) ); }
        void SetUniform4Float   (string unifName, const Maths::Vector4& val)  const { SHADER_UNIF(4F(unifName, (const float*)&val) ); }
        
        void SetUniform4x4Matrix(string unifName, const Maths::Matrix3D& val) const { SHADER_UNIF(Matrix4x4(unifName, val)   ); }
#undef SHADER_UNIF
        
        void SetCamera(const Maths::Matrix3D& camera) { _camera = camera; }

        void DebugMenu();

        //void Render(bool autoSort = true);
        
        static std::unique_ptr<GraphicsDevice> Initialize();

        friend class MeshObject;
    };
}
