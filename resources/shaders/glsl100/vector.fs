#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;
varying float lineLen;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
//out vec4 finalColor;

void main()
{
    // Texel color fetching from texture sampler
    //vec4 texelColor = texture(texture0, fragTexCoord);
    //vec4 lineColor = texelColor*colDiffuse;
    
    //float edgeAdj = abs(fragTexCoord.y) > 0.75 ? (1.0 - ((abs(fragTexCoord.y) - 0.75) / 0.25)) : 1.0;    
    //float edgeDist = 0;//0.01 / lineLen;//0.25 / lineLen; // clamp(lineLen, 0, 1);
    //float edgeAdj = abs(fragTexCoord.y) > (1 - edgeDist) ? (1.0 - ((abs(fragTexCoord.y) - (1 - edgeDist)) / edgeDist)) : 1.0;

    //vec4 finalColor = fragColor * (1.0 - abs(fragTexCoord.x));// * min(edgeAdj, 1);
    //gl_FragColor = vec4(finalColor.xyz, 0.85);

    float edgeDist = lineLen;
    float edgeAdj = abs(fragTexCoord.y) > (1.0 - edgeDist) ? (1.0 - ((abs(fragTexCoord.y) - (1.0 - edgeDist)) / edgeDist)) : 1.0;

    gl_FragColor = fragColor * (1.0 - abs(fragTexCoord.x)) * min(edgeAdj, 1.0);    

    //gl_FragColor = vec4(1, 1, 1, 1);
    //fragColor = vec4(1,0,0,1);//colDiffuse;
}
