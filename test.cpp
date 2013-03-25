#include <mplayer_interface/mplayer_interface.cpp>
#include <string>
//TESTAREA
int main( int argc, char *argv[] )
{
    std::string userI;
    while(strcmp(userI.c_str(), "exit") != 0 && strcmp(userI.c_str(),"quit") != 0)
    {
        std::cout << '>';
        std::cin  >> userI;
        if(userI.compare("play") == 0) {
          std::string s;
          std::cin >> s;
          mplayer_interface::play(s);
        }
        if(userI.compare("bit") == 0) {
          std::cout << "Bitrate: " << mplayer_interface::get_audio_bitrate() << " kbs" << std::endl;
        }
        if(userI.compare("pos") == 0) {
          std::cout << "Percent: " << mplayer_interface::get_percent_pos() << "%" << std::endl;
        }
        if(userI.compare("time") == 0) {
          std::cout << "Current Timeposition: " << mplayer_interface::get_time_pos() << "sec" << std::endl;
        }
        if(userI.compare("next") == 0) {
          mplayer_interface::next();
          std::cout << "Next song" << std::endl;
        }
        if(userI.compare("pause") == 0) {
          mplayer_interface::pause();
          std::cout << "pause" << std::endl;
        }
    }

    mplayer_interface::quit();
    return 0;
}
