#version 330 core

out vec4 FragColor;


uniform mat4 projection;  // Camera view matrix
uniform mat4 view;  // Camera view matrix
uniform vec3 cameraPos;

in vec4 fragPos;
in vec4 fragColor;  // Color passed from the geometry shader
in vec2 fragTexCoord;

void main() {
    if(length(fragTexCoord)>1){
        discard;
    }
    FragColor = fragColor;

    // Get the position of the fragment
    vec3 pointPos = fragPos.xyz;

    // Compute the inverse of projection * view
    mat4 inverseProjectionView = inverse(projection * view);

    // Transform the screen position back to world space
    vec4 worldSpacePosition = inverseProjectionView * fragPos;

    // Calculate the distance from the camera to the fragment's position
    float distToCamera = length(cameraPos - worldSpacePosition.xyz);

    //gl_FragDepth = distToCamera;

}