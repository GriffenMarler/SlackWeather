# SlackWeather

SlackWeather is a C++ program that utilizes the cURL library (https://curl.haxx.se/libcurl/), slacking library for Slack API (https://github.com/coin-au-carre/slacking), Nlohmann Json parser (https://nlohmann.github.io/json/) and the Weatherbit IO API accessed through RapidAPI (https://rapidapi.com/weatherbit/api/weather).

SlackWeather allows a user to get daily weather forecast updates and critical weather incident alerts for a specific latitude and longtitude through the slack messaging system. Our slack bot sends out a a 24 hour temperature forecast once per day, and monitors for critical weather such as snow, wind speed, and UV information to allow user's to receive conveninet weather updates directly to their selected device via the slack application. 

## Virtual Machine Set-Up
We suggest users to deploy our machine on an Azure Virtual Machine, so the bot can virtually run 24/7. For our final project we used an Azure virtual machine with a Visual Studio 2019 Community image preuploaded to it. We then pulled our project from github to allow us to run our program on the virtual machine. We chose an Azure device on a simple plan in order to keep costs down. We used RDP (Remote Desktop Protocol) to access our Azure virtual machine. More documentation on Azure virtual machines imaged with visual studio can be found at this link: https://docs.microsoft.com/en-us/visualstudio/install/using-visual-studio-vm?view=vs-2019

## Task Scheduling
We also suggest users schedule their task to auto run using the Windows Task Scheduler.
```python
import foobar

foobar.pluralize('word') # returns 'words'
foobar.pluralize('goose') # returns 'geese'
foobar.singularize('phenomena') # returns 'phenomenon'
```

## Creators
Stephen Haugland and Griffen Marler
