#version 120
uniform vec2 window;
uniform vec3 camPos;
uniform vec3 camDir;
uniform vec3 camUp;
uniform vec3 camSide;
uniform vec3 julia;

float distest(vec3 pos) {
    const float Iterations = 10.0;
    const float Scale = 2.0;

	vec3 p=pos;
	p.xz=abs(.5-mod(pos.xz,1.))+.01;
	float DEfactor=1.;
	for (int i=0; i<Iterations; i++) {
		p = abs(p)-vec3(0.,2.,0.);  
		float r2 = dot(p, p);
		float sc=Scale/clamp(r2,0.4,1.);
		p*=sc; 
		DEfactor*=sc;
		p = p - vec3(0.5,0.5,0.5);
	}
    return length(p)/DEfactor-.0005;
}


void main() {
    vec2 pos = (gl_FragCoord.xy*2.0 - window.xy) / window.y;
    vec3 rayDir = normalize(camSide*pos.x + camUp*pos.y + camDir);
    vec3 ray = camPos;

	float d=1., totdist=0.;
	vec3 p, col=vec3(1.);
	float ref=0.;
    float det = 0.0;
    const float detail = 0.0001;
	for (int i=0; i<30; i++) {
		if (d>det) {
			p=ray+totdist*rayDir;
			d=distest(p);
			det=detail*(1.+totdist*55.);
			totdist+=d; 
		}
	}
    col*=1.0-log(sqrt(totdist));
    gl_FragColor = vec4(col, 1.0);
}


