#include "calc.h"

int main(int argc, char *argv[]) {
    const struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"file", required_argument, 0, 'f'},
        {"port", required_argument, 0, 'p'},
        {"error", required_argument, 0, 'e'},
        {0, 0, 0, 0}
    };
    if (argc == 1) {
        std::cout << "Калькулятор" << std::endl;
        std::cout << "-h --help Помощь" << std::endl;
        std::cout << "-f --file Название файла" << std::endl;
        std::cout << "-p --port Порт" << std::endl;
        std::cout << "-e --error Файл ошибок" << std::endl;
    }
    int option;
    int option_index = 0;
    int port = 33333;
    std::string file_name = "/etc/vcalc.conf"; 
    std::string file_error = "/var/log/vcalc.log";
    std::string error;

    while ((option = getopt_long(argc, argv, "hf:p:i:e:", long_options, &option_index)) != -1) {
        switch (option) {
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

    Error errors;
    if(errors.er(file_name, file_error)==12) {
        std::cout<<"Ошибка открытия файла"<<std::endl;
        return 1;
    }

    Server server(errors);
    Authorized authorized(errors);
    Calculator calculator(errors);
    int s = server.self_addr(error, file_error, port);
    while(true) {
        int work_sock = server.client_addr(s, error, file_error);
        authorized.authorized(work_sock, file_name, file_error);
        calculator.calc(work_sock);
    }
    return 0;
}
