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
#include <unordered_map>
#include "MessageBlock.h"



// heavily referenced https://gist.github.com/alghanmi/c5d7b761b2c9ab199157 in order to define WriteCallBack function
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}



int main()
{
	// create a string to hold data
	std::string readBuffer;
	nlohmann::json responseJSON;
	nlohmann::json FiveDayJSON;

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
	headers = curl_slist_append(headers, "x-rapidapi-key: 012fd474bamshbc8b59afe42a55ap1dd094jsn4a4772f6f401");
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

	CURLcode ret = curl_easy_perform(hnd);

	curl_easy_cleanup(hnd);

	
	// String value to test with so we do not have to use API
	//std::string teststring = "{\"data\": [{\"rh\":86,\"pod\" : \"n\",\"lon\" : -117.42,\"pres\" : 950.8,\"timezone\" : \"America\/Los_Angeles\",\"ob_time\" : \"2019-12-06 01:00\",\"country_code\" : \"US\",\"clouds\" : 100,\"ts\" : 1575594000,\"solar_rad\" : 0,\"state_code\" : \"WA\",\"city_name\" : \"Spokane\",\"wind_spd\" : 1.54,\"last_ob_time\" : \"2019-12-06T01:00:00\",\"wind_cdir_full\" : \"east-northeast\",\"wind_cdir\" : \"ENE\",\"slp\" : 1022.3,\"vis\" : 5,\"h_angle\" : -90,\"sunset\" : \"23:58\",\"dni\" : 0,\"dewpt\" : 3.7,\"snow\" : 0,\"uv\" : 0,\"precip\" : 0,\"wind_dir\" : 60,\"sunrise\" : \"15:22\",\"ghi\" : 0,\"dhi\" : 0,\"aqi\" : 14,\"lat\" : 47.66,\"weather\" : {\"icon\":\"c02n\",\"code\" : \"802\",\"description\" : \"Scattered clouds\"},\"datetime\" : \"2019-12-06:01\",\"temp\" : 5.9,\"station\" : \"2306P\",\"elev_angle\" : -10.04,\"app_temp\" : 4.9}] ,\"count\" : 1 }";

	// Get rid of [ and ] brackets that were causing the JSON library to not correctly parse the data.
	//readBuffer.erase(std::remove(readBuffer.begin(), readBuffer.end(), '['));
	//readBuffer.erase(std::remove(readBuffer.begin(), readBuffer.end(), ']'));

	// Insert the entire HTTP response message into a JSON object
	FiveDayJSON = nlohmann::json::parse(readBuffer);


	// Holds an array of 40 weather objects. Each object holds data for a forecast 3 hours apart
	nlohmann::json WeatherDataArray = FiveDayJSON.at("/data"_json_pointer);


	MessageBlock DailyTempForecast;
	nlohmann::json DailyTemperature;
	std::string TemperatureString;

	// Retrieve the forecast from 6AM - Midnight every three hours and add it to our MessageBlock (6AM, 9AM, 12PM, 3PM, 6PM, 9PM, 12AM for a total of 7)
	for (int i = 0; i < 7; i++) {
		// New JSON object for each 3 hour interval
		DailyTemperature = WeatherDataArray.at(i);
		// Push back the value stored at temperature
		TemperatureString = DailyTemperature.at("/temp"_json_pointer).dump();
		DailyTempForecast.addTemperature(TemperatureString);
	}

	std::string ForecastBlockTemplate = R"([
	{
            "fallback": "Required plain-text summary of the attachment.",
            "color": "#36a64f",
            "pretext": "Daily Weather Report as of",
            "title_link": "https://api.slack.com/",
            "text": "Time:   6AM\t9AM\t12PM\t3PM\t6PM\t9PM\t12AM\nTemp:\t t1\t \tt2\t\t t3\t\t t4 \t\t t5 \t   t6\t\t t7",
            "fields": [
                {
                    "title": "Priority",
                    "value": "High",
                    "short": false
                }
            ],
            "image_url": "http://my-website.com/path/to/image.jpg",
            "thumb_url": "http://example.com/path/to/thumb.png",
            "footer": "Slack API",
            "footer_icon": "https://platform.slack-edge.com/img/default_application_icon.png"
            
        }
])";



	// Start slack session with bot token
	auto& slack = slack::create("xoxb-862360242919-862955384838-QHOKNT5eajwnBjE8kxWlITzR");

	// Set username and channel
	slack.chat.channel_username_iconemoji("#project", "Weather Bot", ":hamster:");

	// Create Attachment Block
	auto json_attachments = nlohmann::json::parse(ForecastBlockTemplate);

	nlohmann::json AttachmentDump = json_attachments;

	std::cout << "Attachment Dump:" << std::endl;
	std::cout << std::endl;
	std::cout << AttachmentDump.dump() << std::endl;
	
	

	
	slack.chat.attachments = json_attachments;
	auto response = slack.chat.postMessage();
	std::cout << response << std::endl << std::endl;
	
	// "temp"   

	/*// Grab all of the key value pairs inside the "data" section
	responseJSON = responseJSON.at("/data"_json_pointer);

	std::cout << responseJSON.dump() << std::endl;
	std::cout << responseJSON.size() << std::endl;

	// Get the value for "app_temp" and store it inside responseJSON
	responseJSON = responseJSON.at("/app_temp"_json_pointer);
	std::cout << responseJSON.dump();

	// Get the stored app_temp value as a string
	std::string temperature = responseJSON.dump();

	std::cout << temperature << std::endl << std::endl;




	//std::string a = "[{"color":"#36a64f","fallback":"Required plain - text summary of the attachment.","fields":[{"short":false,"title":"Priority","value":"High"}],"footer":"Slack API","footer_icon":"https://platform.slack-edge.com/img/default_application_icon.png","image_url":"http://my-website.com/path/to/image.jpg","pretext":"Daily Weather Report as of","text":"Time:   6AM\t9AM\t12PM\t3PM\t6PM\t9PM\t12AM\nTemp:\t t1\t \tt2\t\t t3\t\t t4 \t\t t5 \t   t6\t\t t7","thumb_url":"http://example.com/path/to/thumb.png","title_link":"https://api.slack.com/"}]"

	

	

	std::cout << json_attachments.dump() << std::endl << std::endl;




	
*/
	system("pause");
	return 0;

}

















//	std::cout << responseJSON.dump();
//
//	auto it_initialresponse = responseJSON.count("data");
//std::cout << "Value at initial:" << it_initialresponse << std::endl;
//
//// https://nlohmann.github.io/json/classnlohmann_1_1basic__json_a8ab61397c10f18b305520da7073b2b45.html#a8ab61397c10f18b305520da7073b2b45
//// Parse out just the data section
//
//	std::cout << responseJSON.at("/data/app_temp"_json_pointer) << std::endl;
//
//
//
//// Set the data section = to a new JSON object.
//	nlohmann::json newJSON = responseJSON.at("/data"_json_pointer);
//
//	std::cout << newJSON.dump(4);
//
//	auto app_temp = newJSON.find("\"app_temp\"");
//	std::cout << "app temp: " << *app_temp << std::endl;

//nlohmann::json newJSON = nlohmann::json::object({ teststring });

//std::cout << newJSON.dump(4) << std::endl;
//std::cout << newJSON.size();
//
//	std::string conversion = newJSON.dump();
////
//	auto j3 = nlohmann::json::parse(conversion);
//
//	j3[""]
//
//
//auto it_check = FinalJSON.count("app_temp");
//std::cout << "Value at temp:" << it_check << std::endl;




	//auto it_data = responseJSON.find("data");
	//std::string data = it_data.value;
	//std::cout << data << std::endl;

