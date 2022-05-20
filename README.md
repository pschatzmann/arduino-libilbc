# iLBC

Internet Low Bitrate Codec (iLBC) is a royalty-free narrowband speech audio coding format and an open-source reference implementation (codec), developed by Global IP Solutions (GIPS) formerly Global IP Sound (acquired by Google Inc in 2011.

iLBC handles lost frames through graceful speech quality degradation with a sampling frequency of 8 kHz/16 bit (160 samples for 20 ms frames, 240 samples for 30 ms frames).
The bitrate is fixed at 15.2 kbit/s for 20 ms frames, 13.33 kbit/s for 30 ms frames as is the frame size (304 bits per block for 20 ms frames, 400 bits per block for 30 ms frames).

### Installation in Arduino

You can download the library as zip and call include Library -> zip library. Or you can git clone this project into the Arduino libraries folder e.g. with

```
cd  ~/Documents/Arduino/libraries
git clone pschatzmann/arduino-ilbc.git
```

I recommend to use git because you can easily update to the latest version just by executing the ```git pull``` command in the project folder.