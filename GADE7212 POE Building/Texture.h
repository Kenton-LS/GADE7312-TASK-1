#pragma once
class Texture
{
private:
	unsigned char* image;
	int width;
	int height;
	int channels;
	bool loaded = false;
	void loadTexture(const char* path);
public:
	Texture(const char* path);
	~Texture();
	void use();
};