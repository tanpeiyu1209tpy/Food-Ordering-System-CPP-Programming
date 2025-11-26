#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include "customer.h"
#include "restaurant.h"
#include "payment.h"


using namespace std;

void mainMenu(){
    cout << "\nWelcome to FOOD FRENZY FEEDER :)" << endl;
    cout << "1. Personal Details" << endl;
    cout << "2. Food Ordering" << endl;
    cout << "3. Exit" << endl;
}

int main()
{
    vector<Customer> customers;
    Customer::readCSV(customers);


    vector<RestaurantBase*> restaurants;
    RestaurantBase::readCSV(restaurants);

    string userName, password;
    int attempts = 0;
    const int maxAttempts = 3;

    bool loginSuccess = false;
    Customer* c = nullptr;
    Restaurant* r = nullptr;
    Payment* p = new Payment();

    while(attempts < maxAttempts){
        cout << "Enter user name: ";
        getline(cin, userName);
        cout << "Enter password: ";
        getline(cin, password);

        for (auto& customer : customers) {
            if (customer.getUserName() == userName && customer.getPassword() == password) {
                loginSuccess = true;
                c = &customer;
                cout << "\nLogin successful!" << endl;
                break;
            }
        }

        if(loginSuccess){
            break;
        }
        else{
            attempts++;
            cerr << "Invalid username or password. Attempts left: " << (maxAttempts-attempts) << endl;
        }
    }


    if (loginSuccess && c) {
        int choice = 0;
        while (choice != 3) {  // Loop until the user chooses to exit
            mainMenu();
            cout << "->";
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
                case 1:
                    c->personalDetailsMenu(c, customers);
                    break;
                case 2:{
                    p->menu(c, r , restaurants);
                    break;
                }
                case 3:
                    cout << "Exiting..." << endl;
                    break;
                default:
                    cout << "Invalid. Please enter again." << endl;
            }
            if (choice != 3) {  // Add a delay before redisplaying the menu
                delay(1);
            }
        }
    }else {
        attempts++;
        cout << "\nMaximum login attempts exceeded. Exiting program." << endl;
    }

    delete p;

    return 0;
}
