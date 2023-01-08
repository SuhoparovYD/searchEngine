#include "../include/ConverterJSON.h"
#include "gtest/gtest.h"

// Проверка метода GetResponsesLimit()
TEST(ConverterJSON, getResponsesLimit) {
    ConverterJSON c;
    int responsesLimit = c.GetResponsesLimit();  // read config

    EXPECT_EQ(responsesLimit, 5);  // "maxResponses": 5    - in config.json
}

TEST(ConverterJSON, getConfig) {
    ConverterJSON c;
    c.GetConfig();

    EXPECT_EQ(c.SearchServerName, "SkillboxSearchEngine");  //  in config.json
}
// Проверка методов класса ConverterJSON:
// GetTextDocuments(), GetConfig(), readConfig()
TEST(ConverterJSON, GetTextDocuments) {
    ConverterJSON cj(true);  // + read config
    auto texts = cj.GetTextDocuments();

    EXPECT_EQ(texts.at(1).substr(0,18), "This article needs");    // file002.txt
}
// Проверка метода GetRequests()
TEST(ConverterJSON, getRequests) {
    ConverterJSON c;
    auto queries = c.GetRequests();

    EXPECT_EQ(queries.at(0), "some words terms");  //  first in request.json
}