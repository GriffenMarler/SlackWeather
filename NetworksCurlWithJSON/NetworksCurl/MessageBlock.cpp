#include "MessageBlock.h"



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

string MessageBlock::getDescription()
{
	return description;
}

void MessageBlock::setDescription(string des)
{
	description = des;
}

string MessageBlock::getIconLocation()
{
	return iconLocation;
}

void MessageBlock::setIconLocation(string iconLoc)
{
	iconLocation = iconLoc;
}

string MessageBlock::getCurrentTemp()
{
	return currentTemp;
}

void MessageBlock::setCurrentTemp(string curT)
{
	currentTemp = curT;
}

string MessageBlock::getDateTime()
{
	return dateTime;
}

void MessageBlock::setDateTime(string dateT)
{
	dateTime = dateT;
}

void MessageBlock::updateContents(string desc, string iconLoc, string curTemp, string dateT)
{
	description = desc;
	iconLocation = iconLoc;
	currentTemp = curTemp;
	dateTime = dateT;
}
