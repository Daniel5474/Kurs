#include <netinet/in.h>
#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
#include <cryptopp/cryptlib.h>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <getopt.h>
#include <csignal>
#include <cstdlib>
#include <cryptopp/hex.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
using namespace CryptoPP;
using namespace std;
#include <cryptopp/md5.h> 

class Error
{
public:
    Error();
    static void errors(std::string error, std::string name);
    static int er(std::string file_name, std::string file_error);
};

class Server
{
private:
    Error e_error;
public:
    Server(Error err) : e_error(err) {
        e_error = err;
    }
    int self_addr(std::string& error, std::string& file_error, int port);
    int client_addr(int s, std::string& error, std::string& file_error);
    //int math(int work_sock);
};

class Authorized
{
private:
    Error e_error;
    std::string salt_generator(const std::size_t length);
    void msgsend(int work_sock, const std::string& mess);
    std::string MD(const std::string& sah);
public:
    Authorized(Error err) : e_error(err) {
        e_error = err;
    }
	int authorized(int work_sock, string file_name, string file_error);
};

class Calculator
{
private:
    Error e_error;
public:
    Calculator(Error err) : e_error(err) {
        e_error = err;
    }
	int calc(int work_sock);
};

