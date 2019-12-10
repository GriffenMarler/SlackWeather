#pragma once 

#include <string>
#include <vector>

using std::string;
using std::vector;


// This class is used to contain all the relevant attributes that are used to create a message block to send to a slack channel
// It contains several weather attributes as well as the JSON template for message block for easy merging of the two before sending
 class MessageBlock {

	 
	 string date, JSONtemplate, cityName;

	 string Snow, UV, Wind_Spd;

	  vector<string> dailyTempForecast;
	 
 public:
	 MessageBlock() {

	 }

	 MessageBlock(string snow, string uv, string wind_spd, string city) {
		 Snow = snow;
		 UV = uv;
		 Wind_Spd = wind_spd;
		 cityName = city;
	 }


	 void setSnow(string snow);
	 string getSnow();

	 void setUV(string uv);
	 string getUV();

	 void setWind_Spd(string wind_spd);
	 string getWind_spd();


	 void setCityName(string city);
	 string getCityName();


	 void addTemperature(string temp);

	 string getJSONtemplate();
	 void setJSONtemplate(string Template);

	 string getDate();
	 void setDate(string dateT);

	 void fillTemplate();

	 void fillConditionTemplate();

	/* void updateCurrentConditions(string snow, string uv, string wind_spd);*/

};