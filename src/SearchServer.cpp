
#include "SearchServer.h"

/**  Метод поиска по запросу (вспомогатеотный для search())  */
std::vector<Entry> SearchServer::find(const std::string& querie){
    std::stringstream querieStream = (std::stringstream)querie;
    std::set<std::string> words;
    std::string word; // слово запроса
    while (!querieStream.eof()) {  // разбор по словам
        querieStream >> word;
        words.emplace(word);       // набор уникальных слов запроса
    }
    std::vector<std::vector<Entry>> foundWords;
    for (auto& word : words)                // по уникальным словам запроса
        foundWords.push_back(invertedIndex.GetWordCount(word));

    // сортировка по возрастанию
    if (foundWords.size())
        sort(foundWords.begin(), foundWords.end(), [](auto fw1, auto fw2) {
            return fw1.size() < fw2.size();
        });
    auto foundAll = foundWords.at(0);   // здесь собираем пересечение множеств по docId, 0-min
    for (std::vector<Entry>::iterator iter = foundAll.begin(); iter != foundAll.end(); iter++) {    // doc с 1-м словом
        for (int i = 1; i < foundWords.size(); i++) {                // по документам со 2 - n словом
            bool found = false;
            for (auto &it : foundWords.at(i))
                if (it.docId == iter->docId) {
                    found = true;
                    iter->count += it.count;  // собираем абсолютную релевантность
                    break;
                }
            if (!found) {
                iter->count = 0;  // нет такого docId
                break;
            }
        }
    }
    std::vector<Entry> absResponse;  //  выбираем док-ты со всеми словами
    for (auto& doc : foundAll)
        if (doc.count)
            absResponse.push_back(doc);
    return  absResponse;
}

/**   Метод обработки поисковых запросов */
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string>& queries) {
    ConverterJSON c;
    int responsesLimit = c.GetResponsesLimit();  // read config
    std::cout << "Responses : \n" ;  // для информации
    std::vector<std::vector<RelativeIndex>> responses;  // ответы
    for (auto&  querie: queries) {

        auto absResponse = find(querie);

        std::vector<RelativeIndex> response;
        float maxrel = 1;
        //сортировка  по убыванию
        if (absResponse.size()) {
            sort(absResponse.begin(), absResponse.end(), [](auto en1, auto en2) {
                return en1.count > en2.count;
            });
            maxrel = (float)absResponse.begin()->count;

            int responsesCount = 0;      // cчетчик для responsesLimit
            for (auto docEntry : absResponse) {
                if (responsesCount++ >= responsesLimit) break;
                response.push_back({ docEntry.docId, docEntry.count / maxrel});
            }
        }
        // для информации
        for (auto& an : response) std::cout << an.docId << ":" << an.rank << "\t";
        std::cout << "\n";

        responses.push_back(response);
    }
    return responses;
}
