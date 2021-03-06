/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at http://blog.squix.ch
*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "WundergroundClient.h"
bool usePM = false; // Set to true if you want to use AM/PM time disaply
bool isPM = false; // JJG added ///////////

WundergroundClient::WundergroundClient(boolean _isMetric) {
  isMetric = _isMetric;
}

void WundergroundClient::updateConditions(String apiKey, String language, String country, String city) {
  isForecast = false;
  doUpdate("/api/" + apiKey + "/conditions/lang:" + language + "/q/" + country + "/" + city + ".json");
}

// wunderground change the API URL scheme:
// http://api.wunderground.com/api/<API-KEY>/conditions/lang:de/q/zmw:00000.215.10348.json
void WundergroundClient::updateConditions(String apiKey, String language, String zmwCode) {
  isForecast = false;
  doUpdate("/api/" + apiKey + "/conditions/lang:" + language + "/q/zmw:" + zmwCode + ".json");
}

void WundergroundClient::updateForecast(String apiKey, String language, String country, String city) {
  isForecast = true;
  doUpdate("/api/" + apiKey + "/forecast10day/lang:" + language + "/q/" + country + "/" + city + ".json");
}

// JJG added ////////////////////////////////
void WundergroundClient::updateAstronomy(String apiKey, String language, String country, String city) {
  isForecast = true;
  doUpdate("/api/" + apiKey + "/astronomy/lang:" + language + "/q/" + country + "/" + city + ".json");
}
// end JJG add  ////////////////////////////////////////////////////////////////////

void WundergroundClient::doUpdate(String url) {
  JsonStreamingParser parser;
  parser.setListener(this);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("api.wunderground.com", httpPort)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: api.wunderground.com\r\n" +
               "Connection: close\r\n\r\n");
  int retryCounter = 0;
  while(!client.available()) {
    delay(1000);
    retryCounter++;
    if (retryCounter > 10) {
      return;
    }
  }

  int pos = 0;
  boolean isBody = false;
  char c;

  int size = 0;
  client.setNoDelay(false);
  while(client.connected()) {
    while((size = client.available()) > 0) {
      c = client.read();
      if (c == '{' || c == '[') {
        isBody = true;
      }
      if (isBody) {
        parser.parse(c);
      }
    }
  }
}

void WundergroundClient::whitespace(char c) {
  Serial.println("whitespace");
}

void WundergroundClient::startDocument() {
  Serial.println("start document");
}

void WundergroundClient::key(String key) {
  currentKey = String(key);
  if (currentKey == "txt_forecast") {
    isSimpleForecast = false;
  }
  if (currentKey == "simpleforecast") {
    isSimpleForecast = true;
  }
}

void WundergroundClient::value(String value) {
  if (currentKey == "local_epoch") {
    localEpoc = value.toInt();
    localMillisAtUpdate = millis();
  }

  // JJG added ... //////////////////////// search for keys /////////////////////////
  if (currentKey == "percentIlluminated") {
    moonPctIlum = value;
  }

  if (currentKey == "ageOfMoon") {
    moonAge = value;
  }

  if (currentKey == "phaseofMoon") {
    moonPhase = value;
  }


  if (currentParent == "sunrise") {      // Has a Parent key and 2 sub-keys
	if (currentKey == "hour") {
		int tempHour = value.toInt();    // do this to concert to 12 hour time (make it a function!)
		if (usePM && tempHour > 12){
			tempHour -= 12;
			isPM = true;
		}
		else isPM = false;
		sunriseTime = String(tempHour);
        //sunriseTime = value;
      }
	if (currentKey == "minute") {
    sunriseTime += ":" + value;
	if (isPM) sunriseTime += "pm";
	else if (usePM) sunriseTime += "am";
   }
  }


  if (currentParent == "sunset") {      // Has a Parent key and 2 sub-keys
	if (currentKey == "hour") {
		int tempHour = value.toInt();   // do this to concert to 12 hour time (make it a function!)
		if (usePM && tempHour > 12){
			tempHour -= 12;
			isPM = true;
		}
		else isPM = false;
		sunsetTime = String(tempHour);
       // sunsetTime = value;
      }
	if (currentKey == "minute") {
    sunsetTime += ":" + value;
	if (isPM) sunsetTime += "pm";
	else if(usePM) sunsetTime += "am";
   }
  }

  if (currentParent == "moonrise") {      // Has a Parent key and 2 sub-keys
	if (currentKey == "hour") {
		int tempHour = value.toInt();   // do this to concert to 12 hour time (make it a function!)
		if (usePM && tempHour > 12){
			tempHour -= 12;
			isPM = true;
		}
		else isPM = false;
		moonriseTime = String(tempHour);
       // moonriseTime = value;
      }
	if (currentKey == "minute") {
    moonriseTime += ":" + value;
	if (isPM) moonriseTime += "pm";
	else if (usePM) moonriseTime += "am";

   }
  }

  if (currentParent == "moonset") {      // Not used - has a Parent key and 2 sub-keys
	if (currentKey == "hour") {
        moonsetTime = value;
      }
	if (currentKey == "minute") {
    moonsetTime += ":" + value;
   }
  }

  if (currentKey == "wind_mph") {
    windSpeed = value;
  }

   if (currentKey == "wind_dir") {
    windDir = value;
  }

// end JJG add  ////////////////////////////////////////////////////////////////////

   if (currentKey == "observation_time_rfc822") {
    date = value.substring(0, 16);
  }
  if (currentKey == "temp_f" && !isMetric) {
    currentTemp = value;
  }
  if (currentKey == "temp_c" && isMetric) {
    currentTemp = value;
  }
  if (currentKey == "icon") {
    if (isForecast && !isSimpleForecast && currentForecastPeriod < MAX_FORECAST_PERIODS) {
      Serial.println(String(currentForecastPeriod) + ": " + value + ":" + currentParent);
      forecastIcon[currentForecastPeriod] = value;
    }
    if (!isForecast) {
      weatherIcon = value;
    }
  }
  if (currentKey == "weather") {
    weatherText = value;
  }
  if (currentKey == "relative_humidity") {
    humidity = value;
  }
  if (currentKey == "pressure_mb" && isMetric) {
    pressure = value + "mb";
  }
  if (currentKey == "pressure_in" && !isMetric) {
    pressure = value + "in";
  }
  if (currentKey == "dewpoint_f" && !isMetric) {
    dewPoint = value;
  }
  if (currentKey == "dewpoint_c" && isMetric) {
    dewPoint = value;
  }
  if (currentKey == "precip_today_metric" && isMetric) {
    precipitationToday = value + "mm";
  }
  if (currentKey == "precip_today_in" && !isMetric) {
    precipitationToday = value + "in";
  }
  if (currentKey == "period") {
    currentForecastPeriod = value.toInt();
  }
  if (currentKey == "title" && currentForecastPeriod < MAX_FORECAST_PERIODS) {
      Serial.println(String(currentForecastPeriod) + ": " + value);
      forecastTitle[currentForecastPeriod] = value;
  }
  // The detailed forecast period has only one forecast per day with low/high for both
  // night and day, starting at index 1.
  int dailyForecastPeriod = (currentForecastPeriod - 1) * 2;

  if (currentKey == "fahrenheit" && !isMetric && dailyForecastPeriod < MAX_FORECAST_PERIODS) {

      if (currentParent == "high") {
        forecastHighTemp[dailyForecastPeriod] = value;
      }
      if (currentParent == "low") {
        forecastLowTemp[dailyForecastPeriod] = value;
      }
  }
  if (currentKey == "celsius" && isMetric && dailyForecastPeriod < MAX_FORECAST_PERIODS) {

      if (currentParent == "high") {
        Serial.println(String(currentForecastPeriod)+ ": " + value);
        forecastHighTemp[dailyForecastPeriod] = value;
      }
      if (currentParent == "low") {
        forecastLowTemp[dailyForecastPeriod] = value;
      }
  }
}

void WundergroundClient::endArray() {

}


void WundergroundClient::startObject() {
  currentParent = currentKey;
}

void WundergroundClient::endObject() {
  currentParent = "";
}

void WundergroundClient::endDocument() {

}

void WundergroundClient::startArray() {

}


String WundergroundClient::getHours() {
    if (localEpoc == 0) {
      return "--";
    }
    int hours = (getCurrentEpoch()  % 86400L) / 3600 + gmtOffset;
    if (hours < 10) {
      return "0" + String(hours);
    }
    return String(hours); // print the hour (86400 equals secs per day)

}
String WundergroundClient::getMinutes() {
    if (localEpoc == 0) {
      return "--";
    }
    int minutes = ((getCurrentEpoch() % 3600) / 60);
    if (minutes < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      return "0" + String(minutes);
    }
    return String(minutes);
}
String WundergroundClient::getSeconds() {
    if (localEpoc == 0) {
      return "--";
    }
    int seconds = getCurrentEpoch() % 60;
    if ( seconds < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      return "0" + String(seconds);
    }
    return String(seconds);
}
String WundergroundClient::getDate() {
  return date;
}
long WundergroundClient::getCurrentEpoch() {
  return localEpoc + ((millis() - localMillisAtUpdate) / 1000);
}

// JJG added ... /////////////////////////////////////////////////////////////////////////////////////////
String WundergroundClient::getMoonPctIlum() {
  return moonPctIlum;
}

String WundergroundClient::getMoonAge() {
  return moonAge;
}

String WundergroundClient::getMoonPhase() {
  return moonPhase;
}

String WundergroundClient::getSunriseTime() {
  return sunriseTime;
 }

String WundergroundClient::getSunsetTime() {
  return sunsetTime;
 }

String WundergroundClient::getMoonriseTime() {
  return moonriseTime;
 }

String WundergroundClient::getMoonsetTime() {
  return moonsetTime;
 }

String WundergroundClient::getWindSpeed() {
  return windSpeed;
 }

String WundergroundClient::getWindDir() {
  return windDir;
 }

 // end JJG add ////////////////////////////////////////////////////////////////////////////////////////////


String WundergroundClient::getCurrentTemp() {
  return currentTemp;
}

String WundergroundClient::getWeatherText() {
  return weatherText;
}

String WundergroundClient::getHumidity() {
  return humidity;
}

String WundergroundClient::getPressure() {
  return pressure;
}

String WundergroundClient::getDewPoint() {
  return dewPoint;
}

String WundergroundClient::getPrecipitationToday() {
  return precipitationToday;
}

String WundergroundClient::getTodayIcon() {
  return getMeteoconIcon(weatherIcon);
}

String WundergroundClient::getTodayIconText() {
  return weatherIcon;
}

String WundergroundClient::getForecastIcon(int period) {
  return getMeteoconIcon(forecastIcon[period]);
}

String WundergroundClient::getForecastTitle(int period) {
  return forecastTitle[period];
}

String WundergroundClient::getForecastLowTemp(int period) {
  return forecastLowTemp[period];
}

String WundergroundClient::getForecastHighTemp(int period) {
  return forecastHighTemp[period];
}

String WundergroundClient::getMeteoconIcon(String iconText) {
  if (iconText == "chanceflurries") return "F";
  if (iconText == "chancerain") return "Q";
  if (iconText == "chancesleet") return "W";
  if (iconText == "chancesnow") return "V";
  if (iconText == "chancetstorms") return "S";
  if (iconText == "clear") return "B";
  if (iconText == "cloudy") return "Y";
  if (iconText == "flurries") return "F";
  if (iconText == "fog") return "M";
  if (iconText == "hazy") return "E";
  if (iconText == "mostlycloudy") return "Y";
  if (iconText == "mostlysunny") return "H";
  if (iconText == "partlycloudy") return "H";
  if (iconText == "partlysunny") return "J";
  if (iconText == "sleet") return "W";
  if (iconText == "rain") return "R";
  if (iconText == "snow") return "W";
  if (iconText == "sunny") return "B";
  if (iconText == "tstorms") return "0";

  if (iconText == "nt_chanceflurries") return "F";
  if (iconText == "nt_chancerain") return "7";
  if (iconText == "nt_chancesleet") return "#";
  if (iconText == "nt_chancesnow") return "#";
  if (iconText == "nt_chancetstorms") return "&";
  if (iconText == "nt_clear") return "2";
  if (iconText == "nt_cloudy") return "Y";
  if (iconText == "nt_flurries") return "9";
  if (iconText == "nt_fog") return "M";
  if (iconText == "nt_hazy") return "E";
  if (iconText == "nt_mostlycloudy") return "5";
  if (iconText == "nt_mostlysunny") return "3";
  if (iconText == "nt_partlycloudy") return "4";
  if (iconText == "nt_partlysunny") return "4";
  if (iconText == "nt_sleet") return "9";
  if (iconText == "nt_rain") return "7";
  if (iconText == "nt_snow") return "#";
  if (iconText == "nt_sunny") return "4";
  if (iconText == "nt_tstorms") return "&";

  return ")";
}
