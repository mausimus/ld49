#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec4 colDiffuse;
uniform int axis;
uniform float xs;
uniform float ys;

// Output fragment color
const float r = 16.0;
const float rr = 256.0;
const float oneover2rr = 0.001953125;
const float boost = 0.75;
const float w0 = 0.035857934;

vec4 gauss(int axis)
{
        vec2 pos = fragTexCoord;

        float d,w;
        vec2 p=pos;
        vec4 col=vec4(0.0,0.0,0.0,0.0);
        if (axis==0)
        {
            d = xs;
            p.x+=-r*d;
            for (float x=-r;x<=r;x++)
            {
                w=w0*exp((-x*x)*oneover2rr);
                col+=texture2D(texture0,p)*w;
                p.x+=d;
            }
        }
        if (axis==1)
        {
            d = ys;
            p.y+=-r*d;
            for (float y=-r;y<=r;y++)
            {
                w=w0*exp((-y*y)*oneover2rr);
                col+=texture2D(texture0,p)*w;
                p.y+=d;
            }
        }
        return col;
}

void main()
    {
        gl_FragColor = gauss(axis);
        if(axis == 1)
        {
            vec4 texValue = texture2D(texture1, fragTexCoord);
            gl_FragColor *= boost;
            gl_FragColor = vec4(max(gl_FragColor.r, texValue.r), max(gl_FragColor.g, texValue.g), max(gl_FragColor.b, texValue.b), 1.0);
        }
    }