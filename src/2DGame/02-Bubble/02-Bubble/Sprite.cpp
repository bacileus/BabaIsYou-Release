#include <GL/glew.h>
#include <GL/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Sprite.h"

Sprite *Sprite::createSprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	Sprite *quad = new Sprite(quadSize, sizeInSpritesheet, spritesheet, program);

	return quad;
}


Sprite::Sprite(const glm::vec2 &quadSize, const glm::vec2 &sizeInSpritesheet, Texture *spritesheet, ShaderProgram *program)
{
	float vertices[24] = {0.f, 0.f, 0.f, 0.f, 
												quadSize.x, 0.f, sizeInSpritesheet.x, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, 0.f, 0.f, 0.f, 
												quadSize.x, quadSize.y, sizeInSpritesheet.x, sizeInSpritesheet.y, 
												0.f, quadSize.y, 0.f, sizeInSpritesheet.y};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), vertices, GL_STATIC_DRAW);
	posLocation = program->bindVertexAttribute("position", 2, 4*sizeof(float), 0);
	texCoordLocation = program->bindVertexAttribute("texCoord", 2, 4*sizeof(float), (void *)(2*sizeof(float)));
	texture = spritesheet;
	shaderProgram = program;
	currentAnimation = -1;
	position = glm::vec2(0.f);

	col = glm::vec3(-1.f, -1.f, -1.f);
}

void Sprite::update(int deltaTime)
{
	if(currentAnimation >= 0)
	{
		timeAnimation += deltaTime;
		while(timeAnimation > animations[currentAnimation].millisecsPerKeyframe)
		{
			timeAnimation -= animations[currentAnimation].millisecsPerKeyframe;
			currentKeyframe = (currentKeyframe + 1) % animations[currentAnimation].keyframeDispl.size();
		}
		texCoordDispl = animations[currentAnimation].keyframeDispl[currentKeyframe];
	}
}

void Sprite::render() const
{
	if (id != 0) {
		glm::mat4 modelview = glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, 0.f));
		shaderProgram->setUniformMatrix4f("modelview", modelview);
		shaderProgram->setUniform2f("texCoordDispl", texCoordDispl.x, texCoordDispl.y);
		if (col != glm::vec3(-1.f)) shaderProgram->setUniform4f("color", col.x, col.y, col.z, 1.0f);
		else setColor();
		glEnable(GL_TEXTURE_2D);
		texture->use();
		glBindVertexArray(vao);
		glEnableVertexAttribArray(posLocation);
		glEnableVertexAttribArray(texCoordLocation);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glDisable(GL_TEXTURE_2D);
	}
}

void Sprite::free()
{
	glDeleteBuffers(1, &vbo);
}

void Sprite::setNumberAnimations(int nAnimations)
{
	animations.clear();
	animations.resize(nAnimations);
}

void Sprite::setAnimationSpeed(int animId, int keyframesPerSec)
{
	if(animId < int(animations.size()))
		animations[animId].millisecsPerKeyframe = 1000.f / keyframesPerSec;
}

void Sprite::addKeyframe(int animId, const glm::vec2 &displacement)
{
	if(animId < int(animations.size()))
		animations[animId].keyframeDispl.push_back(displacement);
}

void Sprite::changeAnimation(int animId)
{
	if(animId < int(animations.size()))
	{
		currentAnimation = animId;
		currentKeyframe = 0;
		timeAnimation = 0.f;
		texCoordDispl = animations[animId].keyframeDispl[0];
	}
}

int Sprite::animation() const
{
	return currentAnimation;
}

void Sprite::setPosition(const glm::vec2 &pos)
{
	position = pos;
}

glm::vec2 Sprite::getPosition() const
{
	return position;
}

void Sprite::setId(int newid)
{
	id = newid;
}

string Sprite::getNoun() const
{
	return noun;
}

string Sprite::getTag() const
{
	return tag;
}

string Sprite::getInteraction() const
{
	return interaction;
}

int Sprite::getId() const
{
	return id;
}

void Sprite::setNoun(string noun)
{
	this->noun = noun;
}

void Sprite::setTag(string tag)
{
	this->tag = tag;
}

void Sprite::setInteraction(string interaction)
{
	this->interaction = interaction;
}

void Sprite::cambiaColor(glm::vec3 color) {

	col = color;

}

int Sprite::getPriority()
{
	return priority;
}

void Sprite::setColor() const
{
	switch (id) {
	case 962: // flag noun
		shaderProgram->setUniform4f("color", 0.95f, 0.87f, 0.55f, 1.0f);
		break;
	case 679: // flag
		shaderProgram->setUniform4f("color", 0.95f, 0.87f, 0.55f, 1.0f);
		break;
	case 774: // fire
		shaderProgram->setUniform4f("color", 1.f, 0.4f, 0.f, 1.0f);
		break;
	case 961: // fire noun
		shaderProgram->setUniform4f("color", 1.f, 0.4f, 0.f, 1.0f);
		break;
	case 979: // is noun
		shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case 692: // floor
		shaderProgram->setUniform4f("color", 0.11f, 0.12f, 0.16f, 0.2f);
		break;
	case 871: // baba noun
		shaderProgram->setUniform4f("color", 0.76f, 0.31f, 0.4f, 1.0f);
		break;
	case 892: // eat noun
		shaderProgram->setUniform4f("color", 1.0f, 0.44f, 0.16f, 1.0f);
		break;
	case 688: // rock
		shaderProgram->setUniform4f("color", 0.81f, 0.64f, 0.3f, 1.0f);
		break;
	case 1068: // rock noun
		shaderProgram->setUniform4f("color", 0.62f, 0.45f, 0.27f, 1.0f);
		break;
	case 1826: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1830: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1829: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1084: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1834: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1837: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1835: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1831: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1828: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1838: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1839: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1833: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1825: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1827: // wall
		shaderProgram->setUniform4f("color", 0.26f, 0.29f, 0.36f, 1.0f);
		break;
	case 1254: // defeat noun
		shaderProgram->setUniform4f("color", 0.57f, 0.21f, 0.14f, 1.0f);
		break;
	case 1347: // push noun
		shaderProgram->setUniform4f("color", 0.62f, 0.45f, 0.27f, 1.0f);
		break;
	case 1357: // stop noun
		shaderProgram->setUniform4f("color", 0.49f, 0.49f, 0.49f, 1.0f);
		break;
	case 1362: // win noun
		shaderProgram->setUniform4f("color", 0.95f, 0.87f, 0.55f, 1.0f);
		break;
	case 1365: // you noun
		shaderProgram->setUniform4f("color", 0.76f, 0.31f, 0.4f, 1.0f);
		break;
	case 1633: // grass
		shaderProgram->setUniform4f("color", 0.23f, 0.24f, 0.20f, 1.0f);
		break;
	case 1642: // grass
		shaderProgram->setUniform4f("color", 0.23f, 0.24f, 0.20f, 1.0f);
		break;
	case 1643: // grass
		shaderProgram->setUniform4f("color", 0.23f, 0.24f, 0.20f, 1.0f);
		break;
	case 1645: // grass
		shaderProgram->setUniform4f("color", 0.23f, 0.24f, 0.20f, 1.0f);
		break;
	case 1636: // grass
		shaderProgram->setUniform4f("color", 0.23f, 0.24f, 0.20f, 1.0f);
		break;
	case 1637: // grass
		shaderProgram->setUniform4f("color", 0.23f, 0.24f, 0.20f, 1.0f);
		break;
	case 492: // skull
		shaderProgram->setUniform4f("color", 0.58f, 0.21f, 0.13f, 1.0f);
		break;
	case 1073: // skull noun
		shaderProgram->setUniform4f("color", 0.57f, 0.21f, 0.14f, 1.0f);
		break;
	case 683: //key para MENU
		shaderProgram->setUniform4f("color", 0.05f, 0.05f, 0.05f, 1.0f);
		break;
	default:
		shaderProgram->setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}
}

void Sprite::setAtr()
{
	
	/*
	default
	string noun = "other";
	string tag = "entity";
	string interaction = "none";
	
	candidates
	nouns { baba, wall, rock, flag, flower, is, eat, you, stop, push, win, defeat, other }
	tags { noun, op, property, entity };
	interactions { none, you, stop, push, win, defeat }
	*/

	tag = "entity";
	switch (id) {
	case 4:	// baba
		noun = "baba";
		break;
	case 962: // flag noun
		noun = "flag";
		tag = "noun";
		interaction = "push";
		break;
	case 679: // flag
		noun = "flag";
		break;
	case 774: // fire
		noun = "fire";
		break;
	case 961: // fire noun
		noun = "fire";
		tag = "noun";
		interaction = "push";
		break;
	case 979: // is op
		noun = "is";
		tag = "op";
		interaction = "push";
		break;
	case 692: // floor
		noun = "floor";
		priority = -1;
		break;
	case 871: // baba noun
		noun = "baba";
		tag = "noun";
		interaction = "push";
		break;
	case 892: // eat noun
		noun = "eat";
		tag = "op";
		interaction = "push";
		break;
	case 688: // rock
		noun = "rock";
		break;
	case 1068: // rock noun
		noun = "rock";
		tag = "noun";
		interaction = "push";
		break;
	case 1826: // wall
		noun = "wall";
		break;
	case 1830: // wall
		noun = "wall";
		break;
	case 1829: // wall
		noun = "wall";
		break;
	case 1084: // wall noun
		noun = "wall";
		tag = "noun";
		interaction = "push";
		break;
	case 1834: // wall
		noun = "wall";
		break;
	case 1837: // wall
		noun = "wall";
		break;
	case 1835: // wall
		noun = "wall";
		break;
	case 1831: // wall
		noun = "wall";
		break;
	case 1828: // wall
		noun = "wall";
		break;
	case 1838: // wall
		noun = "wall";
		break;
	case 1839: // wall
		noun = "wall";
		break;
	case 1833: // wall
		noun = "wall";
		break;
	case 1827: // wall
		noun = "wall";
		break;
	case 1825:
		noun = "wall";
		break;
	case 1254: // defeat noun
		noun = "defeat";
		tag = "property";
		interaction = "push";
		break;
	case 1347: // push noun
		noun = "push";
		tag = "property";
		interaction = "push";
		break;
	case 1357: // stop noun
		noun = "stop";
		tag = "property";
		interaction = "push";
		break;
	case 1362: // win noun
		noun = "win";
		tag = "property";
		interaction = "push";
		break;
	case 1365: // you noun
		noun = "you";
		tag = "property";
		interaction = "push";
		break;
	case 1633: // grass
		noun = "grass";
		priority = -1;
		break;
	case 1642: // grass
		noun = "grass";
		priority = -1;
		break;
	case 1643: // grass
		noun = "grass";
		priority = -1;
		break;
	case 1645: // grass
		noun = "grass";
		priority = -1;
		break;
	case 1636: // grass
		noun = "grass";
		priority = -1;
		break;
	case 1637: // grass
		noun = "grass";
		priority = -1;
		break;
	case 492: // skull
		noun = "skull";
		break;
	case 683: // key
		noun = "key";
		break;
	case 1073: // skull noun
		noun = "skull";
		tag = "noun";
		interaction = "push";
		break;
	case 401: // limit
		noun = "other";
		interaction = "stop";
		break;
	default:
		noun = "other";
		tag = "none";
		interaction = "none";
		break;
	}
 }