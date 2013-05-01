C++ MPlayer Wrapper
==========
Wrapper to control mplayer with c++
####Compile it!
Just download mplayer_interface.cpp modifiy it or include it or compile it with
```bash
g++ test.cpp -I. -lboost_system -lboost_iostreams -lboost_regex -g -o mplayer_wrapper
```
###Use TESTAREA
```bash
./mplayer_wrapper
```
Following commands are available
* play \<file\>
* pause
* bit - get audio bitrate
* pos - get percent position of playing audio file
* time - get the current position in the file in seconds
* next - plays next song
* exit or quit - terminate programm

###License
GPL3 license (see the LICENSE file (folder mplayer_interface)).
Feel free to use it and even contribute bug fixes or enhancements if you want. Enjoy!
