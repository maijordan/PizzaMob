#include "ofApp.h"
#include "Emitter.h"
//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" class to emit Sprites
// and set them in motion. The concept of an "Emitter" is taken from particle
// systems (which we will cover next week).
//
// The Sprite class has also been upgraded to include lifespan, velocity and age
// members.   The emitter can control rate of emission and the current velocity
// of the particles. In this example, there is no acceleration or physics, the
// sprites just move simple frame-based animation.
//
// The code shows a way to attach images to the sprites and optional the
// emitter (which is a point source) can also have an image.  If there are
// no images attached, a placeholder rectangle is drawn.
// Emitters  can be placed anywhere in the window. In this example, you can drag
// it around with the mouse.
//
// OF has an add-in called ofxGUI which is a very simple UI that is useful for
// creating sliders, buttons and fields. It is not recommended for commercial 
// game development, but it is useful for testing.  The "h" key will hide the GUI
// 
// If you want to run this example, you need to use the ofxGUI add-in in your
// setup.
//
//
//  Kevin M. Smith - CS 134 SJSU
//project 2 final



//  Add a Sprite to the Sprite System
//
void SpriteList::add(Sprite s) {
	sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteList::remove(int i) {
	sprites.erase(sprites.begin() + i);
}

void SpriteList::addForce(SpriteForce *f) {
	forces.push_back(f);
}


//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteList::update() {

	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	//
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	//  Move sprite
	//
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].pos += sprites[i].velocity / ofGetFrameRate();
	}
}

//  Render all the sprites
//
void SpriteList::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}



Emitter::Emitter() {
	sys = new SpriteList();
	init();
}


void Emitter::init() {
	lifespan = 3000;    // default milliseconds
	started = false;
	//player hp
	//levels = 20;
	lastSpawned = 0;
	rate = 1;    // sprites/sec
	haveChildImage = false;
	haveImage = false;
	velocity = ofVec3f(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
}



//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
	if (drawable) {

		if (haveImage) {
			image.draw(-image.getWidth() / 2.0 + pos.x, -image.getHeight() / 2.0 + pos.y);
		}
		else {
			ofSetColor(0, 0, 200);
			ofDrawRectangle(-width / 2 + pos.x, -height / 2 + pos.y, width, height);
		}
	}

	// draw sprite system
	//
	sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update() {
	if (!started) return;

	float time = ofGetElapsedTimeMillis();

	if ((time - lastSpawned) > (1000.0 / rate)) {
		
		// call virtual to spawn a new sprite
		//
		for (int i = 0; i < nAgents; i++) {
			spawnSprite(i);
			
		}
		lastSpawned = time;
	}



	// update sprite list
	//
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
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			//			cout << "deleting sprite: " << s->name << endl;
			tmp = sys->sprites.erase(s);
			s = tmp;
		}
		/*
		else if (s->collided || s->boundingboxCollided) {
			tmp = sys->sprites.erase(s);
			s = tmp;
			tri->playerhp--;
		}
		*/
		else s++;
	}
	
	/*
	for (int i = 0; i < sys->sprites.size(); i++) {
		for (int k = 0; k < sys->forces.size(); k++) {
			sys->forces[k]->updateForce(&sys->sprites[i]);
		}
	}
	*/
	/*
	for (int i = 0; i < sys->forces.size(); i++) {
		if (sys->forces[i]->applyOnce)
			sys->forces[i]->applied = true;
	}
	*/
		
		
	

	
	for (int i = 0; i < sys->sprites.size(); i++) {

		moveSprite(&sys->sprites[i]);
	}
	
}

// virtual function to move sprite (can be overloaded)
// changed
void Emitter::moveSprite(Sprite *sprite) {
	//sprite->rotation = glm::dot(glm::vec3(acos(rotation),1, 1), pos);
	glm::vec3 originalHeading = glm::vec3(0, -1, 0);
    sprite->pos += sprite->heading();
	// ofGetFrameRate();
}

// virtual function to spawn sprite (can be overloaded)
//
void Emitter::spawnSprite(int numAgents) {
	Sprite sprite;
	if (haveChildImage) sprite.setImage(childImage);
	sprite.velocity = velocity * 2;
	sprite.lifespan = lifespan;
	sprite.tri = tri;
	
	sprite.pos = pos + glm::vec3(numAgents * childImage.getWidth(), 0, 0);
	
	//random spawn direction
	sprite.rotation = ofRandom(0, 365);
	sprite.birthtime = ofGetElapsedTimeMillis();
	sys->add(sprite);
}

// Start/Stop the emitter.
//
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
	started = false;
}


void Emitter::setLifespan(float life) {
	lifespan = life;
}

void Emitter::setVelocity(const glm::vec3 v) {
	velocity = v;
}

void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
	image = img;
}

void Emitter::setRate(float r) {
	rate = r;
}

void Emitter::agentCollision() {
	glm::vec3 distance = abs(tri->pos - pos);
	float contactDistance = tri->ship.getWidth() / 2 + childImage.getWidth()/2;
	
	 
	
	
}

// Turbulence Force Field 
//
TurbulenceForce::TurbulenceForce(const ofVec3f& min, const ofVec3f& max) {
	tmin = min;
	tmax = max;
}

void TurbulenceForce::updateForce(Sprite* sprite) {
	//
	// We are going to add a little "noise" to a particles
	// forces to achieve a more natual look to the motion
	//
	sprite->force.x += ofRandom(tmin.x, tmax.x);
	sprite->force.y += ofRandom(tmin.y, tmax.y);
	//sprite->forces.z += ofRandom(tmin.z, tmax.z);
}
//from OF code examples->math-> particleexample
void AttractionForce::updateForce(Sprite* sprite) {
	glm::vec3 attractPt(attractPos.x, attractPos.y, 0);
	sprite->force = attractPt - sprite->pos;
	sprite->force = glm::normalize(glm::vec3(sprite->force));
	float drag = ofRandom(0.95, 0.998);
	sprite->velocity *= drag;

	//was velocity
	sprite->acceleration += sprite->force * 0.6;
	
	
}

ImpulseRadialForce::ImpulseRadialForce(float magnitude) {
	this->magnitude = magnitude;
	//this->height = 0;
	applyOnce = true;
}

void ImpulseRadialForce::updateForce(Sprite* sprite) {

	// we basically create a random direction for each particle
	// the force is only added once after it is triggered.
	//
	ofVec3f dir = ofVec3f(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));

	//ofVec3f normDir = dir.getNormalized();
	sprite->force += dir.getNormalized();
	/*
	//sphere force is 1, the other is the height from slider
	if (height != 0) {

		if (normDir.y * magnitude > height / 2) {
			normDir.y = height / 2;
		}
		else if (normDir.y * magnitude < -height / 2) {
			normDir.y = -height / 2;
		}
	}
	*/

	//sprite->forces += ofVec3f(normDir.x, normDir.y, normDir.z) * magnitude;


}
