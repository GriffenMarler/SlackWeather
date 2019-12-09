#pragma once 

#include <string>
#include <vector>

using std::string;
using std::vector;

 class MessageBlock {

	 
	 string description, iconLocation, currentTemp, dateTime, JSONtemplate;

	  vector<string> dailyTempForecast;
	 
	 

 public:
	 MessageBlock() {

	 }


	 void addTemperature(string temp);

	 string getJSONtemplate();
	 void setJSONtemplate(string Template);


	 string getDescription();
	 void setDescription(string des);

	 string getIconLocation();
	 void setIconLocation(string iconLoc);

	 string getCurrentTemp();
	 void setCurrentTemp(string curT);

	 string getDateTime();
	 void setDateTime(string dateT);

	 void updateContents(string desc, string iconLoc, string curTemp, string dateT);

};