module;

#include "common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

export module World;

import Primitives;
import Utility;

export class Frame
{
public:
	int width, height, channels;
	unsigned char* image = nullptr;
	unsigned int texture;

	~Frame()
	{
		if (image != nullptr) {
			//stbi_image_free(image); // <-- this is behaving naughty
		}
	}

	bool load(std::string path)
	{
		// Load image data from drive
		image = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (image == NULL)
		{
			Console::Out("Frame::Load::Error: Failed loading image from path " + path);
			return false;
		}

		// Create Texture Information
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_UNSIGNED_BYTE, GL_RGB, image);
		//glGenerateMipmap(GL_TEXTURE_2D); // <-- dont think this is necessary for this 2D game

		return true;
	}


};

export class Animation
{
public:
	uint16_t head = 0;
	bool repeat = false;
	std::vector<Frame> frames = std::vector<Frame>();

	void rewind()
	{
		head = 0;
	}

	void next()
	{
		++head;
		uint16_t size0 = frames.size() - 1;

		if (head >= size0) {
			if (repeat) {
				rewind();
			}
			else {
				head = size0;
			}
		}
	}

	bool loadStrip(std::vector<std::string> paths) 
	{
		bool err = false;
		std::vector<Frame> tmpFrames = std::vector<Frame>();
		for (auto path : paths)
		{
			Frame frame;
			if (!frame.load(path))
			{
				Console::Out("Animation::LoadStrip::Error: Failed to load image from path " + path + ". aborting");
				err = true;
				break;
			}
			tmpFrames.push_back(frame);
		}

		if (!err) {
			frames = tmpFrames;
			return true;
		}

		return false;
	}
};

export class GameObject
{
public:
	glm::mat4x4 translation = glm::mat4(1.0f);
	glm::vec3 rotation = glm::vec3(0.0f, 1.0f, 0.0);
	glm::mat4x4 scale = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f));
	
	std::shared_ptr<Animation> activeAnimation = nullptr;
	std::unordered_map<std::string, std::shared_ptr<Animation>> animations = std::unordered_map<std::string, std::shared_ptr<Animation>>();

	virtual void Init()
	{
		// load texture

	}

	virtual void Update()
	{

	}

	virtual void Draw()
	{
		glUseProgram(Quad::shaderProgramId);
		glBindVertexArray(Quad::vaoId);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
};

export class Dino : public GameObject
{
public:
	
	void Init() final override
	{
		Console::Out("Loading Dino object");

		bool success = true;

		auto idle = std::make_shared<Animation>();
		success = idle->loadStrip({
			"./gamex64/sprites/dino/dino_idle_00.png"
		});
		if (!success) {
			Console::Out("Failed to load dino idle animation");
		}
		else {
			animations["idle"] = std::move(idle);
		}
		

		auto run = std::make_shared<Animation>();
		success = run->loadStrip({
			"./gamex64/sprites/dino/dino_run_00.png",
			"./gamex64/sprites/dino/dino_run_01.png",
		});
		if (!success) {
			Console::Out("Failed to load dino run animation");
		}
		else {
			animations["run"] = std::move(run);
		}
		

		auto crouch = std::make_shared<Animation>();
		success = crouch->loadStrip({
			"./gamex64/sprites/dino/dino_crouch_00.png",
			"./gamex64/sprites/dino/dino_crouch_01.png",
		});
		if (!success) {
			Console::Out("Failed to load dino crouch animation");
		}
		else {
			animations["crouch"] = std::move(crouch);
		}
		

		auto death = std::make_shared<Animation>();
		success = death->loadStrip({
			"./gamex64/sprites/dino/dino_death_00.png"
		});
		if (!success) {
			Console::Out("Failed to load dino death animation");
		}
		else {
			animations["death"] = std::move(death);
		}
		

		Console::Out("Done loading Dino object");
	}

	void Update() final override
	{

	}
};

export class Ground : public GameObject
{

};

export class Cactus : public GameObject
{

};

export class Bird : public GameObject
{

};

export class World : public GameObject
{

};
