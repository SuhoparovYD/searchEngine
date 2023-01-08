#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>

#include "ConverterJSON.h"
#include "SearchServer.h"

int main() {
    using namespace std;

    ConverterJSON cj(true);  // + read config
    SearchServer ss;
    ss.UpdateDocumentBase(cj.GetTextDocuments());  // обновляем индекс в классе SearchServer

    // Флаг ручной обработки запроса (requests.json)
    // true - игнорирование наличия файла answers.json, (перезапись существующего файла)
    // false - обработка запроса только при отсутствии answers.json
    bool ignore = true;

    // Флаг индексирования
    // false - индексирование возможно, (обработан запрос на прежнем индексе)
    // true - индексирование проведено
    bool indexed =  true;

    // Флаг вывода сообщений об ошибках requests.json
    // 1 - отсутствие requests.json
    // 2 - отсутствие секции requests
    // 3 - ошибка в структуре requests.json
    int tell  = 0;

    bool appMode = true; // режим приложения

    while(true) {
        ifstream answer("answers.json");
        if(!answer || ignore) {                 // Есть необходимость обработки запроса?
            if(answer) answer.close();
            ignore = false;                     // Обработка запроса
            try {
                cj.putAnswers(ss.search(cj.GetRequests()));
                indexed = false;
                tell  = 0;
            }
            catch (const runtime_error &ex) {
                if (!indexed) {      // Индексирование
                    cj.GetConfig();
                    ss.UpdateDocumentBase(cj.GetTextDocuments());
                    indexed = true;
                    tell  = 1;          // блокировка сообщения об отсутствии requests.json
                } else
                    if (tell != 1) {        // Вывод сообщения об отсутствии requests.json
                        cerr << "\n" << cj.SearchServerName << " - " << ex.what();
                        tell  = 1;        // блокировка сообщения об отсутствии requests.json

                    }
            }
            catch (const invalid_argument &ex) {
                if (tell != 2) {    // Вывод сообщения об отсутствии секции requests
                    cerr << "\n" << cj.SearchServerName << " - " << ex.what();
                    tell  = 2;      // блокировка сообщения

                }
            }
            catch (const nlohmann::json::exception ex) {
                if (tell != 3) {    // Вывод сообщения об ошибках в requests.json
                    cerr << ex.what() << "\n";
                    tell  = 3;      // блокировка сообщения

                }
            }
        }
        else
            answer.close();
        // pause to next
        this_thread::sleep_for(chrono::milliseconds(10));   // 100 Hz
        cerr  << flush;
        if (appMode) {
            // Ручной режим
            int command;
            cout << "\nCommand: 1-Request, 2-Indexing, 3-Exit, 4-ServerMode _";
            cin >> command;
            // Выполнение команд
            if (command== 1) {                  // инициация обработки запроса
                ignore = indexed = true;
                tell = 0;
            }
            if(command==2) {                    // Индексирование
                indexed = tell = true;;
                cj.GetConfig();
                ss.UpdateDocumentBase(cj.GetTextDocuments());
            }
            if (command == 3) exit(0);
            if (command == 4) appMode = false;  // Переход в бесконечный цикл - анализ файлов
        }
    }  // while(true)

    return (0);
}
