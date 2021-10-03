#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in float lineLen;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

void main()
{
    float edgeDist = lineLen;
    float edgeAdj = abs(fragTexCoord.y) > (1 - edgeDist) ? (1.0 - ((abs(fragTexCoord.y) - (1 - edgeDist)) / edgeDist)) : 1.0;

    finalColor = fragColor * (1 - abs(fragTexCoord.x)) * min(edgeAdj, 1);
}
