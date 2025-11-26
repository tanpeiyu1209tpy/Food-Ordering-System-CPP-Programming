#ifndef RESTAURANT_H_INCLUDED
#define RESTAURANT_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>  // for invalid_argument
#include <fstream> //including the file stream library
#include <sstream> //including the string stream library
#include <limits> //numeric_limits
#include <algorithm>
#include <set>
#include "customer.h"

using namespace std;

struct menuItem{
    string rFoodID;
    string rFoodType;
    string rFoodName;
    double rFoodPrice;
    string rFoodPreference;
    vector<string> preferenceList;

    menuItem(string rID, string rFT, string rFN, double rFP, string rP){
        rFoodID = rID;
        rFoodType = rFT;
        rFoodName = rFN;
        rFoodPrice = rFP;
        rFoodPreference = rP;

        if (!rP.empty()) {
            istringstream ss(rP);
            string preference;
            while (getline(ss, preference, ',')) {
                preferenceList.push_back(preference);
            }
        }
    }
};

struct order{
    string restaurantName;
    string foodID;
    string foodName;
    int quantity;
    string specialI;
    double price;
    string selectedPreference;


    order(string rName, string fID, string fN, int q, string si, double p, string sp){
        restaurantName = rName;
        foodID = fID;
        foodName = fN;
        quantity = q;
        specialI = si;
        price = p;
        selectedPreference = sp;
    }
};

class exceptionHanddling:public exception{
public:
    const char* what() const noexcept override{
        return "Invalid choice. Please enter again";
    }
};


class RestaurantBase{
protected:
    vector<menuItem>menuItems;
    string rCategory;
    string rName;
    vector<order> orders;
    string currentOrderRestaurantName;
    string specialInstruction;

public:
    RestaurantBase() = default;
    RestaurantBase(string rC, string rN){
        rCategory = rC;
        rName = rN;
    }

    void setRCategory(string rC){
        rCategory = rC;
    }
    string getRCategory(){
        return rCategory;
    }
    void setRName(string rN){
        rName = rN;
    }
    string getRName(){
        return rName;
    }
    void addMenuItem(menuItem& item) {
        menuItems.push_back(item);
    }
    vector<menuItem>& getMenuItems(){
        return menuItems;
    }


    static void readCSV(vector<RestaurantBase*>& restaurants) {
        string filename = "foodMenu.csv";
        ifstream file(filename);
        string line;

        if (!file.is_open()) {
            cerr << "Failed to open the file." << endl;
            return;
        }

        while (getline(file, line)) {
            istringstream iss(line);
            string rC, rN, rID, rFT, rFN, rFP, rP;

            getline(iss, rC, ',');
            getline(iss, rN, ',');
            getline(iss, rID, ',');
            getline(iss, rFT, ',');
            getline(iss, rFN, ',');
            getline(iss, rFP, ',');
            getline(iss, rP, ',');

            if (rP.front() == '"') {
                string temp;
                while (rP.back() != '"' && getline(iss, temp, ',')) {
                    rP += "," + temp;
                }
                rP = rP.substr(1, rP.length() - 2);
            }

            double price = stod(rFP);

            auto it = find_if(restaurants.begin(), restaurants.end(),
                [&](RestaurantBase* restaurant) {
                    return restaurant->getRCategory() == rC && restaurant->getRName() == rN;
                });

            if (it != restaurants.end()) {
                menuItem itemToAdd(rID, rFT, rFN, price, rP);
                (*it)->addMenuItem(itemToAdd);
            } else {
                RestaurantBase* newRestaurant = new RestaurantBase(rC, rN);
                menuItem itemToAdd(rID, rFT, rFN, price, rP);
                newRestaurant->addMenuItem(itemToAdd);
                restaurants.push_back(newRestaurant);
            }
        }

        file.close();
    }

    void viewFoodMenu(vector<RestaurantBase>& restaurants){
        for(auto& restaurant : restaurants){
            cout << "------------------------------------------" << endl;
            cout << "Cuisine: " << restaurant.getRCategory() << endl;
            cout << "Restaurant Name: " << restaurant.getRName() << endl;
            cout << "------------------------------------------" << endl;

            auto& menuItems = restaurant.getMenuItems();
            for (auto& item : menuItems) {
                cout << "Food ID: " << item.rFoodID << endl;
                cout << "Food Type: " << item.rFoodType << endl;
                cout << "Food Name: " << item.rFoodName << endl;
                cout << "Food Price: " << item.rFoodPrice << endl;
                cout << "Food Preference: " << item.rFoodPreference << endl;
                cout << "\n" << endl;
            }

            cout << "\n" << endl;
        }
    }
};

class Restaurant:public RestaurantBase{
public:
    Restaurant() = default;
    Restaurant(string rC, string rN, string cORN, string sI){
        rCategory = rC;
        rName = rN;
        currentOrderRestaurantName = cORN;
        specialInstruction = sI;
    }

    void clearOrders(){
        orders.clear();
    }
    vector<order>& getOrders(){
        return orders;
    }
    void setCurrentOrderRN(string cORN){
        currentOrderRestaurantName = cORN;
    }
    string getCurrentOrderRN(){
        return currentOrderRestaurantName;
    }
    void setSpecialInstruction(string sI){
        specialInstruction = sI;
    }
    string getSpecialInstruction(){
        return specialInstruction;
    }

    string getFoodNameByID(RestaurantBase* restaurant, string& foodID){
        for(auto& item : restaurant->getMenuItems()){
            if(item.rFoodID == foodID){
                return item.rFoodName;
            }
        }
        return "";
    }

    double getFoodPriceByID(RestaurantBase* restaurant, string& foodID){
        for(auto& item : restaurant->getMenuItems()){
            if(item.rFoodID == foodID){
                return item.rFoodPrice;
            }
        }
        return 0.0;
    }

    vector<string> getPreferencesByID(RestaurantBase* restaurant, string& foodID) {
        for (auto& item : restaurant->getMenuItems()) {
            if (item.rFoodID == foodID) {
                return item.preferenceList;
            }
        }
        return {};
    }


    void viewCart(){
        if(orders.empty()){
            cout << "Your cart is empty" << endl;
            return;
        }
        cout << "Your current cart: " << endl;
        cout << "\nRestaurant: " << orders[0].restaurantName << endl;

        for(auto& order: orders){
            cout << "   " << order.foodName << "   RM " << order.price << "   Qty: "<< order.quantity << endl;
            cout << "   ***[Preference: " << order.selectedPreference << "]  " << order.specialI << "\n" << endl;
        }
    }

    void clearCart(){
        orders.clear();
        currentOrderRestaurantName.clear();
        cout << "Your cart has been cleared." << endl;
    }

    void addOrder(const string& rName, string& fID, string& fN, int q, string& si, double p, string sp){
        if(currentOrderRestaurantName.empty()){
            currentOrderRestaurantName = rName;
        }

        if(rName != currentOrderRestaurantName){
            cout << "\nYou currently have items from another restaurant in your cart." << endl;
            viewCart();
            cout << "\nWould you like to clear your cart and order from " << rName << "? (y/n) \n=>";
            string choiceClear;
            cin >> choiceClear;

            try{
                if(choiceClear == "y" || choiceClear == "Y" || choiceClear == "Yes"){
                    clearCart();
                    currentOrderRestaurantName = rName;
                }else if(choiceClear == "n" || choiceClear == "N" || choiceClear == "No"){
                    return;
                }else{
                    throw exceptionHanddling();
                }
            }catch (const exceptionHanddling& e){
                cout << e.what() << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        orders.emplace_back(rName, fID, fN, q, si, p, sp);
        cout << "Order added!\n" << endl;
    }

    void checkOut(){
        if (orders.empty()) {
            cout << "Your cart is empty. Please add items to the cart before checking out." << endl;
        } else {
            cout << "Checking out..." << endl;
            //orders.clear();
        }
    }

    void displayFilteredRestaurants(const vector<RestaurantBase*>& filteredRestaurants) {
        cout << "\nRestaurants in selected category:" << endl;
        int index = 1;
        for (const auto& restaurant : filteredRestaurants) {
            cout << index << ". " << restaurant->getRName() << endl;
            index++;
        }
    }

    void displayMenuItems(RestaurantBase* restaurant) {
        cout << "\nMenu items in " << restaurant->getRName() << ":" << endl;
        for (const auto& item : restaurant->getMenuItems()) {
            cout << "\n===============================================================" << endl;
            cout << "Food ID: " << item.rFoodID << "\n         " << item.rFoodName << " - RM " << item.rFoodPrice << endl;
            if (!item.preferenceList.empty()) {
                cout << "\n***Preferences: ";
                for (const auto& preference : item.preferenceList) {
                    cout << preference << ", ";
                }
                cout << "\b\b " << endl;  // Remove last comma and space
            }
        }
    }

    void handleFoodSelection(vector<RestaurantBase*>& filteredRestaurants) {
        cout << "\nSelect a restaurant by number (or type 'b' to go back): ";
        string choice;
        cin >> choice;
        if (choice == "b") {
            return;
        }

        try {
            int restaurantIndex = stoi(choice);
            if (restaurantIndex < 1 || restaurantIndex > filteredRestaurants.size()) {
                throw invalid_argument("Invalid choice.");
            }

            RestaurantBase* selectedRestaurant = filteredRestaurants[restaurantIndex - 1];
            displayMenuItems(selectedRestaurant);

            cout << "===============================================================\n" << endl;
            cout << "\nEnter Food ID to order (or type 'b' to go back): ";
            cin >> choice;
            if (choice == "b") {
                return;
            }

            string foodID = choice;
            string foodName = getFoodNameByID(selectedRestaurant, foodID);
            double foodPrice = getFoodPriceByID(selectedRestaurant, foodID);
            vector<string> preferences = getPreferencesByID(selectedRestaurant, foodID);

            if (foodName.empty()) {
                cout << "Invalid Food ID. Please try again." << endl;
                return;
            }

            int quantity;

            while (true) {
                cout << "Enter quantity: ";
                cin >> quantity;
                if (cin.fail()) {
                    cin.clear(); // clear error flags
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard invalid input
                    try {
                        throw exceptionHanddling();
                    } catch (const exception& e) {
                        cout << e.what() << endl;
                    }
                }
                else
                    break;
            }

            cout << "Enter special instructions: ";
            string specialInstruction;
            cin.ignore();
            getline(cin, specialInstruction);

            string selectedPreference;
            if (!preferences.empty()) {
                cout << "Available preferences: " << endl;
                for (size_t i = 0; i < preferences.size(); ++i) {
                    cout << i + 1 << ". " << preferences[i] << endl;
                }

                cout << "Select a preference by number (or type '0' for none): ";
                getline(cin, choice);

                try {
                    int preferenceIndex = stoi(choice);
                    if (preferenceIndex < 0 || preferenceIndex > preferences.size()) {
                        throw invalid_argument("Invalid choice.");
                    }

                    if (preferenceIndex == 0) {
                        selectedPreference = "None";
                    } else {
                        selectedPreference = preferences[preferenceIndex - 1];
                    }
                } catch (const invalid_argument& e) {
                    cout << "Invalid input. Please try again." << endl;
                    return;
                }
            } else {
                selectedPreference = "None";
            }

            addOrder(selectedRestaurant->getRName(), foodID, foodName, quantity, specialInstruction, foodPrice, selectedPreference);
        } catch (const invalid_argument& e) {
            cout << "Invalid input. Please try again." << endl;
        }
    }

    void filterMenu(vector<RestaurantBase*>& restaurants){

        while (true) {
            cout << "\nSelect a category (or type 'b' to exit):" << endl;
            set<string> categories;
            for (auto& restaurant : restaurants) {
                categories.insert(restaurant->getRCategory());
            }
            int index = 1;
            for (auto& category : categories) {
                cout << index++ << ". " << category << endl;
            }

            string choice;
            cout << "->";
            getline(cin, choice);
            if (choice == "b" || choice == "B") {
                break;
            }

            try{
                int choiceIndex = stoi(choice);
                if (choiceIndex < 1 || choiceIndex > categories.size()) {
                    throw invalid_argument("Invalid choice.");
                }

                auto it = categories.begin();
                advance(it, choiceIndex - 1);
                string selectedCategory = *it;

                vector<RestaurantBase*> filteredRestaurants;
                for (auto& restaurant : restaurants) {
                    if (restaurant->getRCategory() == selectedCategory) {
                        filteredRestaurants.push_back(restaurant);
                    }
                }

                displayFilteredRestaurants(filteredRestaurants);
                handleFoodSelection(filteredRestaurants);
            } catch (const invalid_argument& e) {
                cout << "Invalid input. Please try again." << endl;
            }
        }
    }

    void reviewPastOrders(const string& GETID) {
        string fileName = "historicalOrder.csv";
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Failed to open the past orders file." << endl;
            return;
        }

        string line;
        vector<order> pastOrders;
        while (getline(file, line)) {
            istringstream iss(line);
            string userID, category, restaurantName, foodID, foodType, foodName, foodPriceStr, foodPreference, foodQuantityStr, specialInstruction;
            getline(iss, userID, ',');
            getline(iss, category, ',');
            getline(iss, restaurantName, ',');
            getline(iss, foodID, ',');
            getline(iss, foodType, ',');
            getline(iss, foodName, ',');
            getline(iss, foodPriceStr, ',');
            getline(iss, foodPreference, ',');
            getline(iss, foodQuantityStr, ',');
            getline(iss, specialInstruction, ',');

            double foodPrice = stod(foodPriceStr);
            int foodQuantity = stoi(foodQuantityStr);

            if (userID == GETID) {
                pastOrders.emplace_back(restaurantName, foodID, foodName, foodQuantity, specialInstruction, foodPrice, foodPreference);
            }
        }

        if (pastOrders.empty()) {
            cout << "No historical orders found." << endl;
            return;
        }

        cout << "Historical orders:" << endl;
        for (size_t i = 0; i < pastOrders.size(); ++i) {
            auto& order = pastOrders[i];
            cout <<"\n" << i + 1 << ". Restaurant: " << order.restaurantName << "\n      Food Name: " << order.foodName
                 << "\n      Quantity: " << order.quantity << "\n      Price: RM" << order.price
                 << "\n      Preference: " << order.selectedPreference << "\n      Special Instruction: " << order.specialI << endl;
        }

        cout << "\nEnter the number of the order you want to reorder (or type 'b' to go back): ";
        string reorderChoice;
        getline(cin, reorderChoice);
        if (reorderChoice == "b" || reorderChoice == "B") {
            return;
        }

        int reorderIndex = stoi(reorderChoice);
        if (reorderIndex < 1 || reorderIndex > pastOrders.size()) {
            cout << "Invalid choice. Please try again." << endl;
            return;
        }

        auto& selectedOrder = pastOrders[reorderIndex - 1];
        addOrder(selectedOrder.restaurantName, selectedOrder.foodID, selectedOrder.foodName,
                 selectedOrder.quantity, selectedOrder.specialI, selectedOrder.price, selectedOrder.selectedPreference);
        cout << "Order added to cart!" << endl;
    }
};

#endif // RESTAURANT_H_INCLUDED
