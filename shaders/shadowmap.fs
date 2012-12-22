#version 330

uniform vec3 cameraPosition;

smooth in vec3 fragmentPosition;

out float distanceToFragment;

void main () {
    distanceToFragment = length(fragmentPosition - cameraPosition);
}
