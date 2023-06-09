//
// Created by bronc on 06.06.2023.
//

#include <fstream>
#include "Client.h"
#include <ctime>
Client::Client(string fN, string lN, string aD, string tN, string aN, Gender g) {
    firstName = fN;
    lastName = lN;
    address = aD;
    telNum = tN;
    accNum = aN;
    gender = g;

}

string Client::readGender() {
    switch (gender) {
        case 0:
            return "male";
            break;
        case 1:
            return "female";
            break;
        case 2:
            return "other";
    }

}

void Client::changeGender() {
    string newGender;
    cout << "New gender (male, female or other)";
    cin >> newGender;
    if (newGender == "male") {
        gender = male;
    } else if (newGender == "female") {
        gender == female;
    } else if (newGender == "other") {
        gender = other;
    } else {
        cout << "Enter the correct data" << endl;
    }
}

void Client::modifyUserData() {
    string newFirstName, newLastName, newAddress, newTelNumber, newAccNumber, newGender;
    int choice;

    cout << "Choose what u want to modify: " << endl;
    cout << "1 - First name and last name\n"
         << "2- Addres\n"
         << "3- Telephone number\n"
         << "4- Account number\n"
         << "5- Gender\n"
         << "6 - All data\n";
    cin >> choice;
    switch (choice) {
        case 1:

            cout << "New first name: ";
            cin >> newFirstName;
            cout << "New last name: ";
            cin >> newLastName;

            lastName = newLastName;
            firstName = newLastName;
            break;
        case 2:
            cin.ignore();
            cout << "New addres: ";
            getline(cin, newAddress);
            address = newAddress;
            break;
        case 3:
            cout << "New telephone number: ";
            cin >> newTelNumber;
            if (newTelNumber.length() != 9) {
                cout << "Telephone number have nine numbers" << endl;
                cin >> newTelNumber;
            }
            telNum = newTelNumber;
            break;
        case 4:
            cout << "New account number: ";
            cin >> newAccNumber;
            accNum = newAccNumber;
            break;
        case 5:
            changeGender();
            break;
        case 6:
           setUserData();
            break;
        default:
            cout << "Niepoprawny wybor" << endl;
    }
    saveUserData();
}

void Client::showUserData() {
    cout << "First name: " << firstName << endl;
    cout << "Last name: " << lastName << endl;
    cout << "Gender: " << readGender() << endl;
    cout << "Addres: " << address << endl;
    cout << "Telephone number: " << telNum << endl;
    cout << "Account number: " << accNum << endl;
}

void Client::saveUserData() {
    ofstream outputFile(R"(C:\Studia\c++\Projekt zaliczeniowy - sklep\client.csv)", ios::app);
    if (outputFile.is_open()) {
        string dataToSave =
                firstName + ";" + lastName + ";" + readGender() + ";" + address + ";" + telNum + ";" + accNum+"\n";
        outputFile << dataToSave;
        outputFile.close();
    } else {
        cout << "Nie udalo sie otworzyc pliku" << endl;
    }
    ofstream binaryOutputFile(R"(C:\Studia\c++\Projekt zaliczeniowy - sklep\client.bin)", ios::binary | ios::app);
    if (binaryOutputFile.is_open()) {
        binaryOutputFile.write(reinterpret_cast<const char *>(&firstName), sizeof(firstName));
        binaryOutputFile.write(reinterpret_cast<const char *>(&lastName), sizeof(lastName));
        binaryOutputFile.write(reinterpret_cast<const char *>(&address), sizeof(address));
        binaryOutputFile.write(reinterpret_cast<const char *>(&telNum), sizeof(telNum));
        binaryOutputFile.write(reinterpret_cast<const char *>(&accNum), sizeof(accNum));
        binaryOutputFile.close();
    } else {
        cout << "Nie udało się otworzyć pliku binarnego" << endl;
    }
}


void Client::addToTransaction(map<int, Products> &productMap, int id, int quantity) {
    int i = 1;
    cout << "Wybierz id produktu który chcesz kupic" << endl;
    auto pm = productMap.find(id);
    pm->second.quantity = quantity;

    if (transaction.empty()) {
        transaction[i] = pm->second;
    } else {
        auto tr = transaction.end();
        i = tr->first + 1;
        transaction[i] = pm->second;
    }

}

float Client::getTotalTransactionAmount() {
    float totalAmount = 0.0;
    for (auto &it: transaction) {
        totalAmount += (it.second.getPriceWithVat()) * it.second.quantity;
    }
    return totalAmount;
}

void Client::submitTransaction(float totalAmount) {
    string finalTransaction;
    int accept;
    setPaymentMethod();
    cout << "Accept the transaction 1- Accept 0-decline" << endl;
    cin >> accept;
    if (accept == 1) {
        finalTransaction =
                "First Name: " + firstName + "\n" + "Last Name: " + lastName + "\n" + "Gender: " + readGender() + "\n" +
                "Addres: " + address + "\n" + "Account Number: " + accNum + "\n" + "Telephone Number: " + telNum + "\n" +
                "\n" +
                "Transaction List: " + "\n" + Products::transactionsToString(transaction) + "\n" + "Total amount: " +
                to_string(totalAmount) + "\n" + "Payment Method: " + readPaymentMethod()+"\n"+"Date: " + getTime();
    } else if (accept == 0) {
        int deleteBasket;
        cout << "Did you want to delete basket? 1-Yes, 2-No" << endl;
        cin >> deleteBasket;
        if (deleteBasket == 1) {
            for (int i = 0; i < transaction.size(); i++) {
                transaction.erase(i);
            }
        }
    } else {
        cout << "Invalid choice" << endl;
    }
    cout << finalTransaction;
    transactionHistory.push_back(finalTransaction);
    saveTransactionHistory();
}

void Client::saveTransactionHistory() {
    ofstream outputFile;
    outputFile.open(R"(C:\Studia\c++\Projekt zaliczeniowy - sklep\TransactionHistory.txt)", ios::app);
    if (outputFile.is_open()) {

        for (const auto &it: transactionHistory) {
            cout << it;
            outputFile << it;
        }
    }
    ofstream binaryOutputFile(R"(C:\Studia\c++\Projekt zaliczeniowy - sklep\TransactionHistory.bin)",
                              ios::binary | ios::app);
    if (binaryOutputFile.is_open()) {
        for (const auto &transaction: transactionHistory) {
            int messageLength = transaction.size();
            binaryOutputFile.write(reinterpret_cast<const char *>(&messageLength), sizeof(messageLength));
            binaryOutputFile.write(transaction.c_str(), messageLength);
        }
        binaryOutputFile.close();
    }
}

void Client::displayCurrentTransaction() {
    if (transaction.empty()) {
        cout << "Transaction is empty" << endl;
    } else {
        for (const auto &it: transaction) {
            cout << it.first;
            Products p = it.second;
            p.showData();
            cout << " quantity: " << it.second.quantity;
        }
    }

}

void Client::modifyCurrentTransaction(float &totalAmount) {
    int id;
    int choice;
    displayCurrentTransaction();
    cout << "Choice what you want to modify" << endl;
    cin >> id;
    auto it = transaction.find(id);
    cout << "What u want to do:\n 1-Change a quantity\n 2-Delete a product" << endl;
    cin >> choice;
    switch (choice) {
        case 1:
            cout << "Quantity";
            cin >> it->second.quantity;
            break;
        case 2:
            totalAmount -= it->second.getPriceWithVat() * it->second.quantity;
            transaction.erase(id);
            break;
        default:
            cout << "Zły wybór" << endl;
    }
}

void Client::setPaymentMethod() {
    int choice;
    cout << "Chose payment method 0-elixir 1-blik 2-cash" << endl;
    cin >> choice;
    switch (choice) {
        case 0:
            method = elixir;
            break;
        case 1:
            method = blik;
            break;
        case 2:
            method = cash;
            break;
        default:
            cout << "Invalid choice" << endl;
    }
}

string Client::readPaymentMethod() {
    switch (method) {
        case elixir:
            return "elixir";
        case blik:
            return "blik";
        case cash:
            return "cash";
    }
}

void Client::setUserData() {
    string newFirstName, newLastName, newAddress, newTelNumber, newAccNumber, newGender;
    cin.ignore();
    cout << "New first name: ";
    getline(cin, newFirstName);
    cout << "New last name: ";
    getline(cin, newLastName);
    cout << "New address: ";
    getline(cin, newAddress);
    if(newAddress.length()<=0){
        cout<<"You must enter yours address"<<endl;
        getline(cin,newAddress);
    }
    cout << "New telephone number: ";
    getline(cin, newTelNumber);
    if (newTelNumber.length() != 9) {
        cout << "Telephone number have nine numbers" << endl;
        getline(cin, newTelNumber);
    }
    cout << "New account number: ";
    getline(cin, newAccNumber);
    firstName = newFirstName;
    lastName = newLastName;
    changeGender();
    address = newAddress;
    telNum = newTelNumber;
    accNum = newAccNumber;
}

string Client::getTime() {
    string data;
    time_t rawTime;
    struct tm * timeInfo;
    time ( &rawTime );
    timeInfo = localtime ( &rawTime );
    data = asctime(timeInfo);
    return data;
}
