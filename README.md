# atp-artnet-esp8266
<h1>All Tech Plus - Artnet NodeMCU Controller</h1>
<h2>Description</h2>
A sketch to allow a node MCU 4M with 1M SPIFFS to process Artnet/DMX packets from xLights Scheduler plus a few bonuses

Last year (2018) after following a few of Dr ZZZs YouTube videos I decided that I wanted to tinker with RGB Pixels.
So I bought my first set of pixels which just happened to be WS2801 - the kind that also needed to use a data pin as
well as a clock pin. I had all sorts of issues getting it to work, most likely because my knowledge of writing sketches
was not that great (still not that great by the way!), in the end I found a version of McLighting that worked and so made
do with that for Christmas of that year.

This year, well, I wanted to get some more pixels (bought one more set of WS2801, and 2 sets of WS2811) and use xLights to
control them. I found a library which is included in this project (all sources at foot of this read me) which could listen for
Artnet/DMX packets, basically the R,G and B values for each node, and managed to cobble something together which, well,
worked. I wasn't happy enough with that though, as I won't be running an xLight show all the time, I'd like to be able to
select inbuilt effects just like I did with McLighting - so I've lifted a few effects from different sources to use as built
in effects.

I've added a web interface. The ability to change some of the settings, and will soon be adding in MQTT control as well.

Now, this may not be coded super effeciently, I may have been wasteful with variable storage, there will always be a better
way of doing things, so that's where you come in - feel free to contribute additions and improvements.

<h2>Installation Instructions</h2>

Take a copy of the ZIP file from this github and extract it into your Arduino sketch folder. You'll have the folder with
the various .ino files in it as well as a data folder.

You may optionally need to install the tool for uploading Sketch data - find instructions here:
    https://github.com/esp8266/arduino-esp8266fs-plugin

Open the main sketch (currently called XXXXX). If you've installed the above tool ok, from the tools menu you'll have
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
    <li>McLighting, and some from tweaking4all.com
        <ul>
<li>https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/
<li>https://www.tweaking4all.com/forums/topic/led-matrix-with-ws2812b-led-stripes-project/
        </ul>
</ul>


<h2>Current Issues</h2>

Whenever I set the effects on startup to off - the ESP8266 dumps its stack - not sure exactly why. I thought it was to do
with incomplete web requests so I added in a few delays before restarting the sketch after a save. If anyone has a fix?

this document is unfinished!
