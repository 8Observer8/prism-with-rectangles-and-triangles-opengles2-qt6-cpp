attribute vec3 aPosition;
attribute vec4 aNormal;
attribute vec2 aTexCoord;

uniform mat4 uMvpMatrix;
uniform mat4 uNormalMatrix;

varying float vNDotL;
varying vec2 vTexCoord;

const vec3 lightDirection = vec3(2.0, 5.0, 4.0);

void main()
{
    gl_Position = uMvpMatrix * vec4(aPosition, 1.0);
    vTexCoord = aTexCoord;
    vec3 normal = normalize(vec3(uNormalMatrix * aNormal));
    vNDotL = max(dot(normal, normalize(lightDirection)), 0.0);
}
