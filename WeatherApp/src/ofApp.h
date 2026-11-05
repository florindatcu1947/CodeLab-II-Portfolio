#pragma once

#include "ofMain.h"
#include "ofxGui.h"

struct WeatherData {
	std::string city;
	std::string region;
	std::string country;
	std::string conditionText;
	std::string localTime;
	float tempC = 0.0f;
	float feelsLikeC = 0.0f;
	int humidity = 0;
	float windKph = 0.0f;
	float uv = 0.0f;
	bool isValid = false;
};

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;
	void keyPressed(int key) override;

	// Asynchronous network response callback
	void urlResponse(ofHttpResponse & response);

	// Fetch and parsing
	void fetchWeather(const std::string & queryCity);
	void parseWeatherData(const std::string & jsonString);
	std::string urlEncode(const std::string & value);

	// WeatherAPI Configuration
	const std::string apiKey = "4b6f9d75c7e54ee1a6b74259260511";

	// Application state
	WeatherData currentWeather;
	std::string inputCity;
	std::string statusMessage;
	bool isLoading;
	bool hasError;

	// GUI Elements
	ofxPanel gui;
	ofxButton searchButton;
	ofxButton clearButton;

	void searchButtonPressed();
	void clearButtonPressed();
};
