#include "MessageBlock.h"



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

//void MessageBlock::updateCurrentConditions(string desc, string iconLoc, string curTemp, string dateT)
//{
//	description = desc;
//	iconLocation = iconLoc;
//	currentTemp = curTemp;
//	dateTime = dateT;
//}
