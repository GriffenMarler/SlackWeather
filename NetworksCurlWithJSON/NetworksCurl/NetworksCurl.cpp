// NetworksCurl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define CURL_STATICLIB
#include <iostream>
#include <string>
#include "curl/curl.h"
#ifdef _DEBUG
#pragma comment (lib, "curl/libcurl_a_debug.lib")
#else
#pragma comment (lib, "curl/libcurl_a.lib")
#endif
#pragma comment (lib, "Normaliz.lib")
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Wldap32.lib")
#pragma comment (lib, "Crypt32.lib")
#pragma comment (lib, "advapi32.lib")
#include"slacking/slacking.hpp"
#include "slacking/json.hpp"
#include "MessageBlock.h"
using std::cout;
using std::endl;


/*
//////// Resources Used /////////

 - Creating stylized messages with slack block builder tool: https://api.slack.com/tools/block-kit-builder?mode=message&blocks=%5B%5D
 - Defining the required WriteCallBack function used in the WeatherBit API options: https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
 - RapidAPI link to WeatherBit IO API where endpoints were tested and starter code was referenced: https://rapidapi.com/weatherbit/api/weather
 - Slacking library used to create slack BOT that can send messages to designated workspace: https://github.com/coin-au-carre/slacking
 - libCurl library used to easily format HTTP request message that were sent to WeatherBit API: https://curl.haxx.se/libcurl/c/libcurl.html
 - libCurl youtube tutorial used to help set up the library using headers: https://www.youtube.com/watch?v=q_mXVZ6VJs4
 - JSON library used to parse data received from WeatherBit and form messages to send to slack: https://github.com/nlohmann/json

*/

//////// function prototypes ///////
nlohmann::json getFiveDayForecast();
nlohmann::json getCurrentWeatherData();
void sendSlackMessage(MessageBlock Block, string channelName);
static size_t WriteCallback(void*, size_t, size_t, string*);

//////// global variables ////////

// This is the Oauth access token needed for the slack API. The channel workspace is also specified when generating this token
// WARNING: if posted to public domain such as github, slack will revoke access and another token will need to be generated
string slackAPIBotKey = "";

// This is the Oauth access token needed for the weatherbit API hosted on the rapid API platform
// Recommended to take this out when uploading to public domain as a security measure
const char* rapidAPIKey = "";



int main()
{
	
	while (true) {
		///////////////////////////////////////////////////////////
		// PART 1: Severe Weather Alerts
		///////////////////////////////////////////////////////////

		//store the raw JSON holding current weather data from WeatherBit
		nlohmann::json currentWeatherJSON = getCurrentWeatherData();

		// Move into the data section of the json
		nlohmann::json currentWeatherDataJSON = currentWeatherJSON.at("/data"_json_pointer);

		// Retrieve city, snow, UV, and Wind_spd from the current weather data
		string snow = currentWeatherDataJSON.at("/snow"_json_pointer).dump();
		string UV = currentWeatherDataJSON.at("/uv"_json_pointer).dump();
		string wind_spd = currentWeatherDataJSON.at("/wind_spd"_json_pointer).dump();
		string ConditionCity = currentWeatherDataJSON.at("/city_name"_json_pointer).dump();

		// only send a severe weather update if certain conditions are met
		if (std::stoi(snow) > 0 || std::stoi(UV) > 3 || std::stoi(wind_spd) > 50) {

			// Get rid of extra quotes that are effecting JSON parsing
			ConditionCity.erase(std::remove(ConditionCity.begin(), ConditionCity.end(), '"'));
				ConditionCity.erase(std::remove(ConditionCity.begin(), ConditionCity.end(), '"'));

				// using weather conditions as a seed, generate a new message block
				MessageBlock SevereWeatherCheck(snow, UV, wind_spd, ConditionCity);

				// This JSON object is used to create stylized message blocks in slack
				// https://api.slack.com/tools/block-kit-builder?mode=message&blocks=%5B%5D
				// Critical weather alert attachment string
				string SevereWeatherTemplate = R"([
			   {
			    "fallback": "Required plain-text summary of the attachment.",
			    "color": "#86C5DA",
				"pretext": "Critical Weather Alert reported in area",
			    "title_link": "https://api.slack.com/",
			    "text":" ",
			    "thumb_url": "",
			    "footer": "Powered by Weatherbit IO and Slack API",
			    "footer_icon": "https://kodi.tv/sites/default/files/styles/medium_crop/public/addon_assets/weather.weatherbit.io/icon/icon.png?itok=1bUxPgiD"
			  }
			  ])";
 
			// pass the block template string to our Message Block object
			SevereWeatherCheck.setJSONtemplate(SevereWeatherTemplate);
			// merge template with the weather conditons
			SevereWeatherCheck.fillConditionTemplate();

			// Send severe weather message to slack
			sendSlackMessage(SevereWeatherCheck, "#severe_weather_alerts");
		}


		////////////////////////////////////////////////////////////
		// PART 2: Daily Temperature Forecast with 3 hour intervals
		////////////////////////////////////////////////////////////

		// send a GET request to the WeatherBit API and store response as a JSON
		nlohmann::json FiveDayJSON = getFiveDayForecast();

		// parse Forecast JSON  to get the city name
		string city = FiveDayJSON.at("/city_name"_json_pointer).dump();
		city.erase(std::remove(city.begin(), city.end(), '"'));
		city.erase(std::remove(city.begin(), city.end(), '"'));

		// create a new message block to display the daily temperature forecast for specified city
		MessageBlock DailyTempForecast;
		DailyTempForecast.setCityName(city);

		//cout << endl << DailyTempForecast.getCityName() << endl;

		// Holds an array of 40 weather objects. Each object holds data for a forecast 3 hours apart
		nlohmann::json WeatherDataArray = FiveDayJSON.at("/data"_json_pointer);


		nlohmann::json DailyTemperature;
		std::string TemperatureString;

		// Retrieve the forecast from 7AM - 1AM every three hours and add it to our MessageBlock (7AM, 10AM, 1PM, 4PM, 7PM, 10PM, 1AM for a total of 7)
		for (int i = 2; i < 9; i++) {
			// New JSON object for each 3 hour interval
			DailyTemperature = WeatherDataArray.at(i);
			// Push back the value stored at temperature
			TemperatureString = DailyTemperature.at("/temp"_json_pointer).dump();
			DailyTempForecast.addTemperature(TemperatureString);
		}


		//  Parse the weather data to store forecast date in message block
		std::string DateString;
		nlohmann::json ForecastDate;
		ForecastDate = WeatherDataArray.at(0);
		DateString = ForecastDate.at("/timestamp_utc"_json_pointer).dump();
		// erase quotes and current time to get date in format: Year-month-day  e.g. 2019-12-10
		DateString.erase(std::remove(DateString.begin(), DateString.end(), '"'));
		DateString.erase(std::remove(DateString.begin(), DateString.end(), '"'));
		DateString.erase(DateString.begin() + 10, DateString.end());
		// pass date into message block object
		DailyTempForecast.setDate(DateString);



		// This JSON object is used to create stylized message blocks in slack
		// https://api.slack.com/tools/block-kit-builder?mode=message&blocks=%5B%5D
		// Daily temperature forecast attachment string
		std::string ForecastBlockTemplate = R"([
		{
            "fallback": "Required plain-text summary of the attachment .",
            "color": "#36a64f",
            "pretext": "Daily Weather Report for on",
            "title_link": "https://api.slack.com/",
            "text": "Time:    \t7AM\t10AM\t1PM\t4PM\t7PM \t10PM\t1AM\nTemp(\u2109):",
            "fields": [
            ],
            "image_url": "http://my-website.com/path/to/image.jpg",
            "thumb_url": "http://example.com/path/to/thumb.png",
            "footer": "Powered by Weatherbit IO and Slack API",
            "footer_icon": "https://kodi.tv/sites/default/files/styles/medium_crop/public/addon_assets/weather.weatherbit.io/icon/icon.png?itok=1bUxPgiD"
            
		}
		])";

		//pass the block template string to our Message Block object
		DailyTempForecast.setJSONtemplate(ForecastBlockTemplate);
		//populate message block with temperatures
		DailyTempForecast.fillTemplate();
		// send stylized forecast message to the daily_forecast channel
		sendSlackMessage(DailyTempForecast, "#daily_forecast");




		std::this_thread::sleep_for(std::chrono::seconds(10));

	}
	return 0;
}


// This function creates a slack API connection and sends a stylized message block to a specific channel
// Input: Block - the message block object that holds the stylized message
//		  channelName - the channel name that the message will be sent to
void sendSlackMessage(MessageBlock Block, string channelName) {

	// setup slack API session with the bot Oauth access token
	auto& ConditionSlack = slack::create(slackAPIBotKey);
	// specify sender information along with desired channel to send message to
	ConditionSlack.chat.channel_username_iconemoji(channelName, "Weather Alert Bot", ":ghost");
	//std::cout << Block.getJSONtemplate() << endl;

	// create a JSON object from the string template in our message block and add it to our slack message
	auto Condition_attachments = nlohmann::json::parse(Block.getJSONtemplate());
	nlohmann::json ConditionDump = Condition_attachments;

	// Output for debugging purposes
	//std::cout << "Condition Dump:" << std::endl;
	//std::cout << std::endl;
	//std::cout << ConditionDump.dump(4) << std::endl;

	ConditionSlack.chat.attachments = Condition_attachments;

	// post the message to slack and store the response message
	auto ConditionResponse = ConditionSlack.chat.postMessage();
	//std::cout << ConditionResponse << endl << endl;

}


// this function sets WeatherBit API options and makes a GET request for a 5 day forecast
// A JSON object containing the current weather is returned
nlohmann::json getFiveDayForecast() {

	// create a string to hold data
	std::string readBuffer;

	////initializes a curl setup
	CURL* hnd = curl_easy_init();

	////sets the options of the curl object to send a get request with latitude and longitude paramaters from Spokane
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_URL, "https://weatherbit-v1-mashape.p.rapidapi.com/forecast/3hourly?units=I&lang=en&lat=47.66&lon=-117.42");
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, &WriteCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);
	curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

	//// adds to the header of the curl item which api, and api-key is being used
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "x-rapidapi-host: weatherbit-v1-mashape.p.rapidapi.com");
	headers = curl_slist_append(headers, rapidAPIKey);
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

	// send the request and store the response code
	CURLcode ret = curl_easy_perform(hnd);

	// cleanup the curl pointer
	curl_easy_cleanup(hnd);

	// Insert the entire HTTP response message into a JSON object and return it
	return nlohmann::json::parse(readBuffer);
}




// this function sets WeatherBit API options and makes a GET request for the current weather
// A JSON object containing the current weather is returned
nlohmann::json getCurrentWeatherData() {

	// create a string to hold data
	std::string ConditionreadBuffer;

	////initializes a curl setup
	CURL* Condition = curl_easy_init();

	////sets the options of the curl object to send a get request with latitude and longitude paramaters from Spokane
	curl_easy_setopt(Condition, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(Condition, CURLOPT_URL, "https://weatherbit-v1-mashape.p.rapidapi.com/current?units=I&lang=en&lon=-117.42&lat=47.66");
	curl_easy_setopt(Condition, CURLOPT_WRITEFUNCTION, &WriteCallback);
	curl_easy_setopt(Condition, CURLOPT_WRITEDATA, &ConditionreadBuffer);
	curl_easy_setopt(Condition, CURLOPT_VERBOSE, 1L); //tell curl to output its progress

	//// adds to the header of the curl item which api, and api-key is being used
	struct curl_slist* Conditionheaders = NULL;
	Conditionheaders = curl_slist_append(Conditionheaders, "x-rapidapi-host: weatherbit-v1-mashape.p.rapidapi.com");
	Conditionheaders = curl_slist_append(Conditionheaders, "x-rapidapi-key: 012fd474bamshbc8b59afe42a55ap1dd094jsn4a4772f6f401");
	curl_easy_setopt(Condition, CURLOPT_HTTPHEADER, Conditionheaders);

	// send the request and store the response code
	CURLcode Conditionret = curl_easy_perform(Condition);

	// cleanup the curl pointer
	curl_easy_cleanup(Condition);

	// Get rid of the extra [ and ] that were affecting JSON parsing due to only a single entity being returned
	ConditionreadBuffer.erase(std::remove(ConditionreadBuffer.begin(), ConditionreadBuffer.end(), '['));
	ConditionreadBuffer.erase(std::remove(ConditionreadBuffer.begin(), ConditionreadBuffer.end(), ']'));

	// Put the entire response into a JSON object and return it
	return nlohmann::json::parse(ConditionreadBuffer);
}




// This function is used to write the data that the WeatherBit API returns to a string
// heavily referenced https://gist.github.com/alghanmi/c5d7b761b2c9ab199157 in order to define WriteCallBack function
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}





