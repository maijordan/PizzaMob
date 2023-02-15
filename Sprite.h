#pragma once

#include "Shape.h"
#include "ofxGui.h"
//#include "Emitter.h"
//project 2 final

class TriangleShape : public Shape {
public:
	TriangleShape() {
		// default data
		set(glm::vec3(20, 20, 0), glm::vec3(0, -40, 0), glm::vec3(-20, 20, 0));
	}
	virtual void draw();
	virtual bool inside(const glm::vec3 p);

	void set(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) {
		verts.clear();
		verts.push_back(v1);
		verts.push_back(v2);
		verts.push_back(v3);
	}

};

class Player : public TriangleShape {
public:
	Player() {}
	Player(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
		verts.push_back(p1);
		verts.push_back(p2);
		verts.push_back(p3);
	}
	void draw();
	ofImage ship;
	ofSoundPlayer gotHit;
	ofSoundPlayer pizzaHit;
	//player hp
	int playerhp = 20;
	//max hp
	int maxhp = 20;
	//ofxIntSlider nAgents;
	ofxToggle showSprite;
	int lineLength = 75;
	glm::vec3 heading() {
		glm::vec3 originalHeading = glm::vec3(0, -1, 0);
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		return  rot * glm::vec4(originalHeading, 1.0);

	}
	bool inside(glm::vec3 p) {
		glm::vec3 newP = glm::inverse(getMatrix()) * glm::vec4(p, 1.0);

		//checking the left,right,bottom, and top (respectively) of the image with the center being the middle of the image
		return (newP.x >= -(ship.getWidth() / 2) && newP.x <= ship.getWidth() / 2 && newP.y >= -(ship.getHeight() / 2) && newP.y <= ship.getHeight());
	}

	bool checkCollision(glm::vec3 newPos) {

		return newPos.y < ofGetWindowHeight() && newPos.x < ofGetWindowWidth() && newPos.x > 0 && newPos.y > 0;
	}

};

class Sprite : public TriangleShape {
public:
	// some functions for highlighting when selected
	//
	void draw() {
		if (bShowImage) {
			ofPushMatrix();
			ofSetColor(ofColor::white);
			ofMultMatrix(getMatrix());
			spriteImage.draw(-spriteImage.getWidth() / 2, -spriteImage.getHeight() / 2.0);
			ofPopMatrix();
		}
		else
		{
			if (bHighlight) ofSetColor(ofColor::white);
			else ofSetColor(ofColor::yellow);
			TriangleShape::draw();
		}
	}

	float age() {
		return (ofGetElapsedTimeMillis() - birthtime);
	}

	void setImage(ofImage img) {
		spriteImage = img;
		bShowImage = true;
		width = img.getWidth();
		height = img.getHeight();
	}

	bool inside(const glm::vec3 p);

	void setSelected(bool state) { bSelected = state; }
	void setHighlight(bool state) { bHighlight = state; }
	bool isSelected() { return bSelected; }
	bool isHighlight() { return bHighlight; }

	
	glm::vec3 heading() {
		return (glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1)) * glm::vec4(0, 1, 0, 1));
	}

	void integrate();

	bool bHighlight = false;
	bool bSelected = false;
	bool bShowImage = false;
	float damping = .99;
	ofVec3f force;
	ofVec3f acceleration;
	ofImage redParticle;
	glm::vec3 velocity = glm::vec3(0, 0, 0);
	float birthtime = 0; // elapsed time in ms
	float lifespan = -1;  //  time in ms
	string name =  "UnammedSprite";
	float width = 40;
	float height = 40;
	bool collided;
	bool boundingboxCollided;
	TriangleShape* tri;

	ofImage spriteImage;
};

class explosionParticles : public Sprite {
public:
	explosionParticles() {

	}

	
};

