#pragma once

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <set>
#include <thread>
#include <stdexcept>

#include "InvertedIndex.h"
#include "ConverterJSON.h"

class SearchServer {
    InvertedIndex  invertedIndex;

public:
    /**
    * @param idx в конструктор класса передаётся ссылка на класс InvertedIndex,
    * чтобы SearchServer мог узнать частоту слов встречаемых в запросе
    */
    explicit SearchServer(InvertedIndex& idx) : invertedIndex(idx) {
    }
    //SearchServer(InvertedIndex& idx)  {
    //    invertedIndex = idx;
    //
    //}

    SearchServer() = default;

    /**
    * Метод обработки поисковых запросов
    * @param queries_input поисковые запросы взятые из файла requests.json
    * @return возвращает отсортированный список релевантных ответов для
    заданных запросов
    */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string>& queries);

    /**  Метод обработки поискового запроса (вспомогатеотный для search())
     * возвращает абсолютную реолевантность */
    std::vector<Entry> find(const std::string& querie);

    /** Oбёртка над invertedIndex.UpdateDocumentBase(inputDocs); */
    void UpdateDocumentBase(std::vector<std::string> inputDocs)  {
        invertedIndex.UpdateDocumentBase(std::move(inputDocs));
    }


};
