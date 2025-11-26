#ifndef CUSTOMER_H_INCLUDED
#define CUSTOMER_H_INCLUDED

#include <iostream>

#include <vector>
#include <stdexcept>  // for invalid_argument
#include <fstream> //including the file stream library
#include <sstream> //including the string stream library
#include <limits> //numeric_limits

using namespace std;

class Customer {
private:
    string userName;
    string password;
    string ID;
    string name;
    string contact;
    string address;
    string email;
    string dateOfBirth;

public:
    vector<Customer> customers;

    Customer() = default;
    Customer(string uN, string p,string id, string n, string c, string a, string e, string dOB)
    {
        userName = uN;
        password = p;
        ID = id;
        name = n;
        contact = c;
        address = a;
        email = e;
        dateOfBirth = dOB;
    }

    //setter and getter function
    void setUserName(string uN){
        userName = uN;
    }
    string getUserName(){
        return userName;
    }
    void setPassword(string p){
        password = p;
    }
    string getPassword(){
        return password;
    }
    void setID(string id){
        ID = id;
    }
    string getID(){
        return ID;
    }
    void setName(string n){
        name = n;
    }
    string getName(){
        return name;
    }
    void setContact(string c){
        contact = c;
    }
    string getContact(){
        return contact;
    }
    void setAddress(string a){
        address = a;
    }
    string getAddress(){
        return address;
    }
    void setEmail(string e){
        email = e;
    }
    string getEmail(){
        return email;
    }
    void setDateOfBirth(string dOB){
        dateOfBirth = dOB;
    }
    string getDateOfBirth(){
        return dateOfBirth;
    }

    // read personalDetails
    static void readCSV(vector<Customer>& customers){
        string filename = "customerDetails.csv";
        ifstream file(filename);
        string line;

        if(file.is_open()){
            while (getline(file, line)) {
                istringstream iss(line);
                string uN, p, userId, n, c, a, e, dOB;

                getline(iss, uN, ',');
                getline(iss, p, ',');
                getline(iss, userId, ',');
                getline(iss, n, ',');
                getline(iss, c, ',');
                getline(iss, a, ',');

                if (a.front() == '"') {  // address field might contain commas and be quoted
                    string temp;
                    while (a.back() != '"') {
                        getline(iss, temp, ',');
                        a += "," + temp;
                    }
                    a = a.substr(1, a.length() - 2); // remove surrounding quotes
                }
                getline(iss, e, ',');
                getline(iss, dOB);

                if (!uN.empty() && !p.empty() && !userId.empty() && !n.empty() && !c.empty() && !a.empty() && !e.empty() && !dOB.empty()) {
                    customers.push_back(Customer(uN, p, userId, n, c, a, e, dOB));
                } else {
                    cerr << "Error reading line from CSV: " << line << endl;
                }
            }
            file.close();
        }
        else{
            cerr<< "Failed to open" << endl;
        }
    }

    // display the personal details
    void display() {
        cout << "\n"
              << "User Name     : " << userName << "\n"
              << "Password      : " << password << "\n"
              << "User ID       : " << ID << "\n"
              << "Name          : " << name << "\n"
              << "Contact       : " << contact << "\n"
              << "Address       : " << address << "\n"
              << "Email         : " << email << "\n"
              << "Date of Birth : " << dateOfBirth << "\n";

    }

    //update customer details
    static void updatePersonalDetails(Customer* c, vector<Customer>& customers){

        bool updated = false;

        int choicePD;

        do{
            cout << "\nUPDATE PERSONAL DETAILS" << endl;
            cout << "1. User Name" << "\n"
                << "2. Password" << "\n"
                << "3. Contact" << "\n"
                << "4. Address" << "\n"
                << "5. Exit" << "\n"
                << "***User ID and Date of Birth cannot be change***" << endl;

            cout << "->";

            try{
                cin >> choicePD;
                if(cin.fail() || choicePD<1 || choicePD>5)
                    throw invalid_argument("Invalid choice. Please try again");
            }catch (const invalid_argument& e){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout << e.what() << endl;
                continue;
            }
            cin.ignore();

            switch(choicePD){
                case 1:{
                    string newUsername;
                    cout << "Enter new user name: ";
                    getline(cin, newUsername);
                    c->setUserName(newUsername);
                    updated = true;
                    break;}

                case 2: {
                    string newPassword;
                    cout << "Enter new password: ";
                    getline(cin, newPassword);
                    c->setPassword(newPassword);
                    updated = true;
                    break;
                }

                case 3: {
                    string newContact;
                    cout << "Enter new contact: ";
                    getline(cin, newContact);
                    c->setContact(newContact);
                    updated = true;
                    break;
                }

                case 4: {
                    string newAddress;
                    cout << "Enter new address: ";
                    getline(cin, newAddress);
                    c->setAddress(newAddress);
                    updated = true;
                    break;
                }

                case 5:
                    cout << "Exiting update menu." << endl;
                    break;

                default:
                    break;
            }

            if (updated) {
                string filename = "customerDetails.csv";
                ofstream file(filename);

                if (file.is_open()) {
                    for (auto& customer : customers) {
                        file << customer.getUserName() << ","
                            << customer.getPassword() << ","
                            << customer.getID() << ","
                            << customer.getName() << ","
                            << customer.getContact() << ","
                            << "\"" << customer.getAddress() << "\","
                            << customer.getEmail() << ","
                            << customer.getDateOfBirth() << "\n";
                    }
                    file.close();
                    updated = false; // Reset the flag after saving
                } else {
                    cerr << "Failed to open file for writing" << endl;
                }
            }

        }while(choicePD != 5);
    }

    // if in the starting menu choose 1, then will jump to this
    void personalDetailsMenu(Customer* c, vector<Customer>& customers){

        int choice;

        do{
            cout << "\nWelcome to FOODIEEEEEEE PERSONAL DETAILS MENU" << endl;
            cout << "1. Display Personal Details" << endl;
            cout << "2. Update Personal Details" << endl;
            cout << "3. Exit" << endl;

            cout << "->";
            cin >> choice;

            switch(choice){
            case 1:
                c->display();
                break;

            case 2:
                updatePersonalDetails(c, customers);
                break;

            case 3:
                break;

            default:
                    cout << "Invalid. Please enter again." << endl;
            }
        }while(choice != 3);
    }
};


#endif // CUSTOMER_H_INCLUDED
