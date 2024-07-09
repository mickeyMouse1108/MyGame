﻿#include "TestCubeRender.h"
#include "Primitives/Quad.h"

#include "imgui.h"
#include "Meshes/Cube.h"

namespace Test {
    void TestCubeRender::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<Graphics::VertexColor3D>(4 * 6, 12);

        using Graphics::Primitives::Quad;
        using namespace Math;

        using namespace Graphics::VertexBuilder;
        u32 i = 0 - 1;
        cube = Graphics::MeshUtils::Cube(Graphics::VertexColor3D::Blueprint {
            .Position = GetPosition {},
            .Color = FromArg<>([&] { ++i; return fColor::color_id((int)(i / 4) + 1); })
        });

        render.UseShaderFromFile(res("shader.vert"), res("shader.frag"));
        render.SetProjection(projection);
    }

    void TestCubeRender::OnRender(Graphics::GraphicsDevice& gdevice) {
        const Math::Matrix3D mat = Math::Matrix3D::transform(modelTranslation, modelScale, modelRotation);

        render.SetCamera(mat);
        render.Draw(cube, Graphics::UseArgs({{ "u_alpha", alpha }}));
    }

    void TestCubeRender::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        ImGui::DragFloat3("Translation" , modelTranslation.begin(), 0.01f);
        ImGui::DragFloat3("Scale"       , modelScale.begin(),       0.01f);
        ImGui::DragFloat3("Rotation"    , modelRotation.begin(),    0.01f);
        ImGui::DragFloat ("Transparency", &alpha,                   0.01f);
    }

    void TestCubeRender::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        render.Destroy();
    }
}
