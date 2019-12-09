#pragma once 

#include <string>
#include <vector>

using std::string;
using std::vector;

 class MessageBlock {

	 
	 string date, JSONtemplate, cityName;

	  vector<string> dailyTempForecast;
	 
 public:
	 MessageBlock() {

	 }

	 void setCityName(string city);
	 string getCityName();


	 void addTemperature(string temp);

	 string getJSONtemplate();
	 void setJSONtemplate(string Template);


	 string getDescription();
	 void setDescription(string des);

	 string getIconLocation();
	 void setIconLocation(string iconLoc);

	 string getCurrentTemp();
	 void setCurrentTemp(string curT);

	 string getDate();
	 void setDate(string dateT);

	 void fillTemplate();

	/* void updateCurrentConditions(string snow, string uv, string wind_spd);*/

};