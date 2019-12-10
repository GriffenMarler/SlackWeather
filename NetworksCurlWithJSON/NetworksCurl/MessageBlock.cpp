#include "MessageBlock.h"



void MessageBlock::setSnow(string snow)
{
	Snow = snow;
}

string MessageBlock::getSnow()
{
	return Snow;
}

void MessageBlock::setUV(string uv)
{
	UV = uv;
}

string MessageBlock::getUV()
{
	return UV;
}

void MessageBlock::setWind_Spd(string wind_spd)
{
	Wind_Spd = wind_spd;
}

string MessageBlock::getWind_spd()
{
	return Wind_Spd;
}

void MessageBlock::setCityName(string city)
{
	cityName = city;
}

string MessageBlock::getCityName()
{
	return cityName;
}

void MessageBlock::setDate(string Date)
{
	date = Date;
}

string MessageBlock::getDate()
{
	return date;
}

void MessageBlock::addTemperature(string temp)
{
	dailyTempForecast.push_back(temp);
}

string MessageBlock::getJSONtemplate()
{
	return JSONtemplate;
}

void MessageBlock::setJSONtemplate(string Template)
{
	JSONtemplate = Template;
}
//
//string MessageBlock::getDescription()
//{
//	return description;
//}
//
//void MessageBlock::setDescription(string des)
//{
//	description = des;
//}
//
//string MessageBlock::getIconLocation()
//{
//	return iconLocation;
//}
//
//void MessageBlock::setIconLocation(string iconLoc)
//{
//	iconLocation = iconLoc;
//}
//
//string MessageBlock::getCurrentTemp()
//{
//	return currentTemp;
//}
//
//void MessageBlock::setCurrentTemp(string curT)
//{
//	currentTemp = curT;
//}
//
//string MessageBlock::getDateTime()
//{
//	return dateTime;
//}
//
//void MessageBlock::setDateTime(string dateT)
//{
//	dateTime = dateT;
//}


// populates the block template with the temperature values throughout the day
void MessageBlock::fillTemplate()
{
	string::size_type position = JSONtemplate.find("u2109");

	for (int i = 0; i < dailyTempForecast.size(); i++) {
		JSONtemplate.insert(position + 7, "  " + dailyTempForecast.at(i) + "\\t");
		position += (dailyTempForecast.at(i).size() + 4);
	}


	position = JSONtemplate.find("for");

	JSONtemplate.insert(position + 3, " " + cityName);

	position = JSONtemplate.find("on");

	JSONtemplate.insert(position + 2, " " + date);

}

void MessageBlock::fillConditionTemplate()
{
	// Find the position to insert the city name
	string::size_type City_Position = JSONtemplate.find("area");

	JSONtemplate.insert(City_Position - 1, " " + cityName);

	// Find position where we need to insert critical weather text
	string::size_type position = JSONtemplate.find("\"text\"");

	// Store the initial size of the template in case more than one critical weather condition is present at the same time
	int sizehold = JSONtemplate.size();

	// Add 8 to get in correct position for JSON format
	position = position + 8;


	if (std::stoi(Snow) > 5) {
	JSONtemplate.insert(position, "Snow reported falling at (mm/h): " + Snow +"\\n");
	}

	if (std::stoi(Wind_Spd) > 0) {
		// Adjust the size if the JSON template changes. Result of below equation will be 0 if snow is not present. 
		int sizeadjust = JSONtemplate.size() - sizehold;
		JSONtemplate.insert(position + sizeadjust, "Damaging winds reported at: " + Wind_Spd + "\\n");
	}

	if (std::stoi(UV) == 0) {
		// Adjust the size if the JSON template changes. Result of below equation will be 0 if snow or high wind speed is not present. 
		int sizeadjust = JSONtemplate.size() - sizehold;
		JSONtemplate.insert(position + sizeadjust, "Notable UV Level reported: " + UV + " Take Precaution when outside." + "\\n");
	}
}

//void MessageBlock::updateCurrentConditions(string desc, string iconLoc, string curTemp, string dateT)
//{
//	description = desc;
//	iconLocation = iconLoc;
//	currentTemp = curTemp;
//	dateTime = dateT;
//}
