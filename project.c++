
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <limits>
#include <cctype>

using namespace std;

const int MAX = 200;

// Structure to store item details
struct Item {
    int id;
    string studentName;
    string studentID;
    string itemName;
    string category;
    string description;
    string location;
    string date;
    string type;
    string status;
};

Item items[MAX];
int countItems = 0;
int nextID = 1;

// Convert string into lowercase
string toLowerCase(string str) {
    for (int i = 0; i < (int)str.length(); i++) {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
    return str;
}

// Load records from file
void loadRecords() {

    ifstream file("items.txt");

    if (!file.is_open()) {
        return;
    }

    while (countItems < MAX) {

        Item temp;

        if (!(file >> temp.id)) {
            break;
        }

        file.ignore(numeric_limits<streamsize>::max(), '\n');

        if (!getline(file, temp.studentName) ||
            !getline(file, temp.studentID) ||
            !getline(file, temp.itemName) ||
            !getline(file, temp.category) ||
            !getline(file, temp.description) ||
            !getline(file, temp.location) ||
            !getline(file, temp.date) ||
            !getline(file, temp.type) ||
            !getline(file, temp.status)) {
            break;
        }

        items[countItems] = temp;

        if (temp.id >= nextID) {
            nextID = temp.id + 1;
        }

        countItems++;
    }

    file.close();
}

// Save records into file
void saveRecords() {

    ofstream file("items.txt");

    if (!file.is_open()) {
        cout << "\nError saving records!\n";
        return;
    }

    for (int i = 0; i < countItems; i++) {

        file << items[i].id << endl;
        file << items[i].studentName << endl;
        file << items[i].studentID << endl;
        file << items[i].itemName << endl;
        file << items[i].category << endl;
        file << items[i].description << endl;
        file << items[i].location << endl;
        file << items[i].date << endl;
        file << items[i].type << endl;
        file << items[i].status << endl;
    }

    file.close();
}

// Get valid integer input
int getChoice() {

    int choice;

    while (!(cin >> choice)) {

        cout << "Enter a valid number: ";

        cin.clear();

        cin.ignore(
            numeric_limits<streamsize>::max(), '\n'
        );
    }

    cin.ignore(
        numeric_limits<streamsize>::max(), '\n'
    );

    return choice;
}

// Display item details
void displayItem(const Item &item) {

    cout << "\n====================================";
    cout << "\nRecord ID: " << item.id;
    cout << "\nStudent Name: " << item.studentName;
    cout << "\nStudent ID: " << item.studentID;
    cout << "\nItem Name: " << item.itemName;
    cout << "\nCategory: " << item.category;
    cout << "\nDescription: " << item.description;
    cout << "\nLocation: " << item.location;
    cout << "\nDate: " << item.date;
    cout << "\nType: " << item.type;
    cout << "\nStatus: " << item.status;
    cout << "\n====================================\n";
}

// Student Login
bool studentLogin(string &name, string &studentID) {

    cout << "\n========== STUDENT LOGIN ==========\n";

    cout << "Enter Student Name: ";
    getline(cin, name);

    cout << "Enter Student ID: ";
    getline(cin, studentID);

    if (name.empty() || studentID.empty()) {

        cout << "\nLogin details cannot be empty!\n";
        return false;
    }

    cout << "\nLogin Successful!";
    cout << "\nWelcome " << name << "!\n";

    return true;
}

// Report Lost or Found Item
void reportItem(
    const string &name,
    const string &studentID,
    const string &type
) {

    if (countItems >= MAX) {

        cout << "\nStorage is full!\n";
        return;
    }

    Item newItem;

    newItem.id = nextID++;
    newItem.studentName = name;
    newItem.studentID = studentID;
    newItem.type = type;
    newItem.status = "OPEN";

    cout << "\nEnter Item Name: ";
    getline(cin, newItem.itemName);

    cout << "Enter Category: ";
    getline(cin, newItem.category);

    cout << "Enter Description: ";
    getline(cin, newItem.description);

    cout << "Enter Location: ";
    getline(cin, newItem.location);

    cout << "Enter Date (DD-MM-YYYY): ";
    getline(cin, newItem.date);

    if (newItem.itemName.empty() ||
        newItem.category.empty()) {

        cout << "\nItem Name and Category are required!\n";
        nextID--;
        return;
    }

    items[countItems] = newItem;

    countItems++;

    saveRecords();

    cout << "\nItem Report Submitted Successfully!";
    cout << "\nYour Record ID is: " << newItem.id << "\n";
}

// Search Item
void searchItem() {

    string searchName;
    bool found = false;

    cout << "\nEnter Item Name to Search: ";
    getline(cin, searchName);

    if (searchName.empty()) {
        cout << "\nSearch name cannot be empty!\n";
        return;
    }

    for (int i = 0; i < countItems; i++) {

        string a = toLowerCase(items[i].itemName);
        string b = toLowerCase(searchName);

        if (a.find(b) != string::npos) {

            displayItem(items[i]);

            found = true;
        }
    }

    if (!found) {
        cout << "\nNo Matching Item Found!\n";
    }
}

// Automatically match Lost and Found items
void autoMatch() {

    bool matched = false;

    for (int i = 0; i < countItems; i++) {

        if (items[i].type != "LOST" ||
            items[i].status == "MATCHED") {
            continue;
        }

        for (int j = 0; j < countItems; j++) {

            if (items[j].type != "FOUND" ||
                items[j].status == "MATCHED") {
                continue;
            }

            string a = toLowerCase(items[i].itemName);
            string b = toLowerCase(items[j].itemName);

            string c = toLowerCase(items[i].category);
            string d = toLowerCase(items[j].category);

            if (a == b && c == d) {

                cout << "\n******** MATCH FOUND ********\n";

                cout << "Lost Record ID: "
                     << items[i].id << endl;

                cout << "Found Record ID: "
                     << items[j].id << endl;

                cout << "Item Name: "
                     << items[i].itemName << endl;

                cout << "Category: "
                     << items[i].category << endl;

                items[i].status = "MATCHED";
                items[j].status = "MATCHED";

                matched = true;

                break;
            }
        }
    }

    if (!matched) {
        cout << "\nNo Matching Records Found!\n";
    }

    saveRecords();
}

// View All Records and sort by item name
void viewRecords() {

    if (countItems == 0) {

        cout << "\nNo Records Available!\n";
        return;
    }

    // Bubble Sort
    for (int i = 0; i < countItems - 1; i++) {

        for (int j = 0; j < countItems - i - 1; j++) {

            if (toLowerCase(items[j].itemName) >
                toLowerCase(items[j + 1].itemName)) {

                swap(items[j], items[j + 1]);
            }
        }
    }

    cout << "\n========== ALL ITEM RECORDS ==========\n";

    for (int i = 0; i < countItems; i++) {

        displayItem(items[i]);
    }
}

// Delete Record
void deleteRecord() {

    if (countItems == 0) {
        cout << "\nNo Records Available!\n";
        return;
    }

    int id;
    bool found = false;

    cout << "\nEnter Record ID to Delete: ";

    id = getChoice();

    for (int i = 0; i < countItems; i++) {

        if (items[i].id == id) {

            for (int j = i; j < countItems - 1; j++) {

                items[j] = items[j + 1];
            }

            countItems--;

            saveRecords();

            cout << "\nRecord Deleted Successfully!\n";

            found = true;

            break;
        }
    }

    if (!found) {
        cout << "\nRecord Not Found!\n";
    }
}

// Item Statistics
void itemStatistics() {

    int lost = 0;
    int found = 0;
    int matched = 0;

    for (int i = 0; i < countItems; i++) {

        if (items[i].type == "LOST") {
            lost++;
        }

        if (items[i].type == "FOUND") {
            found++;
        }

        if (items[i].status == "MATCHED") {
            matched++;
        }
    }

    cout << "\n========== ITEM STATISTICS ==========\n";

    cout << "Total Records : " << countItems << endl;
    cout << "Lost Items    : " << lost << endl;
    cout << "Found Items   : " << found << endl;
    cout << "Matched Items : " << matched << endl;
    cout << "Open Records  : " << countItems - matched << endl;
}

// Export Report
void exportReport() {

    ofstream file("LostFound_Report.txt");

    if (!file.is_open()) {

        cout << "\nUnable to Create Report File!\n";
        return;
    }

    file << "SMART CAMPUS LOST & FOUND REPORT\n";

    file << "====================================\n";

    file << "Total Records: " << countItems << "\n\n";

    for (int i = 0; i < countItems; i++) {

        file << "Record ID: " << items[i].id << endl;

        file << "Student Name: "
             << items[i].studentName << endl;

        file << "Student ID: "
             << items[i].studentID << endl;

        file << "Item Name: "
             << items[i].itemName << endl;

        file << "Category: "
             << items[i].category << endl;

        file << "Description: "
             << items[i].description << endl;

        file << "Location: "
             << items[i].location << endl;

        file << "Date: "
             << items[i].date << endl;

        file << "Type: "
             << items[i].type << endl;

        file << "Status: "
             << items[i].status << endl;

        file << "------------------------------------\n";
    }

    file.close();

    cout << "\nReport Exported Successfully!";
    cout << "\nFile Name: LostFound_Report.txt\n";
}

// Main Function
int main() {

    string studentName;
    string studentID;

    loadRecords();

    cout << "\n====================================\n";
    cout << " SMART CAMPUS LOST & FOUND SYSTEM\n";
    cout << "          COER UNIVERSITY\n";
    cout << "====================================\n";

    if (!studentLogin(studentName, studentID)) {
        return 0;
    }

    int choice;

    do {

        cout << "\n========== MAIN MENU ==========\n";

        cout << "1. Student Login\n";
        cout << "2. Report Lost Item\n";
        cout << "3. Report Found Item\n";
        cout << "4. Search Item\n";
        cout << "5. Auto Match Lost & Found\n";
        cout << "6. View All Records\n";
        cout << "7. Delete Record\n";
        cout << "8. Item Statistics\n";
        cout << "9. Export Report\n";
        cout << "10. Exit\n";

        cout << "\nEnter Your Choice: ";

        choice = getChoice();

        switch (choice) {

            case 1:

                studentLogin(studentName, studentID);

                break;

            case 2:

                reportItem(
                    studentName,
                    studentID,
                    "LOST"
                );

                break;

            case 3:

                reportItem(
                    studentName,
                    studentID,
                    "FOUND"
                );

                break;

            case 4:

                searchItem();

                break;

            case 5:

                autoMatch();

                break;

            case 6:

                viewRecords();

                break;

            case 7:

                deleteRecord();

                break;

            case 8:

                itemStatistics();

                break;

            case 9:

                exportReport();

                break;

            case 10:

                cout << "\nThank You for Using Our System!\n";
                cout << "Program Exited Successfully.\n";

                break;

            default:

                cout << "\nInvalid Choice! Try Again.\n";
        }

    } while (choice != 10);

    return 0;
}
