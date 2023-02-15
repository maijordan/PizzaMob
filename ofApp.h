#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Shape.h"
//project 2 final
class Agent : public Sprite {

};

class AgentEmitter : public Emitter {
public:
	void moveSprite(Sprite* sprite) {
		//cout << tri->pos.y << endl;
		sprite->rotation = glm::degrees(atan2(tri->pos.y - sprite->pos.y,tri->pos.x - sprite->pos.x)- 90 );
		//Emitter::moveSprite(sprite);
		

		//attraction force
		/*
		glm::vec3 attractPt(tri->pos.x, tri->pos.y, 0);
		attractionForce = attractPt - sprite->pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		attractionForce = glm::normalize(attractionForce); //by normalizing we disregard how close the particle is to the attraction point
		float drag = ofRandom(0.95, 0.998);
		sprite->velocity *= drag; //apply drag
		sprite->velocity += attractionForce * 0.6; //apply force
		*/
		//
		//sprite->integrate();

		if (sys->sprites.size() == 0) return;
		vector<Sprite>::iterator s = sys->sprites.begin();
		vector<Sprite>::iterator tmp;

		// check which sprites have exceed their lifespan and delete
		// from list.  When deleting multiple objects from a vector while
		// traversing at the same time, use an iterator.
		//
		while (s != sys->sprites.end()) {
			//checks for collision without sprite
			s->collided = s->inside(tri->pos);
			//checks bounding box
			s->boundingboxCollided = tri->inside(s->pos);

			if (s->collided || s->boundingboxCollided) {
				tmp = sys->sprites.erase(s);
				s = tmp;
				tri->gotHit.play();
				tri->playerhp--;
			}
			
			//else s++;
			else s++;
			
			for (int i = 0; i < sys->sprites.size(); i++) {
				for (int k = 0; k < sys->forces.size(); k++) {
					sys->forces[k]->updateForce(&sys->sprites[i]);
				}
				
			}
			
			sprite->integrate();
			
		}
	}
	void spawnSprite() {
		Emitter::spawnSprite(nAgents);
	}
};



class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
	//	vector<Emitter *> emitters;
	//	int numEmitters;

		Emitter  *turret = NULL;
		Emitter *playerTurret = NULL;
		Emitter* Explosion = NULL;

		ofImage defaultImage;
		ofImage background;
		ofImage startScreen;
		ofImage helpScreen;
		ofImage bullet;
		ofImage endScreen;

		ofImage imageParticle;
		ofSoundPlayer motor;
		ofVec3f mouse_last;
		bool imageLoaded;

		bool bHide;

		ofxFloatSlider shootingRate;
		ofxFloatSlider rate;
		ofxFloatSlider life;
		ofxVec3Slider velocity;
		ofxLabel screenSize;


		ofxPanel gui;


		Player tri;
		bool inside;
		int offSetX;
		int offSetY;
		bool ctrlPressed;
		int prevPos;
		int level = 100;
		map<int, bool> keymap;
		int speed = 600;
		ofxFloatSlider speedSlider;
		ofxFloatSlider scaleSlider;
		//ofxToggle showSprite;
		ofxToggle showHeading;
		glm::vec3 newPosUp;
		glm::vec3 newPosDown;
		ofxIntSlider nAgents = 1;
		string game_state;
		string difficulty = "easy";
		float time;
		float endtime;
		float timeSubtract = 0.0;

		//force slider
		TurbulenceForce* turbulence = NULL;
		ImpulseRadialForce* radialForce = NULL;
		AttractionForce* attractForce = NULL;
		

};
