# atp-artnet-esp8266
<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.3.1/css/bootstrap.min.css" integrity="sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/iJTQUOhcWr7x9JvoRxT2MZw1T" crossorigin="anonymous">
<h1>All Tech Plus - Artnet NodeMCU Controller</h1>
<p class="alert alert-warning">Please note you may have to hard code your ssid and pw for your WiFi for the time being. Though if the ESP8266 hasn't been connected before you should get WiFi manager the first time. I'll have this fixed soon - or rather programmed!</p>
<h2>Description</h2>
A sketch to allow a node MCU 4M with 1M SPIFFS to process Artnet/DMX packets from xLights Scheduler plus a few bonuses

Last year (2018) after following a few of Dr ZZZs YouTube videos I decided that I wanted to tinker with RGB Pixels.
So I bought my first set of pixels which just happened to be WS2801 - the kind that also needed to use a data pin as
well as a clock pin. I had all sorts of issues getting it to work, most likely because my knowledge of writing sketches
was not that great (still not that great by the way!), in the end I found a version of McLighting that worked and so made
do with that for Christmas of that year.

This year, well, I wanted to get some more pixels (bought one more set of WS2801 to make up 100 string, and 2 sets of WS2811 another 100 string) and use xLights to
control them. I found a library which is included in this project (all sources at foot of this read me) which could listen for
Artnet/DMX packets, basically the R,G and B values for each node, and managed to cobble something together which, well,
worked. I wasn't happy enough with that though, as I won't be running an xLight show all the time, I'd like to be able to
select inbuilt effects just like I did with McLighting - so I've lifted a few effects from different sources to use as built
in effects.

<h2>User interfaces</h2>
<h3>It has a web interface!</h3>
Just got to the IP address shown in either the serial console, or http://nodename.local if your machine is running Bonjour.
The interface itself isn't terribly refined at the moment - it needs finishing and improving.
<h3>MQTT</h3>
More or less working MQTT, the topics are hostname/effect, hostname/startup, hostname/cycle and hostname/off.
<ul>
    <li>hostname/effect pass a number as payload corresponding to the effect number.</li>
    <li>hostname/startup pass either 1 to set on, or 0 to set off.</li>
    <li>hostname/cycle pass either 1 or 0.</li>
    <li>hostname/off pass anything you like will turn off startup, and turn the lights off [not currently working]</li>
</ul>
<h3>Wemo</h3>
You can now control with your Amazon Alexa. The device is known by its hostname. Commands are:
<ul>
    <li>Alexa, turn on hostname</li>
    <li>Alexa, turn off hostname</li>
    <li>Alexa, set hostname to X percent</li>
    <li>Alexa, set hostname to 100 percent</li>
    <li>Alexa, set hostname to 99 percent</li>
</ul>
Note that percentage values 1 to 15 are converted to the effect number.
100 percent will turn on the setting for cycling the built in effects, whilst 99 percent will set that to off.

Now, this may not be coded super effeciently, I may have been wasteful with variable storage, there will always be a better
way of doing things, so that's where you come in - feel free to contribute additions and improvements.

<h2>Installation Instructions</h2>

Take a copy of the ZIP file from this github and extract it into your Arduino sketch folder. You'll have the folder with
the various .ino files in it as well as a data folder.

You may optionally need to install the tool for uploading Sketch data - find instructions here:
    https://github.com/esp8266/arduino-esp8266fs-plugin

Open the main sketch (currently called xmas-artnet-ws2811.ino). If you've installed the above tool ok, from the tools menu you'll have
the option to upload the sketch data - this is the web interface and config file.

You may need to make a few changes in the settings.h file, there are comments in there to help you. Most likely you will
only need to change DATA_PIN and/or CLOCK_PIN - I couldn't find a way of changing these on the fly - maybe someone else will
know how?

Compile and upload your sketch in the usual way.

<h3>data folder</h3>
The data folder contains the html, css and js files which make up the web interface. You will also find there a config
file in json format. It is not necessary to edit for your own needs but you can if you wish - the web interface will
update the config when you make changes in any event.

You will need to initially upload the sketch data folder with the Sketch Data Upload tool - don't forget to allocate
some SPIFF storage on your ESP8266.

<h2>Sources</h2>

<ul>
    <li>ESPAsyncWebserver - https://github.com/me-no-dev/ESPAsyncWebServer I don't recall the reason I opted for using this particular library, most likely the result of various searches for webserver examples. Anyway, seems fairly easy to use. - You'll need to download this and place in your libraries folder.
<li>ESPAsyncTCP - https://github.com/me-no-dev/ESPAsyncTCP the webserver above also needed this. You'll need to download this.
<li>FastLED - https://github.com/FastLED/FastLED - I think you can add the library via. the libraries manager.
<li>Artnet Sender/Receiver - https://github.com/hideakitai/ArtNet - I think you can add the library via. the libraries manager.
</ul>

<h3>Built in effects</h3>

<ul>
    <li>McLighting
    <li>tweaking4all.com
        <ul>
<li>https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
        </ul>
</ul>

<h2>Works on...</h2>
<ul>
    <li>Node MCU d1 Mini 4Mb (ESP8266 MOD) - that's what I've got. I'll add more if people tell me!
</ul>
<h2>Current Issues</h2>

Sometimes when settings are changed - the ESP8266 dumps its stack - not sure exactly why. I thought it was to do
with incomplete web requests so I added in a few delays before restarting the sketch after a save. If anyone has a fix?

<h2>To be done - in no particular order</h2>
<ul>
    <li>Add MQTT support so can be used with Home Assistant and Node Red [PARTIALLY WORKING]
    <li>Ability to change wifi [TO BE COMPLETED]
    <li>Fix bugs
    <li>Tidy up code a bit
    <li>Maybe add some more comments
    <li>More built in effects
    <li>Add ability to save a Artnet/DMX sequence and load to play - not sure if this will be feasible depending on what I can get the file size down to
    <li>Finish the web interface - a few rough edges still!
</ul>
