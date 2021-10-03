#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec4 colDiffuse;
uniform int axis;
uniform float xs; // inverted now
uniform float ys;

// Output fragment color
out vec4 finalColor;

const float r = 16;
const float rr = 256;
const float oneover2rr = 0.001953125;
const float boost = 0.75;
const float w0 = 0.035857934;

vec4 gauss(int axis)
{
        vec2 pos = fragTexCoord;

        float x,y,d,w;
        vec2 p=pos;
        vec4 col=vec4(0.0,0.0,0.0,0.0);
        if (axis==0) for (d=xs,x=-r,p.x+=x*d;x<=r;x++,p.x+=d){ w=w0*exp((-x*x)*oneover2rr); col+=texture(texture0,p)*w; }
        if (axis==1) for (d=ys,y=-r,p.y+=y*d;y<=r;y++,p.y+=d){ w=w0*exp((-y*y)*oneover2rr); col+=texture(texture0,p)*w; }
        return col;
}

// org
/*
vec4 gauss(int axis)
{
        vec2 pos = fragTexCoord;

        float x,y,rr=r*r,d,w,w0;
        vec2 p=pos;
        vec4 col=vec4(0.0,0.0,0.0,0.0);
        w0=0.5135/pow(r,0.96);
        if (axis==0) for (d=1.0/xs,x=-r,p.x+=x*d;x<=r;x++,p.x+=d){ w=w0*exp((-x*x)/(2.0*rr)); col+=texture(texture0,p)*w; }
        if (axis==1) for (d=1.0/ys,y=-r,p.y+=y*d;y<=r;y++,p.y+=d){ w=w0*exp((-y*y)/(2.0*rr)); col+=texture(texture0,p)*w; }
        return col;
}
*/

void main()
    {
        finalColor = gauss(axis);
        if(axis == 1)
        {
            vec4 texValue = texture(texture1, fragTexCoord);
            finalColor *= boost;
            finalColor = vec4(max(finalColor.r, texValue.r), max(finalColor.g, texValue.g), max(finalColor.b, texValue.b), 1);
        }
    }
