#version 120
uniform vec2 window;
uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camUp;
uniform vec3 camSide;
uniform vec3 julia;

float distest(vec3 pos)
{
    const int MAX_ITER = 200;
    const float BAILOUT= 100.0;
    const float power = 8.0;

    vec3 z = pos;
    float r=0.0;
    float dr=1.0;

    for(int n=0; n<=MAX_ITER; ++n)
    {
        r = length(z);
        if(r>BAILOUT) break;

        float theta = asin(z.z/r);
        float phi = atan(z.y, z.x);
        dr = pow(r,power-1.0)*power*dr+1.0;

        float zr = pow(r,power);
        theta = theta*power;
        phi = phi*power;

        z = (vec3(cos(theta)*cos(phi), sin(phi)*cos(theta), sin(theta))*zr)+pos;
    }
    return 0.5*log(r)*r/dr;
}

void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos;

    float d = 0.0, total_d = 0.0;
    float l = 0.0; //light
    int i = 0;
    for(; i < 30; ++i) {
        d = distest(ray);
        total_d += d;
        l += 1.0/(100000.0*d);
        ray += rayDir * d;
    }
    gl_FragColor = vec4(0.0, 0.0, (1-i/30.0) + l/2, 1.0 );
}


