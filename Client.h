//
// Created by bronc on 06.06.2023.
//

#ifndef PROJEKT_ZALICZENIOWY_CLIENT_H
#define PROJEKT_ZALICZENIOWY_CLIENT_H
#include <iostream>
using namespace std;
enum Gender{male,female,other};
class Client {
    string firstName;
    string lastName;
    string address;
    string telNum;
    string accNum;
    Gender gender;
    string readGender();
    void changeGender();
public:
    Client(string fN = "", string lN = "", string aD = "", string tN = "", string aN = "",Gender g = other);
    void modifyUserData();
    void showUserData();
    void saveDataToCsv();
    void readDataFromCsv();
};


#endif //PROJEKT_ZALICZENIOWY_CLIENT_H
