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

	//initializes a curl setup
	CURL* hnd = curl_easy_init();

	//sets the options of the curl object to send a get request with latitude and longitude paramaters from Spokane
	curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(hnd, CURLOPT_URL, "https://air-quality.p.rapidapi.com/current/airquality?lon=-117.426048&lat=47.658779");
	curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, &WriteCallback);
	curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

	//curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L); //tell curl to output its progress


	// adds to the header of the curl item which api, and api-key is being used
	struct curl_slist* headers = NULL;
	headers = curl_slist_append(headers, "x-rapidapi-host: air-quality.p.rapidapi.com");
	headers = curl_slist_append(headers, "x-rapidapi-key: 5890f2bc73msh6cf206fed9e48e2p1962c4jsn255815d320c7");
	curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);


	CURLcode ret = curl_easy_perform(hnd);

	curl_easy_cleanup(hnd);

	std::cout << std::endl << readBuffer << std::endl;

	
	responseJSON = nlohmann::json::parse(readBuffer);

	std::string response = responseJSON.dump();
	std::cout << response << std::endl;

	//auto& slack = slack::create("xoxp-862360242919-862360250039-862952376182-2fac0887b0995cde228c44434e4faf52");
	//slack.chat.channel = "#project";
	//slack.chat.postMessage("Test from Stephen's Computer");




	system("pause");
	return 0;

}