#pragma once
#include <iostream>
#include <vector>
#include <string>

enum fileError {
    FINE,
    NOT_EXIST,
    INTEGRITY,
    CORRUPTED,
    WRONG_PASS
};

struct TRANSACTION_STRUCT {
    std::string note;
    float amount;
};

struct TAB_STRUCT {
    std::string tabName;
    std::vector<TRANSACTION_STRUCT> transaction;
};

void accountSave(std::vector<TAB_STRUCT> account, std::string username, std::string password);

fileError accountLoad(std::vector<TAB_STRUCT>* account, std::string username, std::string password, bool integrity_acknowledged);