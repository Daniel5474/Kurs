#include "calc.h"
void msgsend(int work_sock, string mess){
    char *buffer = new char[4096];
    std::strcpy(buffer, mess.c_str());
    send(work_sock, buffer, mess.length(), 0);
}

std::string MD(std::string sah){
Weak::MD5 hash;
    std::string digest;
    StringSource(sah, true,  new HashFilter(hash, new HexEncoder(new StringSink(digest))));  // строка-приемник
      return digest;
 }
 
std::string salt_generator(const std::size_t length) {
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
 
void errors(std::string error, std::string name){
    std::ofstream file;
    file.open(name, std::ios::app);
    if(file.is_open()){
        time_t seconds = time(NULL);
        tm* timeinfo = localtime(&seconds);
        file<<error<<':'<<asctime(timeinfo)<<std::endl;
        std::cout << "error: " << error << std::endl;
    }
}
int er(std::string file_name, std::string file_error){
        std::fstream file;
        file.exceptions(std::ifstream::badbit | std::ifstream::failbit);
        try{
        file.open(file_name);
        return 1;
        }catch(const std::exception & ex){
        std::string error = "error open file";
        errors(error, file_error);
        return 12;
        }
        }

int Server::self_addr(std::string error, std::string file_error, int port) {
    // код функции self_addr
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int on = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof on);
    struct timeval timeout {10, 0}; // Установка таймаута в 10 секунд
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    
    if (sock < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
        sockaddr_in self_addr;
        self_addr.sin_family = AF_INET;
        self_addr.sin_port = htons(port);
        self_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        std::cout << "Ожидание подключения клиента...\n";
        int b = bind(sock, (const sockaddr*)&self_addr, sizeof(sockaddr_in));
        if (b == -1) {
            std::cout << "Ошибка\n";
            error = "Ошибка";
            errors(error, file_error);
            return 1;
        }
        
        if (listen(sock, SOMAXCONN) == -1) {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        
        return sock;
    }

int Server::client_addr(int s, std::string error, std::string file_error) {
        // код функции client_addr
        sockaddr_in * client_addr = new sockaddr_in;
        socklen_t len = sizeof (sockaddr_in);
        
        struct timeval timeout {10, 0}; // Установка таймаута в 10 секунд
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        
        int work_sock = accept(s, (sockaddr*)(client_addr), &len);
        if(work_sock == -1) {
            std::cout << "Ошибка\n";
            error = "Ошибка";
            errors(error, file_error);
            return 1;
        }
        else {
            //Успешное подключение к серверу
            std::cout << "Клиент успешно подключился!\n";
            return work_sock;
        }
    }

int authorized(int work_sock, std::string file_name, std::string file_error) {
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
            errors(error, file_error);
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
                errors(error, file_error);
                close(work_sock);
                return 1;
            } else {
                msgsend(work_sock,  ok);
            }
        }
        return 1;
    }


int math(int work_sock)
{
    int kolvo;
    int numb;
    float vect;
    recv(work_sock, &kolvo, sizeof(kolvo), 0);
    //цикл векторов
    for(int j=0; j<kolvo; j++) {
        recv(work_sock, &numb, sizeof(numb), 0);//прием длинны для первого вектора
        float sum = 0;
        //цикл значений
        for(int i=0; i<numb; i++) {
            recv(work_sock, &vect, sizeof(vect), 0);
            sum = sum + vect;
        }
        float avg;
        avg = sum / numb;
        send(work_sock, &avg, sizeof(avg), 0);
    }

    std::cout << "Завршение работы программы" <<std::endl;
    close(work_sock);
    return 1;
}
