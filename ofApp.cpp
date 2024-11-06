#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetColor(39);
	ofEnableDepthTest();

	this->ico_sphere = ofIcoSpherePrimitive(100, 2);
	this->noise_param = ofRandom(1000);
}

//--------------------------------------------------------------
void ofApp::update() {

	if (ofGetFrameNum() % 50 < 20) {

		this->noise_param += ofMap(ofGetFrameNum() % 50, 0, 20, 0.2, 0);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.25);

	ofSetColor(39);
	ofSetLineWidth(3);
	ofNoFill();

	auto tmp_noise_param = this->noise_param;

	ofBeginShape();
	for (int x = 0; x <= 330; x += 1) {

		if (x > 0 && (ofGetFrameNum() + x) % 50 < 20) {

			tmp_noise_param += ofMap((ofGetFrameNum() + x) % 50, 0, 20, 0.2, 0);
		}

		auto y = ofMap(ofNoise(tmp_noise_param), 0, 1, -100, 100);
		ofVertex(x, y);
	}
	ofEndShape();

	ofDrawLine(glm::vec2(0, -100), glm::vec2(0, 100));
	ofDrawLine(glm::vec2(330, -100), glm::vec2(330, 100));

	auto y = ofMap(ofNoise(this->noise_param), 0, 1, -100, 100);
	ofFill();
	ofDrawCircle(0, y, 8);

	ofTranslate(-180, 0);
	ofSetColor(39);
	ofSetLineWidth(1.5);

	ofRotateX(ofGetFrameNum() * 0.77);
	ofRotateY(ofGetFrameNum() * 0.39);

	auto triangle_list = this->ico_sphere.getMesh().getUniqueFaces();
	for (auto& triangle : triangle_list) {

		glm::vec3 avg = (triangle.getVertex(0) + triangle.getVertex(1) + triangle.getVertex(2)) / 3;
		auto noise_value = ofNoise(glm::vec4(avg * 0.015, this->noise_param));
		auto noise_radius = 0;

		if (noise_value < 0.65) {

			noise_radius = 80;
		}
		else {

			noise_radius = ofMap(noise_value, 0.65, 1.0, 80, 160);
		}

		ofMesh mesh;

		mesh.addVertex(glm::normalize(triangle.getVertex(0)) * noise_radius);
		mesh.addVertex(glm::normalize(triangle.getVertex(1)) * noise_radius);
		mesh.addVertex(glm::normalize(triangle.getVertex(2)) * noise_radius);

		avg = (mesh.getVertex(0) + mesh.getVertex(1) + mesh.getVertex(2)) / 3;

		ofSetColor(239);
		mesh.draw();

		ofSetColor(39);
		mesh.drawWireframe();

		ofDrawLine(glm::vec3(), avg);
	}

	ofPopMatrix();

	ofPushMatrix();
	ofTranslate(ofGetWindowWidth() * 0.5, ofGetWindowHeight() * 0.75);

	ofSetColor(39);
	ofSetLineWidth(3);
	ofNoFill();

	tmp_noise_param = this->noise_param;

	ofBeginShape();
	for (int x = 0; x <= 330; x += 1) {

		if (x > 0 && (ofGetFrameNum() + x) % 50 < 20) {

			tmp_noise_param += ofMap((ofGetFrameNum() + x) % 50, 0, 20, 0.2, 0);
		}

		y = ofMap(ofNoise(tmp_noise_param), 0, 1, -100, 100);
		ofVertex(x, y);
	}
	ofEndShape();

	ofDrawLine(glm::vec2(0, -100), glm::vec2(0, 100));
	ofDrawLine(glm::vec2(330, -100), glm::vec2(330, 100));

	y = ofMap(ofNoise(this->noise_param), 0, 1, -100, 100);
	ofFill();
	ofDrawCircle(0, y, 8);

	ofTranslate(-180, 0);
	ofSetColor(39);
	ofSetLineWidth(1.5);

	ofRotateX(ofGetFrameNum() * 0.77);
	ofRotateY(ofGetFrameNum() * 0.39);

	this->mesh.clear();

	float span = 1;
	ofColor color;
	float len = 150;

	float angle;
	glm::highp_mat4 rotation;
	for (float i = 0; i <= 6; i++) {

		for (float x = len * -0.5; x < len * 0.5; x += span) {

			for (float y = len * -0.5; y < len * 0.5; y += span) {

				if (i <= 4) {

					angle = PI * 0.5 * i;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(0, 1, 0));
				}

				if (i == 5) {

					angle = PI * 0.5;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
				}

				if (i == 6) {

					angle = PI * 0.5 * 3;
					rotation = glm::rotate(glm::mat4(), angle, glm::vec3(1, 0, 0));
				}

				glm::vec3 noise_location = glm::vec4(x, y, len * 0.5, 0) * rotation;
				int noise_value = ofMap(ofNoise(noise_location.x * 0.005, noise_location.y * 0.005, noise_location.z * 0.005, this->noise_param), 0, 1, 0, 26);

				if (noise_value % 2 == 0) {

					continue;
				}

				ofColor color(239);

				vector<glm::vec3> vertices;
				vertices.push_back(glm::vec4(x, y, len * 0.5, 0) * rotation);
				vertices.push_back(glm::vec4(x + span, y, len * 0.5, 0) * rotation);
				vertices.push_back(glm::vec4(x + span, y + span, len * 0.5, 0) * rotation);
				vertices.push_back(glm::vec4(x, y + span, len * 0.5, 0) * rotation);
				this->mesh.addVertices(vertices);

				this->mesh.addColor(color); this->mesh.addColor(color);
				this->mesh.addColor(color); this->mesh.addColor(color);

				this->mesh.addIndex(this->mesh.getNumVertices() - 1); this->mesh.addIndex(this->mesh.getNumVertices() - 4); this->mesh.addIndex(this->mesh.getNumVertices() - 3);
				this->mesh.addIndex(this->mesh.getNumVertices() - 1); this->mesh.addIndex(this->mesh.getNumVertices() - 2); this->mesh.addIndex(this->mesh.getNumVertices() - 3);
			}
		}
	}

	this->mesh.draw();

	ofFill();
	ofSetColor(39);
	ofDrawBox(149);

	ofPopMatrix();

	/*
	int start = 1;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}