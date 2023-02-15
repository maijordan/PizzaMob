#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "Sprite.h"
#include "ofxGui.h"

//project 2 final
//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteForce {
public:
	bool applyOnce = false;
	bool applied = false;
	virtual void updateForce(Sprite*) = 0;
};

class TurbulenceForce : public SpriteForce {
	ofVec3f tmin, tmax;
public:
	TurbulenceForce(const ofVec3f& min, const ofVec3f& max);
	//added this
	void set(const ofVec3f& min, const ofVec3f& max) {
		tmin = min;
		tmax = max;
	}
	void updateForce(Sprite*);
};

class AttractionForce : public SpriteForce {
	glm::vec3 attractPos;
public:
	AttractionForce(const glm::vec3& playerPos) {
		attractPos = playerPos;
	}
	void set(const glm::vec3& playerPos) {
		attractPos = playerPos;
	};
	void updateForce(Sprite*);
};

//from lab 6
class ImpulseRadialForce : public SpriteForce {
	float magnitude;
	float height;
public:
	ImpulseRadialForce(float magnitude);
	void updateForce(Sprite*);

	void setHeight(float y) {
		height = y;
	}

};

class SpriteList {
public:
	void add(Sprite);
	void addForce(SpriteForce *);
	void remove(int);
	void update();
	void draw();
	vector<Sprite> sprites;
	vector<SpriteForce*> forces;
};



//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public Shape {
public:
	Emitter();
	void init();
	void draw();
	void start();
	void stop();
	void setLifespan(float);
	void setVelocity(const glm::vec3 v);
	void setChildImage(ofImage);
	void setImage(ofImage);
	void setRate(float);
	void update();
	void agentCollision();

	// virtuals - can overloaded
	virtual void moveSprite(Sprite *);
	virtual void spawnSprite(int agents);

	SpriteList *sys;
	SpriteList *agentList;
	vector<Sprite> particles;
	float rate;
	glm::vec3 velocity;
	glm::vec3 attractionForce;
	float lifespan;
	bool started;
	float lastSpawned;
	ofImage childImage;
	ofImage particleImage;
	ofImage image;
	bool drawable;
	bool haveChildImage;
	bool haveImage;
	float width, height;
	Player* tri;
	ofxIntSlider nAgents;
	int levels;
	Emitter *explosion;
	//bool collided;
};

class BulletEmitter : public Emitter {
public:
	void moveSprite(Sprite* sprite) override {
		//cout << tri->pos.y << endl;
		glm::vec3 originalHeading = glm::vec3(0, -1, 0);
		//sprite->pos += tri->heading();

		if (sys->sprites.size() == 0) return;
		vector<Sprite>::iterator s = agentList->sprites.begin();
		//vector<Sprite>::iterator bullet = sys->sprites.begin();
		vector<Sprite>::iterator tmp;
		

		// check which sprites have exceed their lifespan and delete
		// from list.  When deleting multiple objects from a vector while
		// traversing at the same time, use an iterator.
		// 
		//collision detection for bullets and agents
		while (s != agentList->sprites.end()) {
			//checks for collision without sprite
			s->collided = s->inside(sprite->pos);
			sprite->collided = s->inside(sprite->pos);
			//checks bounding box
			s->boundingboxCollided = sprite->inside(s->pos);
			sprite->boundingboxCollided = sprite->inside(s->pos);
			
			if (s->collided || s->boundingboxCollided) {
				//cout << explosion->pos << endl;
				//explosion->pos = s->pos;
				tmp = agentList->sprites.erase(s);
				s = tmp;
				for (int i = 0; i < 20; i++) {
					cout << "here" << endl;
					explosionParticles particle;
					particle.pos = sprite->pos;
					particle.setImage(particleImage);
					particle.lifespan = 1 * 1000; //in milliseconds
					particle.birthtime = ofGetElapsedTimeMillis();
					particle.acceleration = ofVec3f(ofRandom(-100, 100), ofRandom(-100, 100), ofRandom(-100, 100));
					particles.push_back(particle);

				}
				tri->pizzaHit.play();
				tri->playerhp++;
				
				
			}
			

			else s++;
		}
		sprite->integrate();
	}

	void spawnSprite(int numAgents) override {
		Sprite sprite;
		if (haveChildImage) sprite.setImage(childImage);
		sprite.velocity = tri->heading() * 1000;
		sprite.lifespan = lifespan;


		//sprite.pos = tri->pos + glm::vec3(childImage.getWidth(), 0, 0);
		sprite.pos = pos;

		//random spawn direction
		//sprite.rotation = ofRandom(0, 365);
		// apply 90 degrees
		sprite.rotation = tri->rotation;
		sprite.birthtime = ofGetElapsedTimeMillis();
		sys->add(sprite);
	}
	
};

class ExplosionEmitter : public Emitter {
public:
	void moveSprite(Sprite* sprite) override {
		sprite->integrate();
		}
	
	

	void spawnSprite(int numAgents) override {
		Sprite sprite;
		ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));
		float speed = velocity.length();
		sprite.velocity = dir.getNormalized() * speed;
		sprite.pos = pos;
		sys->add(sprite);
		
	}

};



