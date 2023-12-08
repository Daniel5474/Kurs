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
#include <cryptopp/cryptlib.h>
#include <iostream>
#include <vector>
#include <getopt.h>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sys/select.h> //Добавочный 
#include <cryptopp/hex.h> // HexEncoder
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

using namespace CryptoPP;
using namespace std;

#include <cryptopp/md5.h>

std::string MD(std::string sah);
void errors(std::string error, std::string name);
int er(std::string file_name, std::string file_error);
void msgsend(int work_sock, string mess);
int authorized(int work_sock, string file_name, string file_error);
int math(int work_sock);

class Server
{
private:

public:
    int self_addr(string error, string file_error, int port);
    int client_addr(int s, string error, string file_error);
};



