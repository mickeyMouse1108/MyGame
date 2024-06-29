#include "TestCircleCollision2D.h"

#include <imgui.h>

#include "Meshes/Circle.h"

namespace Test {
    void TestCircleCollision2D::OnInit(Graphics::GraphicsDevice& gdevice) {
        scene = gdevice.CreateNewRender<Vertex>();
        viewport = { 0, 80, 0, 60 };

        using namespace Graphics::VertexBuilder;
        circleMesh = Graphics::MeshUtils::Circle({ 32 }, Vertex::Blueprint {
            .Position = GetPosition {}
        });

        scene.BindMesh(circleMesh);

        scene.UseShaderFromFile(res("circle.vert"), res("circle.frag"));
        lineShader = Graphics::Shader::FromFile(res("line.vert"), res("line.frag"), res("line.geom"));

        scene.SetProjection(Math::Matrix3D::ortho_projection({ 0, 80, 0, 60, -1, 1 }));

        world = { { 0, -40.0f }, Physics2D::Drag(0.98f, 60.0f) };
        ResetBalls(gdevice);

        auto& vert = totalLineMesh.vertices; auto& inds = totalLineMesh.indices;
        for (int i = 0; i < 6; ++i) vert.emplace_back(0.0f);
        inds.emplace_back(0, 1, 1);
        inds.emplace_back(1, 3, 3);
        inds.emplace_back(2, 3, 3);
        inds.emplace_back(0, 2, 2);
        inds.emplace_back(4, 5, 5);
    }

    void TestCircleCollision2D::OnUpdate(Graphics::GraphicsDevice& gdevice, float deltaTime) {
        const auto& mouse = gdevice.GetIO().Mouse;
        const Math::fVector2 mousePos = mouse.GetMousePos().as<float>().map({ -1, 1, 1, -1 }, viewport);
        if (mouse.LeftOnPress() && !selected) {
            selected = FindBallAt(mousePos);
            if (selected)
                selectOffset = mousePos - selected->position;
        }

        if (mouse.LeftPressed() && selected) {
            const Math::fVector2 newPos = mousePos - selectOffset;
            selected->position = newPos;
            selected->velocity = 0;
        }

        if (mouse.LeftOnRelease()) selected = nullptr;

        if (mouse.MiddleOnPress()) lastDragPosition = mousePos;
        if (mouse.MiddlePressed()) {
            for (auto circ : world.bodies) circ->position -= lastDragPosition - mousePos;
            lastDragPosition = mousePos;
        }

        if (mouse.RightOnPress() && !selected) {
            selected = FindBallAt(mousePos);
            selected = selected && selected->IsDynamic() ? selected : nullptr;
        }

        if (mouse.RightPressed() && selected) {
            totalLineMesh.vertices[4].Position = selected->position;
            totalLineMesh.vertices[5].Position = mousePos;
        }

        if (mouse.RightOnRelease() && selected && selected->IsDynamic()) {
            const bool scale = gdevice.GetIO().Keyboard.KeyPressed(IO::Key::LCONTROL);
            selected->velocity -= (scale ? 10.0f : 1.0f) * (mousePos - selected->position);
            totalLineMesh.vertices[4].Position = 0;
            totalLineMesh.vertices[5].Position = 0;
            selected = nullptr;
        }

        world.Update(deltaTime, 8, 2);

        for (int i = 0; i < 4; ++i) {
            Ref<const Physics2D::Body> e = edge[i];
            totalLineMesh.vertices[i].Position = e->position + e->shape.As<Physics2D::EdgeShape>()->start;
        }

        lineShader.Bind();
    }

    void TestCircleCollision2D::OnRender(Graphics::GraphicsDevice& gdevice) {
        scene->shader.Bind();
        Math::fVector2 offsets[TOTAL_BALL_COUNT];
        float scales[TOTAL_BALL_COUNT];
        Math::fColor colors[TOTAL_BALL_COUNT];

        Math::fRange xRange = {
            world.bodies.back ()->ComputeBoundingBox().max.x,
            world.bodies.front()->ComputeBoundingBox().min.x
        };
        usize i = 0;
        int selectedIndex = -1;
        for (const auto body : world.bodies) {
            if (body->shape.As<Physics2D::EdgeShape>()) continue;
            if (selectedIndex == -1 && body.RefEquals(selected)) selectedIndex = (int)i;
            offsets[i] = body->position;
            scales[i]  = body->shape.As<Physics2D::CircleShape>()->radius;
            colors[i]  = Math::fColor::from_hsv(
                Math::Unit { offsets[i].x }.map(xRange, { 0, 360.f }).value(),
                body->IsStatic() ? 0.2f : 0.8f,
                body->IsStatic() ? 0.2f : 0.8f);
            ++i;
        }

        scene.ResetData();
        scene.RenderInstanced(TOTAL_BALL_COUNT, {
            { "u_projection", scene->projection },
            { "selected",     selectedIndex },
            { "offsets",      offsets },
            { "scales",       scales },
            { "colors",       colors }
        }, false);

        scene.ClearData();
        scene.AddNewMesh(totalLineMesh);
        scene.Render(lineShader, {
            { "u_projection", scene->projection }
        }, false);
    }

    void TestCircleCollision2D::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        if (ImGui::Button("Reset Balls")) ResetBalls(gdevice);

        ImGui::BulletText("Controls");
        ImGui::Text("Left Click = Drag");
        ImGui::Text("Right Click = Add Velocity");
        ImGui::Text(" + Control for increased Force");
        ImGui::Text("Middle Click = Move");

        ImGui::DragFloat2("Gravity", world.gravity.begin());
        ImGui::DragFloat("Inverse Drag", &world.drag, 0.005f, 0.0f, 1.0f);
    }

    void TestCircleCollision2D::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        scene.Destroy();
    }

    void TestCircleCollision2D::AddRandomBall(Graphics::GraphicsDevice& gdevice) {
        auto& rand = gdevice.GetRand();

        const Math::fVector2 position = Math::fVector2::random(rand, viewport);
        const float radius = rand.Get(1.0f, 3.0f);
        world.CreateBody<Physics2D::CircleShape>({ .position = position, .density = 5.0f }, radius);
    }

    void TestCircleCollision2D::ResetBalls(Graphics::GraphicsDevice& gdevice) {
        world.Clear();

        for (u32 i = 0; i < DYNAMIC_BALL_COUNT; ++i)
            AddRandomBall(gdevice);

        using namespace Physics2D;
        auto& rand = gdevice.GetRand();

        for (u32 i = 0; i < STATIC_BALL_COUNT; ++i) {
            world.CreateBody<CircleShape>({
                .position = Math::fVector2::random(rand, viewport),
                .type = BodyType::STATIC,
                .density = 0.0f },
            rand.Get(4.0f, 6.0f));
        }

        edge[0] = world.CreateBody<EdgeShape>({ .type = BodyType::STATIC, .density = 0.0f }, viewport.corner(0), viewport.corner(1));
        edge[1] = world.CreateBody<EdgeShape>({ .type = BodyType::STATIC, .density = 0.0f }, viewport.corner(1), viewport.corner(3));
        edge[2] = world.CreateBody<EdgeShape>({ .type = BodyType::STATIC, .density = 0.0f }, viewport.corner(2), viewport.corner(0));
        edge[3] = world.CreateBody<EdgeShape>({ .type = BodyType::STATIC, .density = 0.0f }, viewport.corner(3), viewport.corner(2));
    }

    Ref<Physics2D::Body> TestCircleCollision2D::FindBallAt(const Math::fVector2& mousePos) const {
        for (auto circ : world.bodies) {
            if (circ->CollidesWith(Physics2D::CircleShape { 0.0f }, mousePos)) {
                return circ;
            }
        }
        return nullptr;
    }
} // Test