#include "TestTexturedSquare.h"

#include "imgui.h"

namespace Test {
    void TestTexturedSquare::OnInit(Graphics::GraphicsDevice& gdevice) {
        render = gdevice.CreateNewRender<VertexColorTexture3D>(4, 2);

        render.UseShader(Graphics::Shader::StdTextured);
        render.SetProjection(projection);

#pragma region Texture Define
        // see testbatchedtextured.cpp
        uchar tex[] = {
            0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
            0x00,0x00,0x00,0x20,0x00,0x00,0x00,0x20,0x08,0x02,0x00,0x00,0x00,0xfc,0x18,0xed,
            0xa3,0x00,0x00,0x00,0x01,0x73,0x52,0x47,0x42,0x00,0xae,0xce,0x1c,0xe9,0x00,0x00,
            0x00,0x04,0x67,0x41,0x4d,0x41,0x00,0x00,0xb1,0x8f,0x0b,0xfc,0x61,0x05,0x00,0x00,
            0x00,0x09,0x70,0x48,0x59,0x73,0x00,0x00,0x0e,0xc3,0x00,0x00,0x0e,0xc3,0x01,0xc7,
            0x6f,0xa8,0x64,0x00,0x00,0x03,0x3d,0x49,0x44,0x41,0x54,0x48,0x4b,0x65,0x56,0x09,
            0x6e,0xc3,0x30,0x0c,0xb3,0x32,0xec,0xea,0xff,0xbf,0xba,0xb5,0xdb,0x5a,0x78,0x3c,
            0x24,0xc5,0x49,0x01,0x42,0xa0,0x65,0x87,0xb4,0xe4,0xa6,0x71,0xc4,0x63,0xbc,0x8e,
            0x78,0x1d,0xdb,0xdb,0x88,0xb7,0xb1,0xbd,0x8f,0x78,0x1f,0xdb,0xc7,0x78,0xf9,0x8c,
            0xed,0x63,0x6e,0x9f,0x83,0xb8,0x60,0x48,0xf2,0x72,0x01,0x0f,0xc5,0xc9,0x05,0x17,
            0x2c,0x08,0x4d,0x4d,0x3c,0x92,0x78,0x0f,0x48,0x51,0x8d,0xb2,0x73,0xdb,0xc6,0xdf,
            0x20,0xee,0x8d,0x10,0xd6,0xa4,0x86,0xf7,0x11,0x02,0xf2,0x81,0xa9,0x47,0xce,0x66,
            0x12,0xc3,0x86,0xf2,0x42,0x60,0x58,0x06,0xc0,0x2f,0x31,0x7b,0xb8,0x63,0x0a,0x98,
            0x9a,0x7a,0x8c,0x91,0x1c,0x53,0x22,0x82,0x57,0x86,0x93,0x04,0x09,0x57,0x6e,0xd6,
            0x1d,0x3f,0xa9,0x1e,0x1c,0xce,0x82,0xd6,0x31,0xf2,0xc9,0xf8,0x8b,0xf9,0x8b,0x05,
            0x8c,0x92,0x23,0x58,0x4d,0xe9,0x86,0x14,0xcd,0x9b,0xd8,0x20,0x14,0x65,0xf3,0x33,
            0x6d,0x96,0xc9,0xe4,0xc6,0x94,0x9c,0xf6,0x5e,0xc9,0x99,0xde,0xe6,0x12,0xf5,0x2c,
            0x0d,0x38,0xb5,0x49,0x34,0x11,0x54,0x87,0x6e,0x30,0x1e,0xa4,0x57,0xd0,0x58,0x4e,
            0x44,0x74,0xa1,0x59,0xfd,0x02,0x39,0x6d,0xe3,0x36,0x1a,0x93,0xea,0x2e,0xc2,0x50,
            0xbe,0xe3,0xcf,0x98,0x24,0xca,0x73,0x37,0x7c,0x7e,0x76,0xf5,0xe4,0x29,0xca,0x2a,
            0x2b,0x69,0x83,0xb9,0x0b,0x5d,0x15,0x61,0x76,0x8b,0x4c,0xee,0x08,0xee,0x31,0x37,
            0x81,0xdd,0x30,0xd2,0xa6,0x89,0x45,0xed,0x51,0xb0,0x01,0xb4,0xe8,0x11,0xe4,0x32,
            0x5b,0x88,0x92,0x32,0xc6,0xde,0x41,0x32,0xa6,0x34,0x6d,0x20,0x04,0xa2,0xe8,0x29,
            0x35,0xb9,0x0d,0xf0,0xe4,0x75,0x8e,0x6b,0x88,0x60,0xc5,0x42,0x24,0x8d,0xb6,0xa8,
            0x33,0x59,0x1c,0x2b,0xa3,0x90,0x23,0xf2,0xd9,0x31,0x20,0x8b,0xf0,0xb0,0x88,0x0d,
            0x88,0x99,0x04,0x66,0x7a,0x12,0x10,0x89,0x1e,0xa2,0x39,0x26,0x44,0xd6,0x91,0xfb,
            0x20,0x49,0xc5,0x03,0x67,0x05,0xdf,0x69,0x50,0x08,0x2a,0x76,0x41,0x30,0xa6,0x9c,
            0xb0,0x72,0xab,0x90,0x54,0x4b,0x4f,0xd2,0x35,0x94,0x41,0x03,0x8a,0xdf,0x53,0xa5,
            0xa8,0x8e,0x05,0x72,0x75,0x94,0x22,0x88,0x75,0xb1,0x15,0x09,0xf9,0xa8,0x70,0x24,
            0xd9,0x4f,0xc7,0xb5,0x45,0x04,0x3d,0x5c,0x01,0x9d,0x3a,0x9f,0x19,0x49,0xc4,0x4d,
            0x0b,0xc4,0x53,0x45,0xf1,0x5c,0x1c,0xd5,0x79,0x3c,0xcf,0x15,0x8c,0x49,0x8e,0x16,
            0xed,0x5d,0x02,0x5c,0x16,0x22,0x49,0x6b,0x35,0xc7,0x26,0xfc,0x5b,0x30,0xec,0x21,
            0x22,0x83,0x2f,0x69,0xb5,0x87,0xb4,0xe2,0xa9,0x4b,0x59,0x1c,0x8e,0x57,0xc3,0x74,
            0xf2,0xd4,0x2a,0x6a,0xd4,0xd0,0x87,0x3c,0x53,0xda,0x1e,0x27,0x58,0xa2,0x84,0xe0,
            0x71,0x28,0xc2,0xe8,0xb6,0x74,0x2c,0xa2,0x33,0xb0,0xd0,0x57,0x2b,0xba,0xa0,0x6a,
            0xd1,0xd1,0x03,0xea,0x3c,0x4f,0x0f,0x2d,0xe4,0x61,0xbf,0x2e,0x1d,0x45,0x96,0x33,
            0x48,0x09,0x55,0xc3,0x61,0x95,0x95,0x07,0x20,0x09,0xf7,0xba,0x86,0x40,0xd4,0x0b,
            0x0f,0xf5,0x7c,0xe7,0x4d,0xe0,0xa1,0xf7,0xc6,0x06,0x81,0x18,0xd6,0x25,0x77,0x97,
            0x0f,0x87,0x2c,0x4c,0xed,0x7d,0x79,0xdd,0xe0,0xbd,0xe8,0x12,0xd4,0xed,0x61,0xff,
            0x8a,0x9e,0xcf,0x13,0xbf,0xa5,0x65,0x58,0xbf,0x4b,0x5a,0x4e,0x1f,0x72,0x4a,0x28,
            0x89,0x78,0xec,0x8c,0xff,0x07,0xfd,0x9f,0xbf,0x57,0xd0,0x72,0x85,0xd3,0x0b,0x75,
            0xce,0xd0,0x75,0xd5,0x2d,0x75,0x90,0xfe,0x8b,0x05,0xf4,0x6f,0xea,0x27,0x77,0x0f,
            0x6e,0x2a,0xdc,0x22,0xce,0x12,0xea,0x43,0xf0,0x0c,0x4a,0x88,0xdb,0x04,0x79,0x7a,
            0x75,0xb3,0x63,0xe2,0x80,0x7e,0x45,0x18,0x57,0x9c,0x34,0x73,0x13,0x0e,0xfb,0x85,
            0x0a,0x9b,0x43,0xa1,0xf0,0x06,0xf7,0xd6,0x97,0x56,0x42,0xff,0xd2,0x3d,0x3c,0xfe,
            0x55,0xf0,0xc4,0xb0,0x71,0x1f,0x63,0x1d,0x26,0xd6,0x89,0x40,0x51,0xa2,0xb6,0x39,
            0xa8,0x10,0x1e,0x66,0xf2,0xfc,0x45,0x3b,0xa2,0xff,0x20,0xc9,0x2b,0x12,0x3c,0x34,
            0x7e,0x56,0x91,0xe9,0x4f,0x4a,0x4e,0x09,0xe5,0xc1,0xef,0x28,0x96,0x69,0xa8,0x8f,
            0xfe,0x49,0x6b,0x57,0x3c,0x81,0x7b,0xc7,0x57,0xde,0x1f,0xaf,0xfc,0xa9,0x68,0x9b,
            0x0d,0x5f,0x2f,0x88,0xfc,0x38,0xbb,0x02,0xae,0x53,0xe1,0xfd,0x4c,0xf3,0x06,0x1f,
            0xdb,0xbf,0xef,0x0b,0xd8,0x0d,0x5a,0xae,0xb3,0x54,0xcf,0xdb,0x4d,0x5f,0xbc,0x72,
            0x7a,0xda,0x89,0xb7,0xab,0xbd,0x8f,0x05,0xee,0xdd,0xd7,0x88,0x9c,0x85,0x84,0x70,
            0xb8,0x2f,0x55,0xd2,0x1e,0x65,0x00,0x68,0x9b,0x91,0x4e,0x13,0x37,0xb8,0xcc,0xe7,
            0xea,0xe4,0xbc,0x08,0x51,0x51,0xb7,0x39,0x66,0x48,0x74,0xe5,0x02,0xc9,0x5b,0x2c,
            0xa7,0x4c,0x78,0xb3,0xcb,0x75,0xcf,0xf0,0x4d,0x74,0xc9,0xe0,0x32,0xdb,0x71,0x9f,
            0x25,0xe1,0xfd,0x97,0xf7,0xc6,0x4a,0x32,0x9f,0x44,0x06,0x3d,0x66,0xd4,0x22,0x92,
            0xda,0x9a,0xd6,0x19,0xbe,0x5d,0x8b,0xd7,0x95,0xfb,0x51,0xbb,0x6e,0x58,0xc4,0xd7,
            0xec,0xfb,0xf8,0x07,0x05,0x62,0x5d,0x32,0x60,0x16,0x00,0x28,0x00,0x00,0x00,0x00,
            0x49,0x45,0x4e,0x44,0xae,0x42,0x60,0x82
        };
#pragma endregion
        
        texture = Graphics::Texture::LoadPNGBytes(tex, sizeof(tex) / sizeof(uchar), false);
        gdevice.BindTexture(texture);

        VertexColorTexture3D vertices[] = { 
            { { -50.0f, -50.0f, 0 }, 1, { 0.0f, 0.0f }, texture.Slot() },
            { { +50.0f, -50.0f, 0 }, 1, { 1.0f, 0.0f }, texture.Slot() },
            { { +50.0f, +50.0f, 0 }, 1, { 1.0f, 1.0f }, texture.Slot() },
            { { -50.0f, +50.0f, 0 }, 1, { 0.0f, 1.0f }, texture.Slot() },
        };

        Graphics::TriIndices indices[] = {
            { 0, 1, 2 },
            { 2, 3, 0 }
        };

        mesh = Graphics::Mesh(
            std::vector(vertices, vertices + 4),
            std::vector(indices, indices + 2)
        );

        render.BindMeshes(&mesh, 1);
    }

    void TestTexturedSquare::OnRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnRender(gdevice);
        Maths::mat3D mat = Maths::mat3D::transform(modelTranslation,
                                                   modelScale,
                                                   modelRotation);
        render.SetCamera(mat);
        mesh.ApplyMaterial(&VertexColorTexture3D::Color, color);
        //LOG(mat);
        render.ResetData();
        render.Render();
    }

    void TestTexturedSquare::OnImGuiRender(Graphics::GraphicsDevice& gdevice) {
        Test::OnImGuiRender(gdevice);

        ImGui::ColorEdit4("Texture Color", color.begin());
        ImGui::DragFloat3("Translation", modelTranslation.begin());
        ImGui::DragFloat3("Scale",       modelScale.begin(), 0.1f);
        ImGui::DragFloat3("Rotation",    modelRotation.begin(), 0.03f);
    }

    void TestTexturedSquare::OnDestroy(Graphics::GraphicsDevice& gdevice) {
        Test::OnDestroy(gdevice);
        render.Destroy();
    }
}
