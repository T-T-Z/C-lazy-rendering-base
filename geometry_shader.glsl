#version 330 core

layout(points) in;  // Input points (1 vertex per point)
layout(triangle_strip, max_vertices = 4) out;  // Output 4 vertices for each point

uniform mat4 view;  // Camera view matrix
uniform float size; // Size of the billboard

out vec4 fragPos; // The position of the point passed to the fragment shader
out vec4 fragColor; // Color to be passed to the fragment shader
out vec2 fragTexCoord;

void main() {
    vec3 Right = vec3(1,0,0);//view[0].xyz;//normalize(cross(view[1].xyz, cameraForward)); // Camera right direction
    vec3 Up = vec3(0,1,0);//view[1].xyz;//cross(cameraForward, cameraRight); // Camera up direction

    // Define the size of the billboard (screen space)
    float halfSize = 0.03 / 2.0f;

    // Generate the four vertices of the billboard quad
    fragPos = gl_in[0].gl_Position + vec4(Up * halfSize + Right * halfSize, 0.0);
    gl_Position = fragPos;//gl_in[0].gl_Position + vec4(Up * halfSize + Right * halfSize, 0.0);
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Red color
    fragTexCoord = vec2(1.0,1.0);
    EmitVertex();

    fragPos = gl_in[0].gl_Position + vec4(Up * halfSize - Right * halfSize, 0.0);
    gl_Position = fragPos;//gl_in[0].gl_Position + vec4(Up * halfSize - Right * halfSize, 0.0);
    fragColor = vec4(0.0, 1.0, 0.0, 1.0);  // Green color
    fragTexCoord = vec2(-1.0,1.0);
    EmitVertex();

    fragPos = gl_in[0].gl_Position + vec4(-Up * halfSize + Right * halfSize, 0.0);
    gl_Position = fragPos;//gl_in[0].gl_Position + vec4(-Up * halfSize + Right * halfSize, 0.0);
    fragColor = vec4(0.0, 0.0, 1.0, 1.0);  // Blue color
    fragTexCoord = vec2(1.0,-1.0);
    EmitVertex();

    fragPos = gl_in[0].gl_Position + vec4(-Up * halfSize - Right * halfSize, 0.0);
    gl_Position = fragPos;//gl_in[0].gl_Position + vec4(-Up * halfSize - Right * halfSize, 0.0);
    fragColor = vec4(1.0, 1.0, 0.0, 1.0);  // Yellow color
    fragTexCoord = vec2(-1.0,-1.0);
    EmitVertex();

    EndPrimitive();
}
