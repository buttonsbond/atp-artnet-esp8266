# atp-artnet-esp8266
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
select inbuilt effects just like I did with McLighting - so I've lifted a few effects from different sources including Dr ZZZs
sketch, and elsewhere.

I've added a web interface. The ability to change some of the settings, and will soon be adding in MQTT control as well.

Now, this may not be coded super effeciently, I may have been wasteful with variable storage, there will always be a better way
of doing things, so that's where you come in - feel free to contribute additions and improvements.

Sources:

ESPAsyncWebserver

this document is unfinished!
