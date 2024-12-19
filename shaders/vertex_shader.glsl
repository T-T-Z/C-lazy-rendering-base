#version 330 core

layout(location = 0) in vec3 aPosition;  // Position of the vertices

uniform mat4 view;         // The view transformation matrix
uniform mat4 projection;   // The projection transformation matrix

out vec2 uv;  // UV coordinates to pass to the fragment shader

void main() {
    // Pass the UV coordinates to the fragment shader (transforming the vertex)
    uv = aPosition.xy * 0.5 + 0.5;  // Transform coordinates to [0, 1] range
    
    // Apply the view, and projection transformations
    gl_Position = projection * view * vec4(aPosition, 1.0);
}