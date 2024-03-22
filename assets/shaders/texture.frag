#ifdef GL_ES
precision mediump float;
#endif

uniform sampler2D uSampler;

varying float vNDotL;
varying vec2 vTexCoord;

void main()
{
    vec4 color = texture2D(uSampler, vTexCoord);
    gl_FragColor = vec4(color.rgb * vNDotL, 1.0);
}
