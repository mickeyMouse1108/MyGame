#include "DemoFlappyBird.h"

#include "imgui.h"
#include "Keyboard.h"
#include "Quad.h"
#include "Random.h"
#include "Tri.h"
#include "MeshUtils.h"

namespace Test {
    void DemoFlappyBird::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<Vertex>(128, 128);
        render.UseShaderFromFile(res("shader.vert"), res("shader.frag"));
        render.SetProjection(Maths::mat3D::ortho_projection({ -320.0f, 320.0f, -240.0f, 240.0f, -1.0f, 1.0f }));

        font = Graphics::Font::LoadFile(res("consola.ttf"));
        font.SetSize(48);
        font.RenderBitmap();
        font.GetTexture().Activate(0);

        mPlayer = Graphics::MeshUtils::SimpleCircleMesh(32,
            [](const Maths::fvec2& pos, uint) {
                 return Vertex { pos, 1, 0, 0 };
        }, Maths::mat3D::scale_mat(30.0f));
        using Graphics::Primitives::Quad;
        mBg = Quad { { 0, 240, 0 }, { 320, 0, 0 }, { 0, 20, 0 } }
             .IntoMesh<Vertex>([](Maths::fvec3 v) -> Maths::fvec2 { return v.xy(); }, &Vertex::Position)
             .Add(
                 Quad { { 0, -240, 0 }, { 320, 0, 0 }, { 0, 20, 0 } }
                .IntoMesh<Vertex>([](Maths::fvec3 v) -> Maths::fvec2 { return v.xy(); }, &Vertex::Position))
             .ApplyMaterial(&Vertex::Color, Maths::colorf::BETTER_GREEN());

        render.BindMeshes({ &mBg, &mPlayer });

        time = gdevice.GetIO().Time.currentTime;
        nextSpawnTime = 0;
        Graphics::Render::SetClearColor(Maths::colorf::BETTER_BLACK());
    }

    void DemoFlappyBird::OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) {
        if (!isEnd) {
            if (gdevice.GetIO().Keyboard.KeyOnPress(IO::Key::SPACE)) {
                velocityY = 250;
            }

            yPos += velocityY * deltaTime;
            velocityY -= 400 * deltaTime;
            score = (int)(gdevice.GetIO().Time.currentTime - time);
        }

        ManageSpikes(gdevice);
        CheckPlayerCollisions();
    }

    void DemoFlappyBird::OnRender(Graphics::GraphicsDevice& gdevice) {
        render.GetShader().Bind();
        render.GetShader().SetUniformTex("u_font", font.GetTexture());

        using namespace Graphics;
        mPlayer.SetTransform(Maths::mat3D::translate_mat({ -150, yPos, 0 }));
        mText.Replace(font.RenderText(std::to_string(score), 80,
            TextAlign { { -20, 20, 100, 140 } }.SpaceOut(1, -16))
            .Convert<Vertex>([](const Font::Vertex& v) -> Vertex {
                return { v.Position, v.Color, v.TextureCoord, 1 };
            }));

        render.ClearData();
        render.AddBoundMeshes();
        for (const Spike& spike : spikes) render.AddNewMeshes(spike.mesh);
        render.AddNewMeshes(mText);
        render.Render();
    }

    void DemoFlappyBird::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        ImGui::Text("Spike Count: %d", spikes.size());
    }

    void DemoFlappyBird::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        render.Destroy();
        Graphics::Render::SetClearColor(0);
    }

    void DemoFlappyBird::ManageSpikes(Graphics::GraphicsDevice& gdevice) {
        if (isEnd) return;
        nextSpawnTime += gdevice.GetIO().Time.deltaTime;
        if (nextSpawnTime > 2) {
            nextSpawnTime -= 2;
            const float midY = gdevice.GetRand().getf(-100.0f, 100.0f);
            Maths::Geometry::ftriangle2d top = { { -50,  220 }, { 50,  220 }, { gdevice.GetRand().getf(-15.0f, 15.0f), midY + 90 } };
            Maths::Geometry::ftriangle2d bot = { { -50, -220 }, { 50, -220 }, { gdevice.GetRand().getf(-15.0f, 15.0f), midY - 90 } };

            spikes.emplace_back(
                Graphics::Primitives::Tri::FromGeometry(top)
               .IntoMesh<Vertex>(
                   [](const Maths::fvec3& v) -> Maths::fvec2 { return v.xy(); },
                   &Vertex::Position)
               .ApplyMaterial(&Vertex::Color, Maths::colorf::BETTER_LIME()),
                top, 370.0f);
            spikes.emplace_back(
                Graphics::Primitives::Tri::FromGeometry(bot)
               .IntoMesh<Vertex>(
                   [](const Maths::fvec3& v) -> Maths::fvec2 { return v.xy(); },
                   &Vertex::Position)
               .ApplyMaterial(&Vertex::Color, Maths::colorf::BETTER_AQUA()),
                bot, 370.0f);
        }

        for (Spike& spike : spikes) {
            spike.xOff -= 150 * gdevice.GetIO().Time.DeltaTimef();
            spike.mesh.SetTransform(Maths::mat3D::translate_mat({ spike.xOff, 0, 0 }));
        }

        if (!spikes.empty() && spikes.front().xOff <= -370.0f) {
            spikes.pop_front();
            spikes.pop_front();
        }
    }

    void DemoFlappyBird::CheckPlayerCollisions() {
        if (yPos <= -190.0f || 190.0f <= yPos) goto die; // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
        for (const Spike& spike : spikes) {
            for (int i = 1; i < 3; ++i) {
                const Maths::Geometry::fline2d line = spike.collider.line(i) + Maths::fvec2::unit_x(spike.xOff);
                const Maths::fvec2 center           = Maths::fvec2 { -150, yPos },
                                   toCenter         = center - line.start,
                                   dist             = line.as_vec(),
                                   proj             = toCenter.project(dist);
                const float factor                  = std::abs(dist.x) > std::abs(dist.y) ? proj.x / dist.x : proj.y / dist.y;
                if (center.distsq(factor <= 0 ? line.start : (factor >= 1 ? line.end : proj + line.start)) <= 30.0f * 30.0f)
                    goto die;
            }
        }
        return;

        die:
        isEnd = true;
        mPlayer.ApplyMaterial(&Vertex::Color, Maths::colorf::BETTER_RED());
    }
}
