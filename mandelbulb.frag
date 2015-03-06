#version 120
uniform vec2 window;
uniform float power;

float distest(vec3 pos)
{
    const int MAX_ITER = 10;
    const float BAILOUT= 4.0;

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


void main( void )
{
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 camPos = vec3(0.0, 0.0, 7.0 - pow(power, 0.7));
    vec3 camTarget = vec3(0.0, 0.0, 0.0);

    vec3 camDir = normalize(camTarget-camPos);
    vec3 camUp  = normalize(vec3(0.0, 1.0, 0.0));
    vec3 camSide = cross(camDir, camUp);
    float focus = 1.8;

    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir*focus);
    vec3 ray = camPos;
    float m = 0.0;
    float d = 0.0, total_d = 0.0;
    const int MAX_MARCH = 50;
    const float MAX_DISTANCE = 1000.0;
    for(int i=0; i<MAX_MARCH; ++i) {
        d = distest(ray);
        total_d += d;
        ray += rayDir * d;
        m += 1.0;
        if(d<0.001) { break; }
        if(total_d>MAX_DISTANCE) { total_d=MAX_DISTANCE; break; }
    }

    float c = (total_d)*0.0001;
    vec4 result = vec4( 1.0-vec3(c, c, c) - vec3(0.05, 0.05, 0.03)*m*0.8, 1.0 );
    gl_FragColor = result;
}
