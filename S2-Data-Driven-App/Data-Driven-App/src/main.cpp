#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	// app window size is set to the FHD resolution
	ofSetupOpenGL(1920, 1080, OF_WINDOW);
	// starts the app
	ofRunApp(new ofApp());
}
