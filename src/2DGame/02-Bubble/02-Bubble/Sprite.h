#ifndef _SPRITE_INCLUDE
#define _SPRITE_INCLUDE


#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"
#include "AnimKeyframes.h"

//enum Physics { none, controllable, solid, pushable, win, eat};

// This class is derived from code seen earlier in TexturedQuad but it is also
// able to manage animations stored as a spritesheet. 

class Sprite
{

public:
	// Textured quads can only be created inside an OpenGL context
	static Sprite *createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program);

	void update(int deltaTime);
	void render() const;
	void free();

	void setNumberAnimations(int nAnimations);
	void setAnimationSpeed(int animId, int keyframesPerSec);
	void addKeyframe(int animId, const glm::vec2 &frame);
	void changeAnimation(int animId);
	int animation() const;
	
	void setPosition(const glm::vec2 &pos);
	glm::vec2 Sprite::getPosition() const;

	void setId(int newid);
	int getId() const;
	string getNoun() const;
	string getTag() const;
	string getInteraction() const;

	void setNoun(string noun);
	void setTag(string tag);
	void setInteraction(string interaction);
	void cambiaColor(glm::vec3 color);

	int getPriority();

	void setColor() const;
	void setAtr();

private:
	Texture *texture;
	ShaderProgram *shaderProgram;
	GLuint vao;
	GLuint vbo;
	GLint posLocation, texCoordLocation;
	glm::vec2 position;
	int currentAnimation, currentKeyframe;
	float timeAnimation;
	glm::vec2 texCoordDispl;
	glm::vec4 color;
	vector<AnimKeyframes> animations;
	glm::vec3 col;
	
	int priority = 0;

	// new
	int id;
	string noun = "other";
	string tag = "none";
	string interaction = "none";

	// nouns { baba, wall, rock, flag, flower, is, eat, you, stop, push, win, defeat, other }
	// tags { noun, op, property, entity, none }
	// interactions { none, you, stop, push, win, defeat, skull }
};


#endif // _SPRITE_INCLUDE

