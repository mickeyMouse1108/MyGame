#version 330 core

layout (location = 0) out vec4 glColor;

in vec2 vTexCoord;

uniform sampler2D displayTex;
uniform float near, far;

void main() {
    float z = texture(displayTex, vTexCoord).r * 2.0 - 1.0;
    float depth = 2 * near / ((far + near) - z * (far - near));

    glColor = vec4(vec3(depth), 1.0);
}
