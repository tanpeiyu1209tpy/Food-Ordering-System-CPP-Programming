#ifndef PAYMENT_H_INCLUDED
#define PAYMENT_H_INCLUDED

//ЛгѕаАлЈ¬ calculate distance
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <stdexcept>  // for invalid_argument
#include <fstream> //including the file stream library
#include <sstream> //including the string stream library
#include <limits> //numeric_limits
#include <algorithm>
#include <set>
#include <map>
#include <cctype>
#include <cstdlib>
#include <random>
#include <unordered_set>
#include "customer.h"
#include "restaurant.h"

using namespace std;

map<pair<string, string>, double> readDistances() {
    map<pair<string, string>, double> distances;
    string filename = "distance.csv";
    ifstream file(filename);
    string line, userId, restaurantName;
    double distance;

    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, userId, ',');
        getline(ss, restaurantName, ',');
        ss >> distance;
        distances[{userId, restaurantName}] = distance;
    }

    file.close();
    return distances;
}

struct Rider{
    string name;
    string contact;
};

vector<Rider> riders = {
    {"Tan Pei Yu", "+1234524632455"},
    {"Miner Hans", "+1293456453212"},
    {"Aonse World", "+9876543456787"}
};

void delay(int seconds)  {
        clock_t end_time = clock() + seconds * CLOCKS_PER_SEC;
        while (clock() < end_time);
}

class Payment{
private:
    double totalFoodPrice;
    double totalPrice;
    double deliveryFee;
    double discountAmount;
    bool discountApply;
    vector<order> finalOrder;
    map<pair<string, string>, double> distances;
    unordered_set<string> usedIDs;

public:
    Payment() = default;
    Payment(double tFP, double tP, double dF, double dA, bool disA){
        totalFoodPrice = tFP;
        totalPrice = tP;
        deliveryFee = dF;
        discountAmount = dA;
        discountApply = disA;
    }

    void setFinalOrder(vector<order>& orders){
        finalOrder = orders;
    }
    void setDistances(const map<pair<string, string>, double>& d) {
        distances = d;
    }
    double getTotalPrice(){
        return totalPrice;
    }

    void applyDiscountCode(double& totalPrice, bool& discountApply){

        discountAmount = 0.0;

        vector<string> validCodes = {"FRENZY10", "FEEDER20", "FFFSHIPPINGRM5"};
        map<string, double> discountCodes = {
            {"FRENZY10", 10.0},
            {"FEEDER20", 20.0},
            {"FFFSHIPPINGRM5", 5.0}
        };
        discountApply = false;

        while (!discountApply) {
            try {
                cout << "Enter discount c0ode (if any) or press Enter to skip: ";
                string code;
                getline(cin, code);

                if (code.empty()) {
                    return; // No discount code entered
                }

                auto it = discountCodes.find(code);
                if (it != discountCodes.end()) {
                    double discountPercentage = it->second;
                    if(code == "FFFSHIPPINGRM5"){
                        discountAmount = discountPercentage;
                        totalPrice = totalPrice - discountAmount;
                        cout << "RM5 shipping discount applied" << endl;
                    }else{
                        discountAmount = totalPrice * (discountPercentage / 100.0);
                        cout << "Discount of " << discountPercentage << "% applied. You saved RM " << fixed << setprecision(2) << discountAmount << "!" << endl;
                        totalPrice = totalPrice - discountAmount;
                    }
                    discountApply = true;
                    break;
                }else {
                    throw invalid_argument("Invalid discount code. Please try again.");
                }
            } catch (const invalid_argument& e) {
                cout << e.what() << endl;
            }
        }
    }

    void calculateTotalPrice(const string& userId){
        totalPrice = 0.0;
        totalFoodPrice = 0.0;

        for(auto& o:finalOrder){
            totalFoodPrice += o.quantity*o.price;

            double distance = distances[{userId, o.restaurantName}];

            if (distance < 3.0) {
                deliveryFee = 5.0;// Direct
            } else if (distance >= 3.0 && distance < 8.0) {
                deliveryFee = 10.0; // Standard
            } else {
                deliveryFee = 15.0; // Saver
            }
            totalPrice = totalFoodPrice + deliveryFee;
            applyDiscountCode(totalPrice, discountApply);
            if (discountApply)
                break;
        }
    }

    void printOrderSummary()const{

        cout << "\n\nOrder Summary:\n" << endl;
        cout << "Restaurant: " << finalOrder[0].restaurantName << "\n";
        for(auto& o:finalOrder){
            cout << "    Food Name: " << o.foodName << "\n"
                  << "    Quantity: " << o.quantity << "\n"
                  << "    Selected Preference: " << o.selectedPreference << "\n"
                  << "    Special Instruction: " << o.specialI << "\n" << endl;
        }
        cout << "Subtotal: RM" << fixed << setprecision(2) << totalFoodPrice << endl;
        cout << "Delivery Fees: RM" << fixed << setprecision(2) << deliveryFee << endl;
        if(discountApply == true)
            cout << "\nDiscount: RM" << fixed << setprecision(2) << discountAmount << endl;
        cout << "\nTotal Price: RM " << fixed << setprecision(2) << totalPrice << endl;

    }

   void finalOrderAndTotalPrice(Customer* c, Restaurant* r){
       vector<order>orders = r->getOrders();

       setFinalOrder(orders);
       cout << "\n*Based on the distance between your location and the restaurant, our system automatically selects the most efficient delivery method for your order.*\n" << endl;
       calculateTotalPrice(c->getID());
       printOrderSummary();
   }

   string generateOrderID() {
       static random_device rd;
       static mt19937 gen(rd());
       uniform_int_distribution<int> dis(10000, 99999);

       string orderId;
       do {
           orderId = "weird" + to_string(dis(gen));
       } while (usedIDs.find(orderId) != usedIDs.end());

       // Add to used IDs set
       usedIDs.insert(orderId);

       return orderId;
   }

   void receipt(){

       int randomIndex = rand()%riders.size();
       Rider selectedRider = riders[randomIndex];

       string orderID = generateOrderID();
       cout << "---------------------------------------------------------------" << "\n"
             << "                            RECEIPT                           " << "\n"
             << "---------------------------------------------------------------" << "\n"
             << "                       FOOD FRENZY FEEDER                     " << "\n"
             << "       42 ½ Whimsical Way, Munchie Meadows, Foodlandia,       " << "\n"
             << "               Delightful District, Snacktopia                " << "\n"
             << "                CONTACT: +09122003 YUMMY-611                  " << "\n"
             << "===============================================================" << "\n"
             << "ORDER ID: " << orderID << "\n\n"
             << "_______________________________________________________________" << "\n"
             << "FOOD NAME                      QUANTITY                   PRICE" << "\n"
             << "_______________________________________________________________" << endl;

       for(auto& orderf:finalOrder){
           cout << left << setw(30) << orderf.foodName
                 << right << setw(5) << orderf.quantity
                 << right << setw(27) << fixed << setprecision(2) << orderf.price*orderf.quantity << endl;
           cout << "*** [" << orderf.selectedPreference << "] "<<orderf.specialI << endl;
           cout << "\n" << endl;
       }
       cout << "SUBTOTAL: RM " << fixed << setprecision(2) << totalFoodPrice << endl;
       cout << "DELIVERY: RM " << fixed << setprecision(2) << deliveryFee << endl;
       if(discountApply == true)
            cout << "\nDISCOUNT: RM" << fixed << setprecision(2) << discountAmount << endl;
       cout << "\nTOTAL: RM " << fixed << setprecision(2) << totalPrice << endl;
       cout << "=============================================================="<< endl;
       cout << "RIDER: " << selectedRider.name << endl;
       cout << "CONTACT: " << selectedRider.contact << endl;
       cout << "==============================================================" << endl;
       cout << "Thanks for your payment :D" << endl;
       cout << "We hope you enjoy meal and have a wonderful experience with us!" << endl;
       cout << "---------------------------------------------------------------" << endl;
   }

   void paymentF(Customer* c, Restaurant* r){
       finalOrderAndTotalPrice(c, r);

       cout << "\nPlease choose the payment methods: " << "\n"
             << "1. Touch N Go" << "\n"
             << "2. Credit Card" << "\n"
             << "3. Cash On Delivery" << endl;
       string paymentChoice;

       do{
           cout << "->";
           getline(cin, paymentChoice);
           if(paymentChoice == "1"){
               cout << "\nPlease enter P for confirm the payment\n-> ";
               string tngpay;
               getline(cin, tngpay);
               cout << "Processing..." << endl;
               delay(2);
               if(tngpay == "p" || tngpay == "P"){
                   receipt();
                   r->clearOrders();
                   finalOrder.clear();
                   exit(0);
               }
               else
                   cout << "Payment uncessful." << endl;
                break;
           }
           else if(paymentChoice == "2"){
               cout << "Please enter your credit/debit card details: " << endl;

               string cardNumber;
               cout << "Card Number -> ";
               getline(cin, cardNumber);
               string validDate;
               cout << "Expiry Date (MMYY) -> ";
               getline(cin, validDate);
               string cvv2;
               cout << "CVC/CVV2 -> ";
               getline(cin, cvv2);

               bool isValid = true;

               if (cardNumber.length() != 16 || !all_of(cardNumber.begin(), cardNumber.end(), ::isdigit)) {
                   isValid = false;
                }

                if (validDate.length() != 4 || !all_of(validDate.begin(), validDate.end(), ::isdigit)) {
                    isValid = false;
                }
                if (cvv2.length() != 3 || !all_of(cvv2.begin(), cvv2.end(), ::isdigit)) {
                    isValid = false;
                }

               cout << "Processing..." << endl;
               delay(2);

               if (isValid) {
                    receipt();
                    r->clearOrders();
                    exit(0);

               }else
                   cout << "Payment uncessful." << endl;

               break;
           }
           else if(paymentChoice == "3"){
               cout << "Thanks for your order. Please have the exact amount ready when your riders arrives.\n" << endl;
               delay(2);
               receipt();
               r->clearOrders();
               finalOrder.clear();
               exit(0);
           }
           else
               cout << "Invalid choice. Re-enter again.\n" << endl;
       }while(paymentChoice!="1"&&paymentChoice!="2"&&paymentChoice!="3");
   }

   void menu(Customer* c, Restaurant* r,vector<RestaurantBase*>& restaurants){
        int foodChoice;
        do{
            cout << "\nNow you can choose to order the food" << endl;
            cout << "1. Order" << "\n"
                  << "2. View Cart" << "\n"
                  << "3. Checkout" << "\n"
                  << "4. View Historical Order" << "\n"
                  << "5. Exit" << endl;

            cout << "-> ";

            try{
                cin >> foodChoice;
                if(cin.fail() || foodChoice<1 || foodChoice>5)
                    throw invalid_argument("Invalid choice. Please try again");
            }catch (const invalid_argument& e){
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(),'\n');
                cout << e.what() << endl;
                continue;
            }
            cin.ignore();

            switch(foodChoice){
            case 1:
                //viewFoodMenu(restaurants);
                r->filterMenu(restaurants);
                break;

            case 2:
                r->viewCart();
                break;

            case 3:
                r->checkOut();
                paymentF(c, r);
                break;

            case 4:
                r->reviewPastOrders(c->getID());
                break;

            case 5:
                break;
            }

        }while(foodChoice != 5);
    }
};

#endif // PAYMENT_H_INCLUDED
