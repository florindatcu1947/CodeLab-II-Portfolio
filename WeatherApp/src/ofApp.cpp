#include "ofApp.h"
#include <iomanip>
#include <sstream>

// Helper function to safely percent-encode spaces and special characters for URLs
std::string ofApp::urlEncode(const std::string & value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (char c : value) {
		// Keep alphanumeric and other standard safe URL characters
		if (isalnum((unsigned char)c) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
		} else if (c == ' ') {
			escaped << "%20";
		} else {
			escaped << std::uppercase;
			escaped << '%' << std::setw(2) << int((unsigned char)c);
			escaped << std::nouppercase;
		}
	}
	return escaped.str();
}

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Data Driven App - Weather Station");
	ofSetWindowShape(850, 600);
	ofBackground(24, 28, 36);

	// Register async URL response
	ofRegisterURLNotification(this);

	inputCity = "London";
	statusMessage = "Ready. Type a city name and hit Enter or click Search.";
	isLoading = false;
	hasError = false;

	// Setup GUI without the broken setWidth call
	gui.setup("Weather Controls");
	gui.setPosition(20, 20);

	searchButton.addListener(this, &ofApp::searchButtonPressed);
	clearButton.addListener(this, &ofApp::clearButtonPressed);

	gui.add(searchButton.setup("Search"));
	gui.add(clearButton.setup("Clear Input"));

	// Initial weather fetch
	fetchWeather(inputCity);
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	// 1. Draw GUI panel
	gui.draw();

	// 2. Draw Search Input Box
	int boxX = 260;
	int boxY = 20;
	int boxW = 550;
	int boxH = 50;

	ofSetColor(35, 41, 54);
	ofDrawRectRounded(boxX, boxY, boxW, boxH, 8);

	ofSetColor(70, 80, 100);
	ofNoFill();
	ofDrawRectRounded(boxX, boxY, boxW, boxH, 8);
	ofFill();

	ofSetColor(180, 190, 205);
	ofDrawBitmapString("Type a city/country (e.g. New Delhi, Mumbai, Paris):", boxX + 15, boxY + 20);

	ofSetColor(255, 255, 255);
	ofDrawBitmapString("> " + inputCity + "_", boxX + 15, boxY + 40);

	// 3. Status bar
	int statusY = boxY + boxH + 25;
	if (isLoading) {
		ofSetColor(240, 190, 60);
	} else if (hasError) {
		ofSetColor(235, 75, 75);
	} else {
		ofSetColor(80, 200, 120);
	}
	ofDrawBitmapString("Status: " + statusMessage, boxX + 15, statusY);

	// 4. Main Weather Card
	int cardY = statusY + 25;
	int cardH = 430;
	ofSetColor(30, 35, 46);
	ofDrawRectRounded(boxX, cardY, boxW, cardH, 10);

	if (currentWeather.isValid) {
		// Location info
		ofSetColor(255, 255, 255);
		std::string locHeader = currentWeather.city;
		if (!currentWeather.country.empty()) {
			locHeader += ", " + currentWeather.country;
		}
		ofDrawBitmapStringHighlight(locHeader, boxX + 25, cardY + 40, ofColor(50, 60, 80), ofColor::white);

		if (!currentWeather.region.empty()) {
			ofSetColor(160, 170, 185);
			ofDrawBitmapString("Region: " + currentWeather.region, boxX + 25, cardY + 65);
		}

		ofSetColor(140, 150, 165);
		ofDrawBitmapString("Local Time: " + currentWeather.localTime, boxX + 25, cardY + 85);

		// Temperature
		ofSetColor(245, 166, 35);
		std::string tempStr = ofToString(currentWeather.tempC, 1) + " C";
		ofDrawBitmapString("TEMPERATURE:", boxX + 25, cardY + 130);
		ofDrawBitmapString(tempStr, boxX + 140, cardY + 130);

		ofSetColor(200, 200, 210);
		ofDrawBitmapString("Feels Like:  " + ofToString(currentWeather.feelsLikeC, 1) + " C", boxX + 25, cardY + 155);
		ofDrawBitmapString("Condition:   " + currentWeather.conditionText, boxX + 25, cardY + 180);

		// Divider
		ofSetColor(60, 70, 90);
		ofDrawLine(boxX + 25, cardY + 205, boxX + boxW - 25, cardY + 205);

		// Weather Details
		ofSetColor(180, 210, 240);
		ofDrawBitmapString("METRICS & ENVIRONMENT", boxX + 25, cardY + 230);

		ofSetColor(220, 225, 235);
		ofDrawBitmapString("Humidity:     " + ofToString(currentWeather.humidity) + "%", boxX + 25, cardY + 265);
		ofDrawBitmapString("Wind Speed:   " + ofToString(currentWeather.windKph, 1) + " km/h", boxX + 25, cardY + 295);
		ofDrawBitmapString("UV Index:     " + ofToString(currentWeather.uv, 1), boxX + 25, cardY + 325);

		// Humidity bar
		ofSetColor(50, 60, 75);
		ofDrawRectRounded(boxX + 180, cardY + 253, 200, 14, 4);
		ofSetColor(70, 140, 230);
		float barWidth = ofMap(currentWeather.humidity, 0, 100, 0, 200, true);
		ofDrawRectRounded(boxX + 180, cardY + 253, barWidth, 14, 4);

	} else if (!isLoading) {
		ofSetColor(150, 155, 170);
		ofDrawBitmapString("No weather data found.", boxX + 30, cardY + 60);
		ofDrawBitmapString("Please check your spelling and search again.", boxX + 30, cardY + 85);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RETURN) {
		fetchWeather(inputCity);
	} else if (key == OF_KEY_BACKSPACE) {
		if (!inputCity.empty()) {
			inputCity.pop_back();
		}
	} else if (key >= 32 && key <= 126) {
		// Append typed characters
		inputCity += static_cast<char>(key);
	}
}

//--------------------------------------------------------------
void ofApp::fetchWeather(const std::string & queryCity) {
	std::string trimmedCity = ofTrim(queryCity);
	if (trimmedCity.empty()) {
		hasError = true;
		statusMessage = "Validation Error: Please enter a city or location name.";
		return;
	}

	isLoading = true;
	hasError = false;
	statusMessage = "Fetching weather for \"" + trimmedCity + "\"...";

	// URL encode city string to safely support spaces like "New Delhi"
	std::string encodedQuery = urlEncode(trimmedCity);
	std::string url = "https://api.weatherapi.com/v1/current.json?key=" + apiKey + "&q=" + encodedQuery;

	ofLoadURLAsync(url, "weatherRequest");
}

//--------------------------------------------------------------
void ofApp::urlResponse(ofHttpResponse & response) {
	isLoading = false;

	if (response.status != 200) {
		hasError = true;
		currentWeather.isValid = false;

		try {
			if (!response.data.getText().empty()) {
				auto json = ofJson::parse(response.data.getText());
				if (json.contains("error") && json["error"].contains("message")) {
					statusMessage = "API Error: " + json["error"]["message"].get<std::string>();
					return;
				}
			}
		} catch (...) {
			// parsing error fallback
		}

		statusMessage = "Error (Code: " + ofToString(response.status) + "): " + (response.error.empty() ? "Request failed" : response.error);
		return;
	}

	parseWeatherData(response.data.getText());
}

//--------------------------------------------------------------
void ofApp::parseWeatherData(const std::string & jsonString) {
	try {
		if (jsonString.empty()) {
			hasError = true;
			statusMessage = "Error: Empty response received.";
			return;
		}

		auto json = ofJson::parse(jsonString);

		if (json.contains("location") && json.contains("current")) {
			currentWeather.city = json["location"].value("name", "Unknown");
			currentWeather.region = json["location"].value("region", "");
			currentWeather.country = json["location"].value("country", "Unknown");
			currentWeather.localTime = json["location"].value("localtime", "N/A");

			auto current = json["current"];
			currentWeather.tempC = current.value("temp_c", 0.0f);
			currentWeather.feelsLikeC = current.value("feelslike_c", 0.0f);
			currentWeather.humidity = current.value("humidity", 0);
			currentWeather.windKph = current.value("wind_kph", 0.0f);
			currentWeather.uv = current.value("uv", 0.0f);

			if (current.contains("condition") && current["condition"].contains("text")) {
				currentWeather.conditionText = current["condition"]["text"].get<std::string>();
			} else {
				currentWeather.conditionText = "N/A";
			}

			currentWeather.isValid = true;
			hasError = false;
			statusMessage = "Success: Loaded weather for " + currentWeather.city + ", " + currentWeather.country;
		} else {
			hasError = true;
			currentWeather.isValid = false;
			statusMessage = "Data Error: Unexpected API response format.";
		}
	} catch (const std::exception & e) {
		hasError = true;
		currentWeather.isValid = false;
		statusMessage = std::string("JSON Parsing Error: ") + e.what();
	}
}

//--------------------------------------------------------------
void ofApp::searchButtonPressed() {
	fetchWeather(inputCity);
}

//--------------------------------------------------------------
void ofApp::clearButtonPressed() {
	inputCity.clear();
	statusMessage = "Input cleared. Enter a location name.";
}
