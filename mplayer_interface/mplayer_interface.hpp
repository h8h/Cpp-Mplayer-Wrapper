#include <string>
#include <boost/process.hpp> // version 0.5
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/asio.hpp>
#include <boost/process/mitigate.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/regex.hpp>

namespace b = boost::process;
namespace bi = boost::process::initializers;
namespace bis = boost::iostreams;

class mplayer_interface
{
private:
	bis::stream<bis::file_descriptor_sink>* mplayer_cmd;
	boost::process::pipe_end* mplayer_result;
	boost::asio::io_service io_service;
public:
	mplayer_interface();
	~mplayer_interface();
	
	//commands
	void play(const std::string& song);
	void stop();
	int get_audio_bitrate();
	int get_percent_pos();
	float get_time_pos();
	void pause();
	
	//conversion
	std::string get_data_from_pipe();
	float get_mpf_float();
	int get_mpf_integer();
};
