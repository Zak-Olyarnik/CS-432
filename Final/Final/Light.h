#ifndef __LIGHT_H__
#define __LIGHT_H__
#include "Angel.h"

class Light {
public:
	Light(vec4 pos, vec4 dir, vec4 a, vec4 s, vec4 d) : position(pos), direction(dir), ambient(a), specular(s), diffuse(d) {}
	vec4 getPosition() { return position; }
	vec4 getDirection() { return direction; }
	void setDirection(vec4 dir) { direction = dir; }
	vec4 getAmbient() { return ambient; }
	vec4 getDiffuse() { return diffuse; }
	vec4 getSpecular() { return specular; }
private:
	vec4 position;
	vec4 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
};
#endif