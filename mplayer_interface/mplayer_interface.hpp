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
protected:
	static mplayer_interface instance;

	bis::stream<bis::file_descriptor_sink>* mplayer_cmd;
	boost::process::pipe_end* mplayer_result;
	boost::asio::io_service io_service;

	mplayer_interface();
	~mplayer_interface();

	// internal functions
	std::string read_from_pipe();
	float pipe_convert_to_float();
	int pipe_convert_to_int();

public:
	// external interface
	static void play(const std::string& song);
  static void quit();
	static void stop();
	static void pause();
  static void next();
	static int get_audio_bitrate();
	static int get_percent_pos();
	static float get_time_pos();
};
