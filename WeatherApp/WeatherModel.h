#pragma once
#include <string>

class WeatherModel {
private:
	std::string cityName;
	std::string countryName;
	std::string conditionText;
	float temperatureC;
	float feelsLikeC;
	int humidity;
	float windKph;
	std::string lastUpdated;
	int conditionCode;

public:
	WeatherModel()
		: cityName("")
		, countryName("")
		, conditionText("")
		, temperatureC(0.0f)
		, feelsLikeC(0.0f)
		, humidity(0)
		, windKph(0.0f)
		, lastUpdated("")
		, conditionCode(1000) { }

	// Setters
	void setCityName(const std::string & name) { cityName = name; }
	void setCountryName(const std::string & country) { countryName = country; }
	void setConditionText(const std::string & condition) { conditionText = condition; }
	void setTemperatureC(float temp) { temperatureC = temp; }
	void setFeelsLikeC(float feels) { feelsLikeC = feels; }
	void setHumidity(int hum) { humidity = hum; }
	void setWindKph(float wind) { windKph = wind; }
	void setLastUpdated(const std::string & time) { lastUpdated = time; }
	void setConditionCode(int code) { conditionCode = code; }

	// Getters
	std::string getCityName() const { return cityName; }
	std::string getCountryName() const { return countryName; }
	std::string getConditionText() const { return conditionText; }
	float getTemperatureC() const { return temperatureC; }
	float getFeelsLikeC() const { return feelsLikeC; }
	int getHumidity() const { return humidity; }
	float getWindKph() const { return windKph; }
	std::string getLastUpdated() const { return lastUpdated; }
	int getConditionCode() const { return conditionCode; }

	bool isRainy() const {
		return (conditionCode >= 1180 && conditionCode <= 1201) || conditionCode == 1063;
	}

	bool isSunny() const {
		return conditionCode == 1000;
	}

	bool isSnowy() const {
		return (conditionCode >= 1210 && conditionCode <= 1225) || conditionCode == 1066;
	}
};
