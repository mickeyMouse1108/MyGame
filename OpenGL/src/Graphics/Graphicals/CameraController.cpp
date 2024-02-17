#include "CameraController.h"
#include "GraphicsDevice.h"

#include "imgui.h"

namespace Graphics {
    void CameraController::Update(GraphicsDevice& gd, const float dt) {
        using namespace Maths;
        using namespace IO;
        const auto& Keyboard = gd.GetIO().Keyboard;
        auto& Mouse = gd.GetIO().Mouse;
        if(Keyboard.KeyOnPress(Key::ESCAPE)) Toggle(gd);

        if (lock) return;
        const mat3D rotation = mat3D::rotate_y(rot.y);
        if (Keyboard.KeyPressed(Key::W))      trans += rotation * fvec3::FRONT() * -speed * dt;
        if (Keyboard.KeyPressed(Key::S))      trans += rotation * fvec3::BACK()  * -speed * dt;
        if (Keyboard.KeyPressed(Key::D))      trans += rotation * fvec3::LEFT()  *  speed * dt;
        if (Keyboard.KeyPressed(Key::A))      trans += rotation * fvec3::RIGHT() *  speed * dt;
        if (Keyboard.KeyPressed(Key::SPACE))  trans += fvec3::UP()    * -speed * dt;
        if (Keyboard.KeyPressed(Key::LSHIFT)) trans += fvec3::DOWN()  * -speed * dt;

        rot = Mouse.GetMousePosPx().perpend() * -sensitivity * dt;
    }

    void CameraController::Toggle(GraphicsDevice& gd) {
        lock ^= true;
        if (lock) {
            gd.GetIO().Mouse.Lock();
        } else {
            gd.GetIO().Mouse.Show();
        }
    }

    Maths::mat3D CameraController::GetViewMat() const {
        return Maths::mat3D::transform(trans, scale, rot).inv();
    }

    void CameraController::ImGuiEdit(const char* const title) {
        ImGui::Separator();
        ImGui::Text("%s", title);
        ImGui::SliderFloat3("Rot", rot.begin(),   -3.14f, 3.14f);
        ImGui::SliderFloat3("Sca", scale.begin(),  0,     3    );
        ImGui::SliderFloat3("Tra", trans.begin(), -10,    10   );
        ImGui::SliderFloat("Speed", &speed, 0, 1);
        ImGui::SliderFloat("Sensitivity", &sensitivity, 0, 1);
    }
}
