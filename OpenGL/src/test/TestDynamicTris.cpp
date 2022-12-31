﻿#include "TestDynamicTris.h"

#include "imgui.h"

namespace Test
{
    Maths::Vector4 TestDynamicTris::COLORS[8] = {
        { 1.0f, 0.0f, 0.0f, 1.0f }, // red
        { 0.0f, 1.0f, 0.0f, 1.0f }, // green
        { 0.0f, 0.0f, 1.0f, 1.0f }, // blue
        { 1.0f, 1.0f, 0.0f, 1.0f }, // magenta
        { 0.0f, 1.0f, 1.0f, 1.0f }, // yellow
        { 1.0f, 0.0f, 1.0f, 1.0f }, // cyan
        { 1.0f, 0.5f, 0.0f, 1.0f }, // orange 
        { 0.5f, 0.0f, 1.0f, 1.0f }, // purple
    };

    Maths::Matrix3D TestDynamicTris::ModelMatrix()
    {
        return Maths::Matrix3D::Transform({modelTranslation, 0.0f}, {modelScale, 1.0f}, {0.0f, 0.0f, modelRotation});
    }

    TestDynamicTris::TestDynamicTris()
        : projection(Maths::Matrix3D::OrthoProjection(-320.f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f))
    {
        va = new Graphics::VertexArray();
        vb = new Graphics::DynamicVertexBuffer<VertexColor3D>(MAX_TRIS * 3);
        
        va->AddBuffer(*vb);

        ib = new Graphics::DynamicIndexBuffer(MAX_TRIS * 3);
        shader = new Graphics::Shader("src/test/res/standard_colored.glsl");
        shader->Bind();

        shader->SetUniformMatrix4x4("u_MVP", projection);
        
        va->Unbind();
        vb->Unbind();
        ib->Unbind();
        shader->Unbind();

        VertexColor3D vc3[3] = {
            { { +00.0f, +50.0f, 0.0f }, COLORS[0]},
            { { -50.0f, -50.0f, 0.0f }, COLORS[0]},
            { { +50.0f, -50.0f, 0.0f }, COLORS[0]},
        };
        
        currentTri = Graphics::TriMesh<VertexColor3D>( vc3 );
    }

    TestDynamicTris::~TestDynamicTris()
    {
        delete va;
        delete vb;
        delete ib;
        delete shader;
    }

    void TestDynamicTris::OnUpdate(float deltaTime)
    {
        Test::OnUpdate(deltaTime);
    }

    void TestDynamicTris::OnRender(Graphics::Renderer& renderer)
    {
        Test::OnRender(renderer);

        vb->ClearData();
        ib->ClearData();

        for (unsigned int i = 0; i < triCount - 1; ++i) tris[i].AddTo(*vb, *ib);
        (ModelMatrix() * currentTri).AddTo(*vb, *ib);

        renderer.Draw(*va, *ib, *shader);
    }

    void TestDynamicTris::OnImGuiRender()
    {
        Test::OnImGuiRender();

        ImGui::DragFloat2("Current Tri Translation", modelTranslation       );
        ImGui::DragFloat2("Current Tri Scale      ", modelScale      , 0.10f);
        ImGui::DragFloat ("Current Tri Rotation   ", &modelRotation  , 0.05f);

        if (ImGui::Button("Add Tri") && !isMax)
        {
            if (triCount >= MAX_TRIS)
            {
                isMax = true;
            } else
            {
                tris[triCount - 1] = ModelMatrix() * currentTri;
                VertexColor3D vc3[3] = {
                    { { +00.0f, +50.0f, 0.0f }, COLORS[triCount]},
                    { { -50.0f, -50.0f, 0.0f }, COLORS[triCount]},
                    { { +50.0f, -50.0f, 0.0f }, COLORS[triCount]},
                };
                currentTri = Graphics::TriMesh<VertexColor3D>(vc3);
                modelTranslation = {0.0f, 0.0f};
                modelScale = {1.0f, 1.0f};
                modelRotation = 0.0f;
                
                ++triCount;
                
                isMin = false;
            }
        }
        
        if (isMax) ImGui::Text("Too Many Tris! (Limit: %i)", MAX_TRIS); 
        
        if (ImGui::Button("Remove Tri") && !isMin)
        {
            if (triCount <= 1) { isMin = true; } else
            {
                currentTri = tris[triCount - 2];
                modelTranslation = {0.0f, 0.0f};
                modelScale = {1.0f, 1.0f};
                modelRotation = 0.0f;
                --triCount;
                
                isMax = false;

                vb->ClearData(false);
                ib->ClearData(false);
            }
        }

        if (isMin) ImGui::Text("Can't Delete All Tris!");
    }
}
