﻿// because gl.h cant be included before glew.h, weird ordering
#include "Render.h"
#include "GraphicsDevice.h"

#include <algorithm>

#include "IO.h"
#include "Mouse.h"
#include "Keyboard.h"

#include <numeric>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Graphics {
    GraphicsDevice::GraphicsDevice(GLFWwindow* window, Maths::ivec2 winSize) : 
        windowSize(winSize), mainWindow{ window } {
        Instance = *this;
        IO::Init(*this);
    }

    void GraphicsDevice::Quit() {
        DeleteAllRenders(); // delete gl objects
        
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(mainWindow);
        glfwTerminate();

        mainWindow = nullptr; // mark as terminated
    }

    GraphicsDevice::~GraphicsDevice() {
        if (!IsClosed())
            Quit();
    }

    void GraphicsDevice::BeginRender() {
        if (IsClosed()) return;
        
        Render::Clear();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        SetRenderWireframe(useWireRender);

        IO::Mouse.Update();
    }

    void GraphicsDevice::EndRender() {
        if (IsClosed()) return;
        
        ImGui::Render();
        // glClear(GL_COLOR_BUFFER_BIT);
        
        glfwPollEvents();
            
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            
        glfwSwapBuffers(mainWindow);
    }
    
    void GraphicsDevice::BindRender(RenderData& render) {
        render.device = this;
        render.deviceIndex = renders.size() - 1;
    }

    void GraphicsDevice::DeleteRender(uint index) {
        renders[index].device = nullptr;
        renders.erase(renders.begin() + (int)index);
    }

    void GraphicsDevice::DeleteAllRenders() {
        std::ranges::for_each(renders, [](RenderData& r){ r.device = nullptr; });
        renders.clear();
    }

    RenderData& GraphicsDevice::GetRender(uint index) {
        return renders[index];
    }

    void GraphicsDevice::Render(RenderData& r) {
        r.EnableShader();
        Render::Draw(r, r.shader);
    }

    void GraphicsDevice::BindTexture(Texture& texture, uint slot) {
        texture.Bind(slot);
    }

    void GraphicsDevice::ClearColor(const Maths::fvec4& color) {
        Render::SetClearColor(color);
    }

    bool GraphicsDevice::WindowIsOpen() const {
        if (mainWindow) return !glfwWindowShouldClose(mainWindow);
        return false;
    }

    void GraphicsDevice::DebugMenu() {
        static bool enabled = false;
        if (ImGui::Button(enabled ? "Hide Debug Menu" : "Show Debug Menu")) enabled = !enabled;

        if (!enabled) goto skipDebugs; // i know goto is not great but its more readable imo  // NOLINT(cppcoreguidelines-avoid-goto, hicpp-avoid-goto)
        
        ImGui::Text("Application Averages %fms/frame (~%f FPS)", 1000.0 / (double)ImGui::GetIO().Framerate, (double)ImGui::GetIO().Framerate);
        if (ImGui::Button(useWireRender ? "Draw Fill" : "Draw Wireframe")) { useWireRender = !useWireRender; }

        if (ImGui::CollapsingHeader("Mouse Input")) {
            ImGui::Text("Mouse Position is at: (%f, %f),",
                IO::Mouse.GetMousePosPx().x,
                IO::Mouse.GetMousePosPx().y);
            ImGui::Text("   relative at: (%f, %f),",
                IO::Mouse.GetMousePos().x,
                IO::Mouse.GetMousePos().y);
            ImGui::Text("which is%s in the window", IO::Mouse.IsInWindow() ? "" : " not");
            ImGui::NewLine();
            
            ImGui::Text("Left Mouse Pressed: %s",   IO::Mouse.LeftPressed()   ? "True" : "False");
            ImGui::Text("Right Mouse Pressed: %s",  IO::Mouse.RightPressed()  ? "True" : "False");
            ImGui::Text("Middle Mouse Pressed: %s", IO::Mouse.MiddlePressed() ? "True" : "False");

            if (ImGui::TreeNode("Advanced")) {
                ImGui::Text("Pressed: ");
                for (int i = 0; i < IO::MouseT::LAST_MOUSE; ++i) {
                    if (!IO::Mouse.ButtonPressed(i)) continue;
                    ImGui::Text("   %s", IO::MouseT::MouseButtonToStr(i));
                }

                ImGui::TextDisabled("// The following below actually works, you just can't see it.");

                ImGui::Text("On Pressed: ");
                for (int i = 0; i < IO::MouseT::LAST_MOUSE; ++i) {
                    if (!IO::Mouse.ButtonOnPress(i)) continue;
                    ImGui::Text("   %s", IO::MouseT::MouseButtonToStr(i));
                }

                ImGui::Text("On Release: ");
                for (int i = 0; i < IO::MouseT::LAST_MOUSE; ++i) {
                    if (!IO::Mouse.ButtonOnRelease(i)) continue;
                    ImGui::Text("   %s", IO::MouseT::MouseButtonToStr(i));
                }
                
                ImGui::TreePop();
            }
        }

        if (ImGui::CollapsingHeader("Keyboard Input")) {
            ImGui::Text("Keys Pressed Are:");

            for (const auto key : IO::Keyboard.KeysPressed()) {
                ImGui::Text("   %s", IO::KeyboardT::KeyToStr(key));
            }
        }
        // ImGui::ShowDemoWindow();

        skipDebugs:
        if (ImGui::Button("Quit Application"))
            Quit();
    }

    GraphicsDevice GraphicsDevice::Initialize(Maths::ivec2 winSize) {
        /* Initialize the library */
        if (!glfwInit())
            ASSERT(false);

        /* Create a windowed mode window and its OpenGL context */
        GLFWwindow* window = glfwCreateWindow(winSize.x, winSize.y, "Hello World", nullptr, nullptr);
        
        if (!window) {
            glfwTerminate();
            ASSERT(false);
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        glfwSwapInterval(1);

        /* SETTING UP GLEW W/ GLEWINIT*/
        if (glewInit() != GLEW_OK) {
            LOG("ERR: ERROR SETTING UP glewInit()");
        }

        LOG(glGetString(GL_VERSION));

        GLCALL(glEnable(GL_BLEND));
        GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
        GLCALL(glEnable(GL_DEPTH_TEST));
        glDepthFunc(GL_LEQUAL);

        // IMGUI INIT
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init((const char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));
        
        return GraphicsDevice(window, winSize);
    }
}
