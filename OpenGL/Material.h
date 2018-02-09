#pragma once

class Material
{
public:
	float Opacity() const { return this->opacity; }
	void Opacity(float opacity) { this->opacity = opacity; }

private:
	float shininess;
	float opacity;
};