#include "TestShadowMap.h"

#include <imgui.h>

#include "VertexBlueprint.h"
#include "Extension/ImGuiExt.h"
#include "Meshes/CubeNormless.h"
#include "Meshes/Quad.h"
#include "ModelLoading/OBJModelLoader.h"

namespace Test {
    void TestShadowMap::OnInit(Graphics::GraphicsDevice& gdevice) {
        scene = gdevice.CreateNewRender<Vertex>(2560, 1280);

        Graphics::OBJModelLoader mloader;
        mloader.LoadFile(res("untitled.obj"));
        const Graphics::OBJModel model = mloader.RetrieveModel();

        for (int i = 0; i < model.objects.Length(); ++i) {
            meshes.Push(
                model.objects[i].mesh.GeometryMap<Vertex>(QGLCreateBlueprint$(Vertex, (
                    in (Position, Normal),
                    out (Position) = Position;,
                    out (Color) = Math::fColor::color_id(1 + i);,
                    out (Normal) = Normal;
                ))
            ));
        }

        depthShader = Graphics::Shader::FromFile(res("depth.vert"), res("depth.frag"));
        scene.UseShaderFromFile(res("shadow.vert"), res("shadow.frag"));

        depthMap = Graphics::FrameBuffer::New();
        depthTex = Graphics::Texture::New(
            nullptr, gdevice.GetWindowSize().as<u32>(), {
                .load = {
                    .format = Graphics::TextureFormat::DEPTH,
                    .internalformat = Graphics::TextureIFormat::DEPTH_16,
                    .type = Graphics::GLTypeID::FLOAT
                },
                .params = {
                    { Graphics::TextureParamName::XT_WRAPPING, Graphics::TextureBorder::CLAMP_TO_BORDER },
                    { Graphics::TextureParamName::BORDER_COLOR, Math::fVector4::ONE().cbegin() }
                }
            }
        );
        depthMap.Bind();
        depthMap.Attach(depthTex, Graphics::AttachmentType::DEPTH);
        Graphics::Render::SetColorWrite(Graphics::BufferMode::NONE);
        depthMap.Complete();
        depthMap.Unbind();

        shadowMapDisplay = gdevice.CreateNewRender<Graphics::VertexTexture2D>(4, 2);
        shadowMapDisplay.UseShaderFromFile(res("display.vert"), res("display.frag"));

        screenQuad = Graphics::MeshUtils::Quad(QGLCreateBlueprint$(Graphics::VertexTexture2D, (
            in (Position),
            out (Position) = Position;,
            out (TextureCoordinate) = (Position + 1) * 0.5f;
        )));
        depthTex.Activate(0);

        Graphics::Render::EnableCullFace();
        Graphics::Render::SetFrontFacing(Graphics::OrientationMode::CLOCKWISE);

        camera.position = { -9.684464, 10.050441, -1.4917407 };
        camera.yaw = -8.13158; camera.pitch = 0.6891256;
        camera.speed = 5;
        camera.sensitivity = 0.12;
        camera.fov = 90;
        camera.fovRange = { 1, 90 };
        camera.zoomRatio = 0.5;
        camera.smoothZoom = 120;
    }

    void TestShadowMap::OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) {
        camera.Update(gdevice, deltaTime);
    }

    void TestShadowMap::OnRender(Graphics::GraphicsDevice& gdevice) {
        const Math::Matrix3D lightProj = Math::Matrix3D::perspective_fov(90.0f, gdevice.GetAspectRatio(), clipDistance.min, clipDistance.max),
                           lightView = Math::Matrix3D::look_at(lightPosition, 0, Math::fVector3::UP());

        {
            Graphics::Render::SetCullFace(Graphics::FacingMode::FRONT);
            depthMap.Bind();
            Graphics::Render::ClearDepthBit();
            scene.SetProjection(lightProj);
            scene.SetCamera(lightView);

            scene.Draw(meshes, UseShader(depthShader));
            depthMap.Unbind();
        }
        Graphics::Render::SetCullFace(Graphics::FacingMode::BACK);
        Graphics::Render::ClearColorBit();
        Graphics::Render::ClearDepthBit();
        if (showDepthMap) {
            shadowMapDisplay.Draw(screenQuad, Graphics::UseArgs({
                { "displayTex", depthTex },
                { "near", clipDistance.min.value() },
                { "far", clipDistance.max.value() }
            }, false));
        } else {
            scene.SetProjection(camera.GetProjMat());
            scene.SetCamera(camera.GetViewMat());

            scene.Draw(meshes, Graphics::UseArgs({
                { "lightSpaceMat", lightProj * lightView },
                { "depthMap", depthTex },
                { "lightPos", lightPosition },
                { "viewPos", camera.position },
                { "ambStrength", ambStrength },
                { "useSmoothShadows", useSmoothShadows }
            }));
        }
    }

    void TestShadowMap::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        ImGui::Checkbox("Show Depth Mag [Debug]", &showDepthMap);
        ImGui::Checkbox("Use Smooth Shading", &useSmoothShadows);
        ImGui::EditScalar("Ambient Strength", ambStrength, 0.05f);
        ImGui::EditRange("Clip Distance", clipDistance);
        ImGui::EditVector("Light Position", lightPosition, 0.2f);
        ImGui::EditCameraController("Camera", camera);
        // ImGui::Image((void*)(intptr_t)depthTex.rendererID, { 400.0f, 300.0f }, { 0, 1 }, { 1, 0 });
    }

    void TestShadowMap::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        scene.Destroy();
        shadowMapDisplay.Destroy();
        Graphics::Render::DisableCullFace();
    }
} // Test