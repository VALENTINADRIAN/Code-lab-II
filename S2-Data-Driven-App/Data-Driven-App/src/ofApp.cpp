#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetBackgroundColor(40, 40, 40);
	
	// loads standard text ffonts
	myFont.load(OF_TTF_SANS, 16);
	
	typedText = "";
	displayMessage = "Type a country name and press ENTER";
	isSearching = false;
	gotError = false;
	
	// empty datas for the struct
	currentCountry.name = "";
	currentCountry.capital = "";
	currentCountry.population = "";
	currentCountry.currency = "";
}

//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){
	// main title for the programme
	ofSetColor(255);
	myFont.drawString("Country Facts Finder", 50, 50);
	
	// draws a box for the search input
	ofNoFill();
	ofSetColor(200);
	ofDrawRectangle(50, 80, 450, 40);
	
	ofFill();
	ofSetColor(255);
	
	// the text 'Search:' is displayed inside the search box
	string textToDraw = "Search: " + typedText;
	
	// adds a blinking cursor symbol "|" every 500 milliseconds for easy user comprehension
	if ((ofGetElapsedTimeMillis() / 500) % 2 == 0) {
		textToDraw += "|";
	}
	
	// shows what the user is typing right now with the blinking cursor
	myFont.drawString(textToDraw, 60, 106);
	
	// colour of message changes depending on the status of the search (error, waiting, success)
	if (gotError == true) {
		ofSetColor(255, 100, 100); // red for API error
	} else if (isSearching == true) {
		ofSetColor(255, 200, 100); // yellow for API waiting
	} else {
		ofSetColor(100, 255, 100); // green for API success
	}
	myFont.drawString(displayMessage, 50, 160);
	
	// if country datas found then insert them on screen
	ofSetColor(255);
	if (currentCountry.name != "") {
		myFont.drawString("Country Name: " + currentCountry.name, 50, 220);
		myFont.drawString("Capital City: " + currentCountry.capital, 50, 260);
		myFont.drawString("Population:   " + currentCountry.population, 50, 300);
		myFont.drawString("Currency:     " + currentCountry.currency, 50, 340);
	}
}

//--------------------------------------------------------------
void ofApp::searchCountry(string countryName){
	// check if user typed nothing
	if (countryName == "") {
		gotError = true;
		displayMessage = "Error. A country name must be typed first";
		currentCountry.name = ""; 
		return;
	}
	
	isSearching = true;
	gotError = false;
	displayMessage = "Searching informations for " + countryName + "...";
	currentCountry.name = ""; // clear old datas from screen
	
	// preventing url break due to space in the search term with "%20"
	string safeName = countryName;
	ofStringReplace(safeName, " ", "%20");
	
	// this is the api link for REST Countries
	string url = "https://api.restcountries.com/countries/v5?q=" + safeName;
	
	// ofHttpRequest is used here to send the API key
	ofHttpRequest request(url, "country_request");
	request.headers["Authorization"] = "Bearer rc_live_53088941fd994ec396197492842c2b05";
	
	// loads the url and wait for answer using file loader
	ofURLFileLoader loader;
	ofHttpResponse response = loader.handleRequest(request);
	
	isSearching = false;
	
	// checks if internet connection is good
	if (response.status == 200 || response.data.getText().length() > 0) {
		
		// the text attempted to read as json
		bool checkOpen = json.parse(response.data.getText());
		
		if (checkOpen) {
			// checks if the programme got the "data" object and it has the "objects" array
			if (json.isMember("data") && json["data"].isMember("objects") && json["data"]["objects"].size() > 0) {
				
				gotError = false;
				displayMessage = "Success. Found country informations.";
				
				// the API goes through a loop and finds the exact name match 
				ofxJSONElement exactCountry = json["data"]["objects"][0]; 
				int totalFound = json["data"]["objects"].size();
				
				for (int i = 0; i < totalFound; i++) {
					string foundName = json["data"]["objects"][i]["names"]["common"].asString();
					// checks if the name matches exactly what the user typed (ignores uppercase/lowercase)
					if (ofToLower(foundName) == ofToLower(countryName)) {
						exactCountry = json["data"]["objects"][i];
						break; // searching is stopped because the query found it
					}
				}
				
				// gets country name 
				currentCountry.name = exactCountry["names"]["common"].asString();
				
				// gets capital 
				if (exactCountry.isMember("capitals") && exactCountry["capitals"].size() > 0) {
					currentCountry.capital = exactCountry["capitals"][0]["name"].asString();
				} else {
					currentCountry.capital = "No capital was found";
				}
				
				// gets population
				if (exactCountry.isMember("population")) {
					long long popNumber = exactCountry["population"].asInt64();
					currentCountry.population = ofToString(popNumber);
				} else {
					currentCountry.population = "Unknown";
				}
				
				// gets currency code and name, if there is no currency then it will display "No currency"
				currentCountry.currency = "No currency";
				if (exactCountry.isMember("currencies") && exactCountry["currencies"].size() > 0) {
					string cName = exactCountry["currencies"][0]["name"].asString();
					string cCode = "";
					
					if (exactCountry["currencies"][0].isMember("code")) {
						cCode = exactCountry["currencies"][0]["code"].asString();
					}
					currentCountry.currency = cName + " (" + cCode + ")";
				}
				
			} else if (json.isObject() && json.isMember("errors")) {
				// api send error object if country is fake
				gotError = true;
				if (json["errors"].size() > 0) {
					displayMessage = "API Error: " + json["errors"][0]["message"].asString();
				} else {
					displayMessage = "Error. API cannot find this country.";
				}
			} else {
				gotError = true;
				displayMessage = "Error. API cannot find this country.";
			}
		} else {
			gotError = true;
			displayMessage = "Error. Cannot parse JSON from API.";
		}
	} else {
		// internet problem
		gotError = true;
		displayMessage = "Error. Network fail. Cannot connect to API.";
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	// if user presses the enter key, starts the search through the API call
	if (key == OF_KEY_RETURN) {
		searchCountry(typedText);
	}
	// if user presses backspace, the last letter gets deleted
	else if (key == OF_KEY_BACKSPACE) {
		if (typedText.length() > 0) {
			typedText = typedText.substr(0, typedText.length() - 1);
		}
	}
	else if (key >= 32 && key <= 126) {
		typedText += (char)key;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}
