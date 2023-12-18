/** @file
* @author Гурин Д.А.
* @version 1.0
* @date 17.12.2023
* @copyright ИБСТ ПГУ
* @warning Курсовая работа 
* @brief Заголовочный файл программы сервера
*/
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



/** @brief Класс ошибок
* @details Данный класс создан для 
* отслеживания исключений в программе и записи в файл с ошибками
*/
class Error
{
public:
    Error(); ///< Конструктор по умолчанию

    static void errors(std::string error, std::string name); ///< Статический метод для вывода ошибки

    static int er(std::string file_name, std::string file_error); ///< Статический метод для обработки ошибок
};


/** @brief Класс Сервер
* @details Данный класс создан для 
* для установки адреса сервера и клиента
*/
class Server
{
private:
    Error e_error; ///< Приватный член класса Error

public:
    Server(Error err) : e_error(err) { ///< Конструктор класса Server
        e_error = err; ///< Присваивание объекта err приватному члену e_error
    }
    
	/** @brief Функция установки сетевого соединения
    * @details Функция  используется для определения 
    * собственного сетевого адреса и порта
    * @param [in] error Строка с сообщением об ошибки
    * @param [in] file_error Имя файла для записи ошибок
    * @param [in] port Порт
    * @return Информацию о выполнении операции
    * @throw error, если имеются ошибки при подключении
	*/
    int self_addr(std::string& error, std::string& file_error, int port); ///< Метод для установки адреса сервера
    
    /** @brief Функция установки сетевого соединения
    * @details Функция используется для получения адреса клиента, с которым 
    * установлено сетевое соединение по заданному сокету
    * @param [in] s Передаваемый сокетом для работы с сетью
    * @param [in] error Имя файла базы данных
    * @param [in] file_error Имя файла для записи ошибок
    * @return Информацию о выполнении операции
    * @throw error, если имеются ошибки при подключении
	*/
    int client_addr(int s, std::string& error, std::string& file_error); ///< Метод для установки адреса клиента
};

/** @brief Класс авторизации
* @details Данный класс создан для 
* авторизации клиента на сервере и 
* проверки логина и пароля в базе данных 
* при подключении пользователя
* @warning Пользователь должен быть в базе данных
*/
class Authorized
{
private:
    Error e_error; ///< Приватный член класса Error

    std::string salt_generator(const std::size_t length); ///< Метод для генерации соли
    void msgsend(int work_sock, const std::string& mess); ///< Метод для отправки сообщения
    std::string MD(const std::string& sah); ///< Метод для вычисления хэша

public:
    Authorized(Error err) : e_error(err) { ///< Конструктор класса Authorized
        e_error = err; ///< Присваивание объекта err приватному члену e_error
    }
    
	/** @brief Функция авторизации
    * @details Функция выполняет авторизации клиента на сервере
    * @param [in] work_sock Передаваемый сокетом для работы с сетью
    * @param [in] file_name Имя файла базы данных
    * @param [in] file_error Имя файла для записи ошибок
    * @return Информацию о статусе авторизации пользователя
    * @throw error, если введены некорректные данные логина или пароля
	*/
    int authorized(int work_sock, string file_name, string file_error); ///< Метод для авторизации пользователя
};

/** @brief Класс калькулятор
* @details Данный класс создан для вычисления 
* среднего арифметического элементов вектора
* @warning Поддерживаемый тип данных float
*/
class Calculator
{
private:
    Error e_error; ///< Приватный член класса Error

public:
    Calculator(Error err) : e_error(err) { ///< Конструктор класса Calculator
        e_error = err; ///< Присваивание объекта err приватному члену e_error
    }
    
	/** @brief Функция вычисления
    * @details Функция вычисляет значение 
    * среднего арифметического элементов вектора
    * @param [in] work_sock Передаваемый сокет через 
    * который осуществляется обмен 		
    * данными между клиентом и сервером
    * @return Результат математических вычислений
    */
    int calc(int work_sock); ///< Метод для выполнения вычислений
};
