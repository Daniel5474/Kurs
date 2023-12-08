#include "calc.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    const struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"file", required_argument, 0, 'f'},
        {"port", required_argument, 0, 'p'},
        {"error", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };
	if(argc == 1){
        std::cout << "Kалькулятор"  << std::endl;
        std::cout << "-h --help Помощь" << std::endl;
        std::cout << "-f --file Название файла" << std::endl;
        std::cout << "-p --port Порт" << std::endl;
        std::cout << "-e --error Файл ошибок" << std::endl;
    }
    int opt;
    int option_index = 0;
    int port = 33333;
    std::string file_name = "/ect/vcalc.conf";
    std::string file_error = "/var/log/vcalc.log";
    std::string error;

    while ((opt = getopt_long(argc, argv, "hf:p:i:e:", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'h':
                std::cout << "Векторный калькулятор выполняющий действие нахождения среднего арифметического" << std::endl;
                std::cout << "Инструкция: -f База данных пользователей -p Порт -e Файл ошибок" << std::endl;
                std::cout << "-h Помощь" << std::endl;
                std::cout << "-f Название файла" << std::endl;
                std::cout << "-p Порт" << std::endl;
                std::cout << "-e Файл ошибок" << std::endl;
                return 1;
            case 'f': {
                file_name = std::string(optarg);
            }
                break;
            case 'p': {
                port = stoi(std::string(optarg));
            }
                break;
            case 'e': {
                file_error = std::string(optarg);
            }
                break;
        }
    }

    if (er(file_name, file_error) == 12) {
        std::cout << "Ошибка открытия файла" << std::endl;
        return 1;
    }

    Server Server;

    while (true) {
        int s = Server.self_addr(error, file_error, port);
        int work_sock = Server.client_addr(s, error, file_error);

        authorized(work_sock, file_name, file_error);
        math(work_sock);

        return 0;
    }
}
