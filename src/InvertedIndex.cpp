
#include "InvertedIndex.h"

void InvertedIndex::UpdateDocument(size_t docId) {
    // для одного файла
    std::map<std::string, size_t> frecFileDict;               // word:count - частотный словарь для одного файла
    std::string text = docs.at(docId);

    std::stringstream textStream = (std::stringstream)text;
    std::string word; // слово
    while (!textStream.eof()) {
        textStream >> word;
        ++frecFileDict[word];   // частота вхождений слова
    }

    freqMutex.lock();
    for (auto & item : frecFileDict) {                               // загрузка в общий словарь
        if (freqDictionary.count(item.first)) {                             // if word exist
            std::vector<Entry> wordEntry = freqDictionary.at(item.first);   // this word in freqDictionary
            wordEntry.push_back(Entry{ docId, item.second });
            freqDictionary.at(item.first) = wordEntry;                      // модифицируем словарь
        }
        else                                                                   // добавдяем слово в словарь
            freqDictionary[item.first] = std::vector<Entry>{ Entry{docId, item.second} };
    }
    freqMutex.unlock();
}

// поток индексирования документов
void InvertedIndex::makeThreads() {
    while (true) {
        fileMutex.lock();
        size_t docId = head++;
        fileMutex.unlock();
        if (docId >= docs.size()) break; // > числa файлов
        UpdateDocument(docId);
    }
}

void InvertedIndex::UpdateDocumentBase(std::vector<std::string> inputDocs) {
    docs = std::move(inputDocs);
    screenMutex.lock();
    std::cout << "Indexing, hardware_concurrency: " << std::thread::hardware_concurrency() << "\n";
    screenMutex.unlock();
    // создание потоков в оптимальном количестве для данного процессора(ов))
    std::vector<std::thread> threads;
    for (unsigned i = 0; i < std::thread::hardware_concurrency(); i++)
        threads.emplace_back(&InvertedIndex::makeThreads, this);
    // завершение потоков
    for (unsigned i = 0; i < std::thread::hardware_concurrency(); i++)
        threads.at(i).join();

    std::cout << "\n";
}

std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) const {
    if (freqDictionary.count(word))
        return freqDictionary.at(word);
    else
        return std::vector<Entry>{};  // empty
}

