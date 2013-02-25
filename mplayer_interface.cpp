#include <boost/process.hpp> // version 0.5
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <string>
#include <boost/asio.hpp>
#include <boost/process/mitigate.hpp>
#include <boost/asio/socket_base.hpp>
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
    mplayer_interface() {
      b::pipe p1 = b::create_pipe();
      b::pipe p2 = b::create_pipe(); //asio pipe only for posix, sry windows :)

      {
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
        }
      //STDIN
      bis::file_descriptor_sink sink1(p1.sink, bis::close_handle);
      mplayer_cmd = new bis::stream<bis::file_descriptor_sink>(sink1);

      //STDOUT - Asynchronous
      mplayer_result = new boost::process::pipe_end(io_service, p2.source);
    }

    void play(const std::string& song) {
      *mplayer_cmd << "loadfile " << song << std::endl;
    }

    void get_audio_bitrate() {
      *mplayer_cmd << "get_audio_bitrate" << std::endl;
       get_mpf_float();
    }

    void get_percent_pos() {
      *mplayer_cmd << "get_percent_pos" << std::endl;
      get_mpf_float();
    }

    void get_mpf_float() {
      //avoid blocking, but manipulate the stdout !bug!
      boost::asio::ip::tcp::socket socket(io_service);
      boost::asio::socket_base::bytes_readable command(true);
      mplayer_result->io_control(command);
      std::size_t bytes_readable = command.get();
      if(bytes_readable == 0) { return; }
      //read data from buffer
      boost::system::error_code error;
      boost::asio::streambuf buffer;
      boost::asio::read_until( *mplayer_result,buffer,"\n", error);
      io_service.run();
      std::istream str(&buffer);
      std::string s;
      std::getline(str, s);
      //todo: make str to float
      std::cout << "read: "<< s << std::endl;
    }

    void pause() {
      *mplayer_cmd << "pause" << std::endl;
    }
};


//TESTAREA
int main( int argc, char *argv[] )
{
    mplayer_interface mplayer;
    std::string userI;
    while ( strcmp( userI.c_str(), "exit") != 0 )
    {
        std::cout<<'>';
        std::cin>>userI;
        if(userI.compare("play") == 0) {
          std::string s;
          std::cin>>s;
          mplayer.play(s);
        }
        if(userI.compare("bit") == 0) {
          mplayer.get_audio_bitrate();
        }
        if(userI.compare("pos") == 0) {
          mplayer.get_percent_pos();
        }

    }
    return(0);
}
