# SlackWeather

SlackWeather allows a user to get daily weather forecast updates and critical weather incident alerts for a specific latitude and longtitude through the slack messaging system. Our slack bot sends out a temperature forecast once per day, and monitors for critical weather such as snow, wind speed, and UV information to allow user's to receive convenient weather updates directly to their selected device via the slack application. 

SlackWeather is a C++ program that utilizes the cURL library (https://curl.haxx.se/libcurl/), slacking library for Slack API (https://github.com/coin-au-carre/slacking), Nlohmann Json parser (https://nlohmann.github.io/json/) and the Weatherbit IO API accessed through RapidAPI (https://rapidapi.com/weatherbit/api/weather). If you are confused at all about how any of these library works, please refer to our source code as well as the links above for examples and functionality. 

## Program Configurations
If you would like to implement this bot in your own Slack workspace, take a look at the following steps:
1. Create or login to an existing Slack workspace
2. Once logged in, navigate to the slack API application page (https://api.slack.com/apps?new_app=1)
3. Once you are at the API application page, create a new APP. In the development Slack Workspace area put your workspace you want your bot working in. 
4. Once this step has been completed, you will be taken to a "Building Apps for Slack" screen. When you get to this screen, under Add features and functionality select the "Bots" field.  
5. Once the bot field is selected, you will be given the option to add a bot user. Go ahead and add a bot user with any display name and username you would like. Ignore the "Always Show My Bot as Online" button". 
6. After the bot user is added, you will need to Install your newly created application to your workspace. Navigate app to the Install App menu under "settings" and Install your application. When prompted for permissions, please allow for all in order to ensure functionality of our application. 
7. After you have installed the application to your workspace, you will be taken to an "Installed App Settings" screen that holds OAuth tokens for your team. The Bot User Oauth Access Token is what you will be looking for to use in our program. Please copy this value and store it in a place where you can easily transfer it into your code. The bot user code is in the following format: xxxx-xxxxxxxxxxx-xxxxxxx-xxxxxxxxxx
8. Now, we will need to set up and get a RapidAPI key. To do this, navigate to https://rapidapi.com/ and sign up for a new account.
9. Once you are signed up to an account, navigate to the WeatherBit API at the following link: https://rapidapi.com/weatherbit/api/weather
10. Now, you will need to obtain a key for RapidAPI by subscribing to the WeatherBit API. Navigate to the pricing plan screen and sign up: https://rapidapi.com/weatherbit/api/weather/pricing
11. Navigate to the WeatherBit endpoint page: https://rapidapi.com/weatherbit/api/weather/endpoints
12. Obtain your X-RapidAPI-Key and store it in a convenient place. Also obtain your X-RapidAPI-Host string and store it in a convenient place.
13. Input your conveniently stored Bot User Ouath Access Token, X-RapidAPI-Key, and X-RapidAPI-Host string into the global variables at the top of "NetworksCurl.cpp". 

## Virtual Machine Set-Up
We suggest users to deploy our machine on an Azure Virtual Machine, so the bot can virtually run 24/7. For our final project we used an Azure virtual machine with a Visual Studio 2019 Community image preuploaded to it. We then pulled our project from github to allow us to run our program on the virtual machine. We chose an Azure device on a simple plan in order to keep costs down. We used RDP (Remote Desktop Protocol) to access our Azure virtual machine. More documentation on Azure virtual machines imaged with visual studio can be found at this link: https://docs.microsoft.com/en-us/visualstudio/install/using-visual-studio-vm?view=vs-2019

## Creators
Stephen Haugland and Griffen Marler
