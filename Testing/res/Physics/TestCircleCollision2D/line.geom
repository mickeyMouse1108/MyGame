#version 330 core

layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

flat out int colorID;

void main() {
    colorID = gl_PrimitiveIDIn;
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    EmitVertex();
    EndPrimitive();
}
