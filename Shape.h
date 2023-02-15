#pragma once

#include "ofMain.h"

class Shape {
public:
	Shape() {}
	float scale1 = 1.0;
	float scale2 = 1.0;
	float scale3 = 1.0;
	virtual void draw() {

		// draw a box by defaultd if not overridden
		//project 2 final
		ofPushMatrix();
		ofMultMatrix(getMatrix());
		ofDrawBox(defaultSize);
		ofPopMatrix();
	}

	virtual bool inside(glm::vec3 p) {
		return false;
	}


	glm::mat4 getMatrix() {
		glm::mat4 m1 = glm::translate(glm::mat4(1.0), glm::vec3(pos));
		glm::mat4 m2 = glm::rotate(m1, glm::radians(rotation), glm::vec3(0, 0, 1));
		//glm::mat4 M = glm::scale(m2, scale);      // added this scale if you want to change size of object
		glm::mat4 scale = glm::scale(m2, glm::vec3(scale1, scale2, scale3));
		return scale;
	}
	glm::vec3 pos;
	float rotation = 0.0;    // degrees 
	glm::vec3 scale = glm::vec3(1, 1, 1);
	float defaultSize = 20.0;

	vector<glm::vec3> verts;
};