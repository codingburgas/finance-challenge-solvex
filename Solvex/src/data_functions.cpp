#include "../headers/data_functions.h"
#include <fstream>

bool debugging_enabled = false;

//xor encryption and apparently it's insecure but better than nothing
std::string encryptDecrypt(std::string data, std::string password) {
    if (debugging_enabled) return data;


    password = "saltlol" + password;
    for (size_t i = 0; i < data.size(); ++i) {
        data[i] ^= password[i % password.size()];
        if (data[i] == '\0') data[i] ^= password[i % password.size()]; //file gets corrupted if the character becomes a newline so we switch it back, still secure probably
    }
    return data;
}


void accountSave(std::vector<TAB_STRUCT> account, std::string username, std::string password) {
    float sum;
    std::ofstream file(username + ".txt", std::ios::trunc);
    file << "PAS:" << encryptDecrypt(password, password) << std::endl;
    for (size_t i = 0; i < account.size(); i++) {
        sum = 0;
        file << "TAB:" << encryptDecrypt(account[i].tabName, password) << std::endl;

        for (size_t j = 0; j < account[i].transaction.size(); j++) {
            sum += account[i].transaction[j].amount;
        }

        file << "SUM:" << encryptDecrypt(std::to_string(sum), password) << std::endl;
        for (size_t j = 0; j < account[i].transaction.size(); j++) {
            file << "TRN:" << encryptDecrypt(account[i].transaction[j].note, password) << std::endl;
            file << "TRF:" << encryptDecrypt(std::to_string(account[i].transaction[j].amount), password) << std::endl;
        }
    }
    file.close();
}

fileError accountLoad(std::vector<TAB_STRUCT>* account, std::string username, std::string password, bool integrity_acknowledged) {
    std::ifstream file(username + ".txt", std::ios::in);
    if (!file) {
        return NOT_EXIST;
    }
    std::vector<TAB_STRUCT> altAccount;
    int tabIndex = -1;
    int transactionIndex = -1;
    float sum = 0;
    float realsum = 0;

    std::string line;
    getline(file, line);

    std::string temp = line.substr(0, 4);
    std::string data = line.substr(4);

    if (temp != "PAS:" || password != encryptDecrypt(data, password))
    {
        return WRONG_PASS;
    }

    while (getline(file, line)) {
        std::string temp = line.substr(0, 4);
        std::string data = encryptDecrypt(line.substr(4), password);
        if (temp == "TAB:")
        {
            if (tabIndex != -1)
            {
                for (size_t i = 0; i < altAccount[tabIndex].transaction.size(); i++) {
                    realsum += altAccount[tabIndex].transaction[i].amount;
                }
                if (sum != realsum && integrity_acknowledged == false)
                {
                    return INTEGRITY;
                }
                sum = 0;
                realsum = 0;
            }

            tabIndex++;
            altAccount.push_back({ data });
            transactionIndex = -1;
        }
        else if (temp == "SUM:")
        {
            sum = std::stof(data);
        }
        else if (temp == "TRN:")
        {
            altAccount[tabIndex].transaction.push_back({ data });
            transactionIndex++;
        }
        else if (temp == "TRF:")
        {
            altAccount[tabIndex].transaction[transactionIndex].amount = std::stof(data);
        }
        else
        {
            return CORRUPTED;
        }
    }
    *account = altAccount;
    return FINE;
}