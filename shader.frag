//threshold will decide the boundary to determine if point has ESCAPED to infinity i.e. greater than threshold
//typically threshold is 4.0 because mandelbrot is bounded in a circle of radius 2 in the complex plane
//higher threshold = smoother appearance, greater detail, but not the typical fractal look

#version 410

uniform int iterations;
uniform float zoom;
uniform vec2 viewportSize;
uniform vec2 offset;

float threshold = 9000;

//in mandelbrot c is varying
//and z is iterated over

float mandelbrot(vec2 c) {
    vec2 z = vec2(0.0, 0.0);
    int numberofitrs = 0;

    for(int i = 0; i < iterations; i++) {
        vec2 znew;

        znew.x = (z.x * z.x) - (z.y * z.y) + c.x;
        znew.y = (2.0 * z.x * z.y) + c.y;
        
        z = znew;
        
        if ((z.x * z.x) + (z.y * z.y) > threshold) break;
        
        ++numberofitrs;
    }
    //noramlize the number of iterations to [0,1]
    return float(numberofitrs) / float(iterations);
}

//in julia z is varying as per each pixel
//c is a constant, a julia constant
//some common julia fractals are obtained by setting 
//c = (-0.4, 0.6) 
//or (-0.7, 0.27015)
//or (0.285, 0.01)
//or (0.355, 0.355)
//or (-0.8, 0.156)

float julia(vec2 z) {
    int numberofitrs = 0;
    for (int i = 0; i < iterations; i++) {
        vec2 znew;
        vec2 c = vec2(-0.8, 0.156);
        znew.x = (z.x * z.x) - (z.y * z.y) + c.x;
        znew.y = (2.0 * z.x * z.y) + c.y;
        z = znew;

        if ((z.x * z.x) + (z.y * z.y) > threshold) break;
        numberofitrs++;
    }
    //noramlize the number of iterations to [0,1]
    return float(numberofitrs) / float(iterations);
}

// //linear gradient
// vec4 colorInterpolation(float t) {
//     vec3 color1 = vec3(0.0, 0.0, 1.0); // Blue
//     vec3 color2 = vec3(1.0, 0.0, 0.0); // Red

//     vec3 color = mix(color1, color2, t);

//     return vec4(color, 1.0);
// }

// //sinusoidal gradient
// vec4 colorInterpolation(float t) {
//     float r = 0.5 + 0.5 * sin(3.0 * 3.14159 * t);
//     float g = 0.5 + 0.5 * sin(3.0 * 3.14159 * (t + 0.33));
//     float b = 0.5 + 0.5 * sin(3.0 * 3.14159 * (t + 0.66));

//     return vec4(r, g, b, 1.0);
// }

// //logarithmic gradient
// vec4 colorInterpolation(float t) {
//     t = log(1.0 + t * 9.0) / log(10.0); // Transform t to a logarithmic scale
//     float r = t;
//     float g = t * t;
//     float b = sqrt(t);

//     return vec4(r, g, b, 1.0);
// }

// //hsv to rgb
// vec3 hsv2rgb(vec3 c) {
//     vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
//     vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
//     return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
// }

// vec4 colorInterpolation(float t) {
//     vec3 hsv = vec3(t * 360.0, 1.0, 1.0); // Hue, Saturation, Value
//     vec3 rgb = hsv2rgb(hsv);

//     return vec4(rgb, 1.0);
// }

// //rainbow gradient
// vec4 colorInterpolation(float t) {
//     float r = sin(6.28318 * t + 0.0);
//     float g = sin(6.28318 * t + 2.09439); // 2*pi/3
//     float b = sin(6.28318 * t + 4.18879); // 4*pi/3

//     return vec4(r * 0.5 + 0.5, g * 0.5 + 0.5, b * 0.5 + 0.5, 1.0);
// }

//custom polynomial gradient
vec4 colorInterpolation(float t){
    float r, g, b;
    
    r = 15.0 * (1.0 - t) * t * t * t;
    g = 9.5 * (1.0 - t) * (1.0 - t) * t * t;
    b = 6.0 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t; 

    return vec4(r, g, b, 1.0);
}

// //temperature gradient
// vec4 colorInterpolation(float t) {
//     float r = smoothstep(0.0, 0.5, t) * (1.0 - smoothstep(0.5, 1.0, t));
//     float g = smoothstep(0.25, 0.75, t);
//     float b = 1.0 - smoothstep(0.0, 1.0, t);

//     return vec4(r, g, b, 1.0);
// }

// //fire gradient
// vec4 colorInterpolation(float t) {
//     float r = smoothstep(0.0, 0.5, t);
//     float g = smoothstep(0.0, 0.25, t) * (1.0 - smoothstep(0.25, 0.75, t));
//     float b = (1.0 - smoothstep(0.0, 1.0, t)) * (1.0 - smoothstep(0.75, 1.0, t));

//     return vec4(r, g, b, 1.0);
// }

// //exponential gradient
// vec4 colorInterpolation(float t) {
//     float r = exp(t) - 1.0;
//     float g = exp(0.5 * t) - 1.0;
//     float b = exp(0.25 * t) - 1.0;

//     return vec4(r, g, b, 1.0);
// }

// //pastel
// vec4 colorInterpolation(float t) {
//     float r = exp(t) - 1.0;
//     float g = exp(0.5 * t) - 1.0;
//     float b = exp(0.25 * t) - 1.0;

//     return vec4(r, g, b, 1.0);
// }

// //night gradient
// vec4 colorInterpolation(float t) {
//     vec3 color = vec3(0.0, 0.0, 0.1) * (1.0 - t) + vec3(1.0, 0.6, 0.0) * t;
//     return vec4(color, 1.0);
// }

void main() {
    vec2 fragCoord = gl_FragCoord.xy;
    vec2 coords = ((fragCoord - viewportSize / 2.0) / zoom) - offset;

    float mandelbrotValue = mandelbrot(coords);
    float juliaValue = julia(coords);

    vec4 color = colorInterpolation(juliaValue);

    gl_FragColor = color;
}
