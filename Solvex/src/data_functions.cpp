#include "../headers/data_functions.h"
#include <fstream>
void accountSave(std::vector<TAB_STRUCT> account, std::string username, std::string password) {
    float sum;
    std::ofstream file(username + ".txt", std::ios::trunc);
    for (size_t i = 0; i < account.size(); i++) {
        sum = 0;
        file << "TAB:" << account[i].tabName << std::endl;

        for (size_t j = 0; j < account[i].transaction.size(); j++) {
            sum += account[i].transaction[j].amount;
        }

        file << "SUM:" << sum << std::endl;
        for (size_t j = 0; j < account[i].transaction.size(); j++) {
            file << "TRN:" << account[i].transaction[j].note << std::endl;
            file << "TRF:" << account[i].transaction[j].amount << std::endl;
        }
    }
    file.close();
}

fileError accountLoad(std::vector<TAB_STRUCT> *account, std::string username, std::string password) {
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
    while (getline(file, line)) {
        std::string temp = line.substr(0, 4);
        std::string data = line.substr(4);
        if (temp == "TAB:")
        {
            if (tabIndex != -1)
            {
                for (size_t i = 0; i < altAccount[tabIndex].transaction.size(); i++) {
                    realsum += altAccount[tabIndex].transaction[i].amount;
                }
                if (sum != realsum)
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
}
