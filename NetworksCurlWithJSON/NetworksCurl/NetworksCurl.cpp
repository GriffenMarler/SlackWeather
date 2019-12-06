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



// heavily referenced https://gist.github.com/alghanmi/c5d7b761b2c9ab199157 in order to define WriteCallBack function
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp)
{
	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}



int main()
{
	// create a string to hold data
	//std::string readBuffer;
	nlohmann::json responseJSON;
	nlohmann::json FinalJSON;

	////initializes a curl setup
	//CURL* hnd = curl_easy_init();

	////sets the options of the curl object to send a get request with latitude and longitude paramaters from Spokane
	//curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	//curl_easy_setopt(hnd, CURLOPT_URL, "https://weatherbit-v1-mashape.p.rapidapi.com/current?lang=en&lon=-117.420648&lat=47.658779");
	//curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, &WriteCallback);
	//curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

	////curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L); //tell curl to output its progress


	//// adds to the header of the curl item which api, and api-key is being used
	//struct curl_slist* headers = NULL;
	//headers = curl_slist_append(headers, "x-rapidapi-host: weatherbit-v1-mashape.p.rapidapi.com");
	//headers = curl_slist_append(headers, "x-rapidapi-key: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	//curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);


	//CURLcode ret = curl_easy_perform(hnd);

	//curl_easy_cleanup(hnd);

	
	// String value to test with so we do not have to use API
	std::string teststring = "{\"data\": [{\"rh\":86,\"pod\" : \"n\",\"lon\" : -117.42,\"pres\" : 950.8,\"timezone\" : \"America\/Los_Angeles\",\"ob_time\" : \"2019-12-06 01:00\",\"country_code\" : \"US\",\"clouds\" : 100,\"ts\" : 1575594000,\"solar_rad\" : 0,\"state_code\" : \"WA\",\"city_name\" : \"Spokane\",\"wind_spd\" : 1.54,\"last_ob_time\" : \"2019-12-06T01:00:00\",\"wind_cdir_full\" : \"east-northeast\",\"wind_cdir\" : \"ENE\",\"slp\" : 1022.3,\"vis\" : 5,\"h_angle\" : -90,\"sunset\" : \"23:58\",\"dni\" : 0,\"dewpt\" : 3.7,\"snow\" : 0,\"uv\" : 0,\"precip\" : 0,\"wind_dir\" : 60,\"sunrise\" : \"15:22\",\"ghi\" : 0,\"dhi\" : 0,\"aqi\" : 14,\"lat\" : 47.66,\"weather\" : {\"icon\":\"c02n\",\"code\" : \"802\",\"description\" : \"Scattered clouds\"},\"datetime\" : \"2019-12-06:01\",\"temp\" : 5.9,\"station\" : \"2306P\",\"elev_angle\" : -10.04,\"app_temp\" : 4.9}] ,\"count\" : 1 }";

	// Get rid of [ and ] brackets that were causing the JSON library to not correctly parse the data.
	teststring.erase(std::remove(teststring.begin(), teststring.end(), '['));
	teststring.erase(std::remove(teststring.begin(), teststring.end(), ']'));

	// Insert the entire HTTP response message into a JSON object
	responseJSON = nlohmann::json::parse(teststring);

	// Grab all of the key value pairs inside the "data" section
	responseJSON = responseJSON.at("/data"_json_pointer);

	std::cout << responseJSON.dump() << std::endl;
	std::cout << responseJSON.size() << std::endl;

	// Get the value for "app_temp" and store it inside responseJSON
	responseJSON = responseJSON.at("/app_temp"_json_pointer);
	std::cout << responseJSON.dump();

	// Get the stored app_temp value as a string
	std::string temperature = responseJSON.dump();

	std::cout << temperature;

	auto& slack = slack::create("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx");
	slack.chat.channel_username_iconemoji("#project", "Weather Bot", ":rain:");
	slack.chat.postMessage("The current temperature is " + temperature + " degrees celcius.");


	

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

