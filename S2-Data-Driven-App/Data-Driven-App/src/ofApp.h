#pragma once

#include "ofMain.h"
#include "ofxJSON.h"

// a structure is used to hold all country datas together
struct CountryInfo {
	string name;
	string capital;
	string population;
	string currency;
};

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		
		// this function is to make the api search possible, it will be called when user press enter key
		void searchCountry(string countryName);
		
		ofxJSONElement json; // stores data from API
		
		// variables for user inputs and messages
		string typedText;
		string displayMessage;
		bool isSearching;
		bool gotError;
		
		// uses the struct here
		CountryInfo currentCountry;
		
		ofTrueTypeFont myFont;
};
