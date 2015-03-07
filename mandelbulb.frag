#version 120
uniform vec2 window;
uniform float power;

float g=0.99;
vec3 C = vec3(.7,.9,1.4);

float distest(vec3 p) {
    float dr = 1.0;
    
    vec4 ot = vec4(1000.0); 
    float r2;
  
    for( int i=0; i< 15; i++ ) {
        r2 = dot(p,p);
        if(r2>100.)continue;
        
        ot = min( ot, vec4(abs(p),r2) );

        //Kali formula
        p=abs(p)/r2*g-C;
        dr= dr/r2*g;
    }    
    return .1*(abs(p.x)+abs(p.y))*length(p)/dr;
}

void main( void )
{
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 camPos = vec3(0.0, 0.0, 7.0-power);
    vec3 camTarget = vec3(0.0, 0.0, 0.0);

    vec3 camDir = normalize(camTarget-camPos);
    vec3 camUp  = normalize(vec3(0.0, 1.0, 0.0));
    vec3 camSide = cross(camDir, camUp);
    float focus = 1.8;

    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir*focus);
    vec3 ray = camPos;
    float m = 0.0;
    float d = 0.0, total_d = 0.0;
    const int MAX_MARCH = 30;
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


