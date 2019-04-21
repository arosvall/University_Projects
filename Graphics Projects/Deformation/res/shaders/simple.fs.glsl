
#version 150 core

uniform int gColor;

out vec4 color;

in vec3 fragPosition;
in vec3 fragNormal;

const vec3 lightDir = vec3(0.1, 0.1, -1);

const vec3 ambientColor = vec3(0.3, 0.3, 0.3);
const vec3 diffuseColor = vec3(0.4, 0.4, 1.0);
const vec3 specColor    = vec3(0.2, 0.1, 0.1);

const float shininess = 16.0;

void main() {
	// for picking-selection
	if ( gColor >= 0 ) {
		color = vec4(gColor/255.0, 0, 0, 1.0);
		return;
	}

    vec3 normal = fragNormal;
	normal = cross(dFdx(fragPosition), dFdy(fragPosition)); // flat shading
	normal = normalize(normal);
    vec3 lightDir = normalize(-lightDir);

    float lambertian = max(dot(lightDir,normal), 0.0);
    //lambertian = 1;
    float specular = 0.0;

    if (lambertian > 0.0) {
        vec3 viewDir = normalize(-fragPosition);

        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, normal), 0.0);

        //specular = pow(specAngle, shininess);
    }

    vec3 fragColor = ambientColor +
        lambertian * diffuseColor +
        specular * specColor;

    color = vec4(fragColor, 1.0);
}
