#include "mplayer_interface.hpp"

mplayer_interface mplayer_interface::instance;

mplayer_interface::mplayer_interface()
{
	b::pipe p1 = b::create_pipe();
	b::pipe p2 = b::create_pipe(); //asio pipe only for posix, sry windows :)

	bis::file_descriptor_source source1(p1.source, bis::close_handle);
	bis::file_descriptor_sink sink2(p2.sink, bis::close_handle);
	std::vector<std::string> args;
	args.push_back("/usr/bin/mplayer");
	args.push_back("-slave");
	args.push_back("-idle");
	args.push_back("-nolirc");
	args.push_back("-really-quiet");
	args.push_back("-msglevel");
	args.push_back("global=4");
	args.push_back("-input");
	args.push_back("nodefault-bindings");
	args.push_back("-noconfig");
	args.push_back("all");

	b::execute(
		bi::set_args(args),
		bi::bind_stdin(source1),
		bi::bind_stdout(sink2),
		bi::inherit_env(),
		bi::close_fd(p1.sink)
	);

	//STDIN
	bis::file_descriptor_sink sink1(p1.sink, bis::close_handle);
	mplayer_cmd = new bis::stream<bis::file_descriptor_sink>(sink1);

	//STDOUT - Asynchronous
	mplayer_result = new boost::process::pipe_end(io_service, p2.source);
}

mplayer_interface::~mplayer_interface()
{
	delete mplayer_cmd;
	delete mplayer_result;
}

std::string mplayer_interface::read_from_pipe()
{
	//avoid blocking
	boost::asio::deadline_timer t(io_service, boost::posix_time::milliseconds(100));
	t.wait(); //wait till mplayer answered
	boost::asio::posix::stream_descriptor::bytes_readable command(true);
	mplayer_result->io_control(command);
	std::size_t bytes_readable = command.get();
	bytes_readable = command.get();
	if(bytes_readable == 0) { return ""; }

	//read data from buffer
	boost::system::error_code error;
	boost::asio::streambuf buffer;
	boost::asio::read_until( *mplayer_result,buffer,"\n", error);
	io_service.run();

	//turn istream into string
	std::string s;
	std::istream str(&buffer);
	std::getline(str, s);
	return s;
}

float mplayer_interface::pipe_convert_to_float()
{
	std::string s = read_from_pipe();

	//search for digits 123.456
	boost::regex regex("\\d+\\.\\d+");
	boost::smatch result;
	if(!boost::regex_search(s, result, regex)) { return -1; }
	try
	{
		return boost::lexical_cast<float>(result[0]);
	}
	catch(boost::bad_lexical_cast &)
	{
		return -1;
	}
}

int mplayer_interface::pipe_convert_to_int()
{
	std::string s = read_from_pipe();

	//search for digits 123456
	boost::regex regex("\\d+");
	boost::smatch result;
	if(!boost::regex_search(s, result, regex)) { return -1; }
	try
	{
		return boost::lexical_cast<int>(result[0]);
	}
	catch(boost::bad_lexical_cast &)
	{
		return -1;
	}
}

void mplayer_interface::play(const std::string& song)
{
	*instance.mplayer_cmd << "loadfile " << song << " 1" << std::endl;
}

void mplayer_interface::stop()
{
	*instance.mplayer_cmd << "stop" << std::endl;
}

void mplayer_interface::quit()
{
  *instance.mplayer_cmd << "quit" << std::endl;
}
void mplayer_interface::pause()
{
	*instance.mplayer_cmd << "pause" << std::endl;
}

void mplayer_interface::next()
{
  *instance.mplayer_cmd << "pausing_keep_force pt_step 1" << std::endl;
  *instance.mplayer_cmd << "get_property pause" << std::endl;
}
int mplayer_interface::get_audio_bitrate()
{
	*instance.mplayer_cmd << "get_audio_bitrate" << std::endl;
	return instance.pipe_convert_to_int();
}

int mplayer_interface::get_percent_pos()
{
	*instance.mplayer_cmd << "get_percent_pos" << std::endl;
	return instance.pipe_convert_to_int();
}

float mplayer_interface::get_time_pos()
{
	*instance.mplayer_cmd << "get_time_pos" << std::endl;
	return instance.pipe_convert_to_float();
}
