#include "ofApp.h"

/*
* project 2 final
void Player::draw() {

	//ofSetColor(ofColor::red);
	ofFill();

	ofPushMatrix();
	ofMultMatrix(getMatrix());
	ofDrawTriangle(verts[0], verts[1], verts[2]);
	if (showSprite) {
		ship.draw(-ship.getWidth() / 2, -ship.getHeight() / 2);
	}
	ofPopMatrix();

}
*/
//--------------------------------------------------------------
void ofApp::setup(){
	
	game_state = "start";
	// create an image for sprites being spawned by emitter
	//
	if (defaultImage.load("images/pineapple.png")) {
		imageLoaded = true;
	}
	else {
		cout << "Can't open image file" << endl;
		ofExit();
	}

	background.load("images/pizzashop.png");
	startScreen.load("images/startscreen.png");
	bullet.load("images/pepPizza.png");
	endScreen.load("images/endscreen.png");
	helpScreen.load("images/helpscreen.png");
	motor.loadSound("sounds/motor.wav");
	imageParticle.loadImage("images/explosionparticle.png");
	// create an array of emitters and set their position;
	//
	
//	turret = new Emitter();
	tri = Player(glm::vec3(-50, 50, 0), glm::vec3(50, 50, 0), glm::vec3(0, -50, 0));
	tri.pos = glm::vec3(ofGetWindowWidth() / 2.0 - 100, ofGetWindowHeight() / 2.0 - 100, 0);
	ofSetColor(ofColor::red);
	tri.ship.load("images/bobPlayer.png");
	ofSetVerticalSync(true);
	tri.gotHit.loadSound("sounds/give.wav");
	tri.pizzaHit.loadSound("sounds/pizzaHit.wav");

	turret = new AgentEmitter();
	//copy constructor
	playerTurret = new BulletEmitter();
	Explosion = new ExplosionEmitter();


	turret->pos = glm::vec3(ofRandom(0,ofGetWindowWidth()), ofRandom(0, ofGetWindowHeight()), 0);
	turret->drawable = false; 
	turret->setChildImage(defaultImage);
	turret->tri = &tri;
	
	radialForce = new ImpulseRadialForce(1000.0);
	attractForce = new AttractionForce(tri.pos);
	//turret->sys->addForce(turbulence);
	turret->sys->addForce(attractForce);
	turret->start();
	//added for player turret
	playerTurret->pos = glm::vec3(tri.pos.x,tri.pos.y,0);
	playerTurret->drawable = false;
	//player bullet
	playerTurret->particleImage = imageParticle;
	playerTurret->agentList = turret->sys;
	playerTurret->setChildImage(bullet);
	playerTurret->tri = &tri;
	//playerTurret->start();
	playerTurret->explosion = Explosion;
	playerTurret->start();
	Explosion->drawable = true;
	Explosion->pos = glm::vec3(0, 0, 0);
	Explosion->sys->addForce(radialForce);
	Explosion->setChildImage(defaultImage);
	//Explosion->start();
	//playerTurret->explosion = Explosion;
	//playerTurret->start();
	
	gui.setup();
	gui.add(rate.setup("Agent Spawnrate", 1, 1, 10));
	gui.add(shootingRate.setup("Fire Rate", 1, 1, 10));
	gui.add(life.setup("Agent Lifespan", 10, .1, 20));
	gui.add(velocity.setup("velocity", ofVec3f(0, 0, 0), ofVec3f(-1000, -1000, -1000), ofVec3f(1000, 1000, 1000)));
	bHide = false;
	gui.add(speedSlider.setup("Speed", speed, 1, 800));
	gui.add(turret->nAgents.setup("Amount of enemies", 1, 1, 3));
	gui.add(scaleSlider.setup("Sprite Scale", tri.scale1, 1, 10));
	gui.add(tri.showSprite.setup("Show Sprite", true));
	gui.add(showHeading.setup("Show Heading", false));
	
	playerTurret->nAgents = 1;
	//Explosion->nAgents = 1;


}

//--------------------------------------------------------------
void ofApp::update() {

	if (keymap[' ']) {
		//cout << "space";
		if (game_state == "start") {
			game_state = "game";
		}
		else if (game_state == "end") {
			//restarts the game by resetting values
			game_state = "game";
			tri.playerhp = 20;
			tri.pos = glm::vec3(ofGetWindowWidth() / 2.0 - 100, ofGetWindowHeight() / 2.0 - 100, 0);
			//ofResetElapsedTimeCounter();

		}
	}
	else if (keymap['1']) {
		difficulty = "easy";
		rate = 1;
		shootingRate = 2;
		turret->nAgents = 1;
	}
	else if (keymap['2']) {
		difficulty = "medium";
		shootingRate = 3;
		rate = 2;
		turret->nAgents = 2;
	}
	else if (keymap['3']) {
		difficulty = "hard";
		shootingRate = 3;
		rate = 5;
		turret->nAgents = 3;
	}
	else if (keymap['p']) {
		if (game_state == "start" || game_state == "end") {
			game_state = "help";

		}
	}
	else if (keymap['b']){
		if (game_state == "help") {
			game_state = "start";
		}
	}

	if (game_state == "game") {
		turret->setRate(rate);
		turret->setLifespan(life * 1000);    // convert to milliseconds 
		turret->setVelocity(ofVec3f(velocity->x, velocity->y, velocity->z));
		turret->update();
		turret->pos.x = ofRandom(0, ofGetWindowWidth());
		turret->pos.y = ofRandom(0, ofGetWindowHeight());
		playerTurret->pos.x = tri.pos.x;
		playerTurret->pos.y = tri.pos.y;
		//if (keymap[' ']) {
			//cout << "space" << endl;
		
		playerTurret->setRate(shootingRate);
		playerTurret->setLifespan(life * 1000);    // convert to milliseconds 
		playerTurret->setVelocity(ofVec3f(velocity->x, velocity->y, velocity->z));
		playerTurret->update();
		//Explosion->setRate(rate);
		//Explosion->update();
		//}
		//playerTurret->update();
		
	
		//turbulence->set(ofVec3f(turbMin1, turbMin2, 0), ofVec3f(turbMax1, turbMax2, 0));
		attractForce->set(tri.pos);
		tri.scale1 = scaleSlider;
		tri.scale2 = scaleSlider;
		tri.scale3 = scaleSlider;

		//explosion code
		vector<Sprite>::iterator s = playerTurret->particles.begin();
		vector<Sprite>::iterator tmp;
		while (s != playerTurret->particles.end()) {
			if (s->lifespan != -1 && s->age() > s->lifespan) {
				//			cout << "deleting sprite: " << s->name << endl;
				tmp = playerTurret->particles.erase(s);
				s = tmp;
			}
			else {
				s->integrate();
				s++;
			}
		}
		//

		if (keymap[OF_KEY_LEFT])
			tri.rotation = tri.rotation - 2;


		if (keymap[OF_KEY_RIGHT])
			tri.rotation = tri.rotation + 2;

		if (keymap[OF_KEY_UP]) {
			//motor.play();
			newPosUp = tri.pos + tri.heading() * (speed / ofGetFrameRate());
			if (tri.checkCollision(newPosUp)) {
				tri.pos = newPosUp;
			}
			else {
				//tri.pos = tri.pos - tri.heading();
			}
		}

		if (keymap[OF_KEY_DOWN]) {
			newPosDown = tri.pos - tri.heading() * (speed / ofGetFrameRate());
			if (tri.checkCollision(newPosDown)) {
				tri.pos = newPosDown;
			}
			else {

			}
		}
		if (tri.playerhp <= 0) {
			game_state = "end";
			turret->sys->sprites.clear();
		}
	}
	
	

	speed = speedSlider;
}


//--------------------------------------------------------------
void ofApp::draw(){
	if (game_state == "start") {
		ofSetColor(ofColor::white);
		startScreen.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		string startdiff;
		startdiff += "Difficulty: " + difficulty;
		ofDrawBitmapString(startdiff, 0, 150);
		/*
		string startmessage = "Press Space to start the game";
		ofSetColor(ofColor::white);
		ofDrawBitmapString(startmessage, ofGetWindowWidth() / 2, ofGetWindowHeight()/2);
		*/

	}
	else if (game_state == "game") {
		background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		if (showHeading) {
			ofDrawLine(tri.pos, tri.pos + tri.heading() * tri.lineLength);
		}



		turret->draw();
		tri.draw();
		playerTurret->draw();
		//Explosion->draw();
		//explosion code
		vector<Sprite>::iterator s = playerTurret->particles.begin();
		vector<Sprite>::iterator tmp;
		while (s != playerTurret->particles.end()) {
			s->draw();
			s++;
		}
		if (!bHide) {
			gui.draw();
		}
		//


		//screen data
		string frames;
		frames += "Frame Rate: " + std::to_string(ofGetFrameRate());
		ofSetColor(ofColor::white);
		ofDrawBitmapString(frames, ofGetWindowWidth() - 170, 15);
		string level;
		level += "Levels: " + std::to_string(tri.playerhp) + " / " + std::to_string(tri.maxhp);
		ofDrawBitmapString(level, ofGetWindowWidth() / 2, 15);
		time = ofGetElapsedTimef();
		
		string elapsedTime = "Elapsed Time: " + std::to_string(time - timeSubtract);
		ofDrawBitmapString(elapsedTime, ofGetWindowWidth() - 170, 30);

		string diff;
		diff += "Difficulty: " + difficulty;
		ofDrawBitmapString(diff, ofGetWindowWidth() - 170, 45);
	}
	else if (game_state == "end") {
		ofSetColor(ofColor::white);
		//background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		string endmessage = "";
		//finding how long the player played the game for by subtracting the start time from the last session(timeSubtract) with the overall current time (time)
		if ((time - timeSubtract) > 0){
			endtime = time - timeSubtract;
		}
		cout << time - timeSubtract << endl;
		endmessage += "You've survived for " + std::to_string(endtime) + " seconds.";
		endScreen.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		ofDrawBitmapString(endmessage, (ofGetWindowWidth() / 2) - 170, ofGetWindowHeight() / 2);
		//background.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());
		timeSubtract = time;
		
		

	}
	else if (game_state == "help") {
		helpScreen.draw(0, 0, ofGetWindowWidth(), ofGetWindowHeight());


	}
}


//--------------------------------------------------------------

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
//	cout << "mouse( " << x << "," << y << ")" << endl;
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

void ofApp::keyPressed(int key) {
	keymap[key] = true;
	switch (key) {
	case 'C':
	case 'c':
		break;
	case 'F':
	case 'f':
		ofToggleFullscreen();
		break;
	case 'H':
	case 'h':
		bHide = !bHide;
		break;
	case 'r':
		break;
	case 's':
		break;
	case 'u':
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	case OF_KEY_DEL:
		break;
	case OF_KEY_UP:
		motor.play();
		break;
	case OF_KEY_DOWN:
		motor.play();
		break;
	}
}


//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
	keymap[key] = false;
	switch (key) {
	case OF_KEY_LEFT:
	case OF_KEY_RIGHT:
	case OF_KEY_UP:
	case OF_KEY_DOWN:
		break;
	case OF_KEY_ALT:
		break;
	case OF_KEY_CONTROL:
		break;
	case OF_KEY_SHIFT:
		break;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

