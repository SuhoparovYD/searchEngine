#pragma once
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <thread>
#include <mutex>
#include <cmath>
#include <stdexcept>
//#include <nlohmann\json.hpp>

    /**  Cтруктура для хранения частоты слов, встречаемых в тексте
    doc_id — номер элемента в векторе docs, по которому хранится текст;
    count — число, которое обозначает, сколько раз ключевое слово встретилось в
    документе doc_id.
    */
struct Entry {
    size_t docId, count;
    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (docId == other.docId &&
                count == other.count);
    }
};

class InvertedIndex {
    std::vector<std::string> docs;                 // список содержимого документов
    std::map<std::string, std::vector<Entry>> freqDictionary; // частотный словарь

    std::mutex fileMutex;
    std::mutex freqMutex;
    std::mutex screenMutex;

    size_t head{ 0 };   // указатедь(относительный) на загружаемые файлы

public:
    InvertedIndex() = default;
 //Если  мьютексы внести в класс, то необходимо:
    InvertedIndex(InvertedIndex& other) {
        this->freqDictionary = other.freqDictionary; // частотный словарь
        //other.freqDictionary.empty();
    };


    /**
    * Обновить или заполнить базу документов, по которой будем совершать поиск
    * @param texts_input содержимое документов
    */
    void UpdateDocumentBase(std::vector<std::string> inputDocs);

    /** Обновить или заполнить базу документов для одного файла */
    void UpdateDocument(size_t docId);
    /** создание потоков индексирования текста документов */
    void makeThreads();

    /**
    * Метод определяет количество вхождений слова word в загруженной базе документов
    * @param word слово, частоту вхождений которого необходимо определить
    * @return возвращает подготовленный список с частотой слов
    */
    [[nodiscard]]
    std::vector<Entry> GetWordCount(const std::string& word) const;
};
