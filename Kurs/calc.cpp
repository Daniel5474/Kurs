#include "calc.h"

void Authorized::msgsend(int work_sock, const std::string& mess){
    char *buffer = new char[4096];
    std::strcpy(buffer, mess.c_str());
    send(work_sock, buffer, mess.length(), 0);
}

std::string Authorized::MD(const std::string& sah){
Weak::MD5 hash;
    std::string digest;
    StringSource(sah, true,  new HashFilter(hash, new HexEncoder(new StringSink(digest))));  // строка-приемник
      return digest;
 }
 
std::string Authorized::salt_generator(const std::size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    std::string salt;
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::uniform_int_distribution<int> dist(0, sizeof(charset) - 2);

    for (std::size_t i = 0; i < length; i++) {
        salt += charset[dist(rng)];
    }

    return salt;
}

Error::Error()
{

}
 
void Error::errors(std::string error, std::string name){
    std::ofstream file;
    file.open(name, std::ios::app);
    if(file.is_open()){
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        file<<error<<':'<<asctime(timeinfo)<<std::endl;
        std::cout << "error: " << error << std::endl;
    }
}
int Error::er(std::string file_name, std::string file_error){
        std::fstream file;
        file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        try{
        file.open(file_name);
        return 1;
        }catch(const std::exception & ex){
        std::string error = "Ошибка открытия файла";
        errors(error, file_error);
        return 12;
        }
        }

void alarm_handler(int signal) {
    std::cout << "Время ожидания истекло\n";
    exit(EXIT_FAILURE);
}

int Server::self_addr(std::string& error, std::string& file_error, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }
    
    int on = 1;
    int rc = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (rc == -1) {
        throw std::system_error(errno, std::generic_category(), "Ошибка установки сокета");
    }
    
    signal(SIGALRM, alarm_handler);
    alarm(10);
    
    struct timeval timeout {10, 0};
    rc = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    if (rc == -1) {
        throw std::system_error(errno, std::generic_category(), "Ошибка установки сокета");
    }
    
    sockaddr_in self_addr;
    self_addr.sin_family = AF_INET;
    self_addr.sin_port = htons(port);
    self_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    std::cout << "Ожидание подключения клиента...\n";
    
    int b = bind(sock, reinterpret_cast<sockaddr*>(&self_addr), sizeof(self_addr));
    if (b == -1) {
        std::cout << "Ошибка привязки\n";
        error = "Ошибка";
        e_error.errors(error, file_error);
        return 1;
    }
    
    listen(sock, SOMAXCONN);
    
    return sock;
}

int Server::client_addr(int s, std::string& error, std::string& file_error) {
        // код функции client_addr
        sockaddr_in * client_addr = new sockaddr_in;
        socklen_t len = sizeof (sockaddr_in);
        
        int work_sock = accept(s, (sockaddr*)(client_addr), &len);
        if(work_sock == -1) {
            std::cout << "Ошибка\n";
            error = "Ошибка";
            e_error.errors(error, file_error);
            return 1;
        }
        else {
            //Успешное подключение к серверу
            std::cout << "Клиент успешно подключился!\n";
            return work_sock;
        }
    }

int Authorized::authorized(int work_sock, string file_name, string file_error) {
        // код функции autorized
        std::string salt = salt_generator(16);
        std::string ok = "OK";
        std::string err = "ERR";
        std::string error;
        char msg[255];
        
        //Авторизация
        recv(work_sock, &msg, sizeof(msg), 0);
        std::string message = msg;
            std::string login, hashq;
            std::fstream file;
            file.open(file_name);
            getline (file, login, ':');
            getline (file, hashq);

        //СВЕРКА ЛОГИНОВ
        if(message == login){
            msgsend(work_sock,  err);
            error = "Ошибка логина";
            e_error.errors(error, file_error);
            close(work_sock);
            return 1;
        } else {
            //соль отправленная клиенту
            msgsend(work_sock,  salt);
            recv(work_sock, msg, sizeof(msg), 0);
            std::string sah = salt + hashq;
            std::string digest;
            digest = MD(sah);
            //СВЕРКА ПАРОЛЕЙ
            if(digest == msg){
                std::cout << digest << std::endl;
                std::cout << msg << std::endl;
                msgsend(work_sock,  err);
                error = "Ошибка пароля";
                e_error.errors(error, file_error);
                close(work_sock);
                return 1;
            } else {
                msgsend(work_sock,  ok);
            }
        }
        return 1;
    }


int Calculator::calc(int work_sock)
{
    int Quantity;
    int Length;
    float Vector_numbers;
    recv(work_sock, &Quantity, sizeof(Quantity), 0);
    for(int j=0; j<Quantity; j++) {
        recv(work_sock, &Length, sizeof(Length), 0);
        float Amount = 0;
        for(int i=0; i<Length; i++) {
            recv(work_sock, &Vector_numbers, sizeof(Vector_numbers), 0);
            Amount = Amount + Vector_numbers;
        }
        float Average_value;
        if (Amount > 2147483647) {
            Average_value = 2147483647;
            }
        else if (Amount < -2147483648) {
            Average_value = -2147483648;
            }
        else {
        	Average_value = Amount / Length;
        	}
        send(work_sock, &Average_value, sizeof(Average_value), 0);
    }

    std::cout << "Завршение работы программы" <<std::endl;
    close(work_sock);
    return 1;
}
