#include "TestPostProcessing.h"

#include "Constants.h"
#include "imgui.h"
#include "Mesh.h"
#include "Quad.h"

namespace Test {
    void TestPostProcessing::OnInit(Graphics::GraphicsDevice& gdevice) {
                scene = gdevice.CreateNewRender<VertexColor3D>(72, 108);
        postProcessingQuad = gdevice.CreateNewRender<VertexTexture2D>(4, 2);

        cubes.reserve(9);

        constexpr float s = 0.5f;
        cubes.push_back(Graphics::MeshUtils::CubeMesh(0, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_GRAY()));
        cubes.push_back(Graphics::MeshUtils::CubeMesh({  1,  1,  1 }, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_WHITE()));
        cubes.push_back(Graphics::MeshUtils::CubeMesh({  1,  1, -1 }, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_YELLOW()));
        cubes.push_back(Graphics::MeshUtils::CubeMesh({  1, -1,  1 }, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_PURPLE()));
        cubes.push_back(Graphics::MeshUtils::CubeMesh({  1, -1, -1 }, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_RED()));
        cubes.push_back(Graphics::MeshUtils::CubeMesh({ -1,  1,  1 }, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_CYAN()));
        cubes.push_back(Graphics::MeshUtils::CubeMesh({ -1,  1, -1 }, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_LIME()));
        cubes.push_back(Graphics::MeshUtils::CubeMesh({ -1, -1,  1 }, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_BLUE()));
        cubes.push_back(Graphics::MeshUtils::CubeMesh({ -1, -1, -1 }, s, s, s)
            .ApplyMaterial(&VertexColor3D::Color, Maths::colorf::BETTER_BLACK()));

        scene.BindMeshes(cubes);
        scene.UseShader(Graphics::Shader::StdColored);
        scene.SetProjection(projection);

        const auto [winX, winY] = gdevice.GetWindowSize();
        fbo = Graphics::FrameBuffer {{}};
        fbo.Bind();

        renderResult = Graphics::Texture {
            nullptr, (uint)winX, (uint)winY,
            true, Graphics::TextureFormat::RGB, (Graphics::TextureInternalFormat)Graphics::TextureFormat::RGB
        };

        fbo.Attach(renderResult);

        depthStencilAttachment = Graphics::RenderBuffer {
            Graphics::TextureInternalFormat::DEPTH_24_STENCIL_8, gdevice.GetWindowSize()
        };

        fbo.Bind();
        fbo.Attach(depthStencilAttachment, Graphics::AttachmentType::DEPTH_STENCIL);
        fbo.Complete();
        fbo.Unbind();

        screenQuad = Graphics::Primitives::Quad { 0, Maths::fvec3::RIGHT(), Maths::fvec3::UP() }
                    .IntoMesh<VertexTexture2D>(
                        [](Maths::fvec3 v) -> Maths::fvec2 { return v.xy(); }, &VertexTexture2D::Position)
                    .Convert<VertexTexture2D>([](const VertexTexture2D& v) {
                        return VertexTexture2D { v.Position, (v.Position + 1) * 0.5f };
                     });
        postProcessingQuad.BindMeshes(screenQuad);

        const char* vert = "res\\TestPostProcessing\\vertex.vert";
        postProcessingQuad.UseShaderFromFile(vert, "res\\TestPostProcessing\\none.frag");
        // inv shader
        shaderInv = Graphics::Shader::FromFile(vert, "res\\TestPostProcessing\\invert.frag");
        // hsl shader
        shaderHsv = Graphics::Shader::FromFile(vert, "res\\TestPostProcessing\\hsv.frag");
        // blur shader
        shaderBlur = Graphics::Shader::FromFile(vert, "res\\TestPostProcessing\\simple_blur.frag");
        // edge detect shader
        shaderEdgeDetect = Graphics::Shader::FromFile(vert, "res\\TestPostProcessing\\simple_ed.frag");

        currShader = &postProcessingQuad.GetShader();
        renderResult.Activate(0);
    }

    void TestPostProcessing::OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) {
        modelRotation += turnSpeed * deltaTime;
        modelRotation = modelRotation.apply([](const float f) -> float { return std::fmod(f, Maths::TAU); });
    }

    void TestPostProcessing::OnRender(Graphics::GraphicsDevice& gdevice) {
        Maths::mat3D mat = Maths::mat3D::transform(modelTranslation, modelScale, modelRotation);

        scene.SetCamera(mat);

        Graphics::Render::EnableDepth();
        if (usePostProcessing) {
            fbo.Bind();
            Graphics::Render::Clear();
        }

        scene.ResetData();
        Graphics::Shader& shader = scene.GetShader();
        shader.Bind();
        shader.SetUniformMatrix4x4("u_projection", projection);
        shader.SetUniformMatrix4x4("u_view", mat);
        Graphics::Render::Draw(scene.GetRenderData());

        if (usePostProcessing) {
            Graphics::Render::DisableDepth();
            fbo.Unbind();

            // Graphics::Render::SetClearColor(1);
            Graphics::Render::ClearColorBit();

            currShader->Bind();
            currShader->SetUniformTex("screenTexture", renderResult);
            if (currShader == &shaderBlur) {
                currShader->SetUniform2F("blurOff", effectOff.begin());
            } else if (currShader == &shaderEdgeDetect) {
                currShader->SetUniform2F("detectOff", effectOff.begin());
            } else if (currShader == &shaderHsv) {
                currShader->SetUniform1F("dh", hueShift);
                currShader->SetUniform1F("ds", satMul);
                currShader->SetUniform1F("dv", valShift);
            }

            postProcessingQuad.ResetData();
            Graphics::Render::Draw(postProcessingQuad.GetRenderData(), *currShader);
        }
    }

    void TestPostProcessing::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        ImGui::DragFloat3("Translation" , modelTranslation.begin(), 0.01f);
        ImGui::DragFloat3("Scale"       , modelScale.begin(),       0.01f);
        ImGui::DragFloat3("Rotation"    , modelRotation.begin(),    0.01f);

        ImGui::DragFloat3("Spin Speed", turnSpeed.begin(), 0.01f, -10, 10);

        ImGui::Separator();

        ImGui::Checkbox("Use Post Processing", &usePostProcessing);

#define TAB_ITEM(X, N, P, C) if (ImGui::BeginTabItem(N)) { currShader = &(P); ImGui::EndTabItem(); C }
        if (ImGui::BeginTabBar("Post Processing Shader")) {
            TAB_ITEM(NONE, "None", postProcessingQuad.GetShader(), )
            TAB_ITEM(COLOR_INVERT, "Color Invert", shaderInv, )
            TAB_ITEM(COLOR_HSL, "Color Hue", shaderHsv,
                ImGui::DragFloat("Hue Shift", &hueShift, 0.01f, 0, 1);
                ImGui::DragFloat("Saturation Multiplier", &satMul, 0.01f, 0);
                ImGui::DragFloat("Value Shift", &valShift, 0.01f, -1, 1);)
            TAB_ITEM(BLUR, "Blur", shaderBlur, ImGui::DragFloat2("Blur Offset", effectOff.begin(), 0.1f); )
            TAB_ITEM(EDGE_DETECT, "Edge Detection", shaderEdgeDetect, ImGui::DragFloat2("Detect Offset", effectOff.begin(), 0.1f); )
            ImGui::EndTabBar();
        }
#undef TAB_ITEM
    }

    void TestPostProcessing::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        scene.Destroy();
        Graphics::Render::EnableDepth();
    }
}
