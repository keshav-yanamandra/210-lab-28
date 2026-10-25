// Keshav Yanamandra
// COMSC-210-5293, Fall 2026
// Lab 28


#include <iostream>
#include <fstream>
#include <iomanip>
#include <list>
#include <algorithm>
#include <numeric>


#include "Goat.h"
using namespace std;

const int SZ_NAMES = 200, SZ_COLORS = 25;

int select_goat(list<Goat> trip);
void delete_goat(list<Goat> &trip);
void add_goat(list<Goat> &trip, string [], string []);
void display_trip(list<Goat> trip);
int main_menu();

void list_with_for_each(list<Goat> trip);
bool older_than_limit(const Goat &g);
bool name_matches(const Goat &g);
bool at_max_age(const Goat &g);
bool by_age(const Goat &a, const Goat &b);
int add_age(int sum, const Goat &g);



int ageLimit;
string searchName;


int main() {
    srand(time(0));

    // read & populate arrays for names and colors
    ifstream fin("names.txt");
    string names[SZ_NAMES];
    int i = 0;
    while (fin >> names[i++]);
    fin.close();
    ifstream fin1("colors.txt");
    string colors[SZ_COLORS];
    i = 0;
    while (fin1 >> colors[i++]);
    fin1.close();

    // create & populate a trip of Goats using std::list of random size 8-15
    int tripSize = rand() % 8 + 8;
    list<Goat> trip;
    int age;
    string name, color;
    for (int i = 0; i < tripSize; i++) {
        age = rand() % MAX_AGE;  // defined in Goat.h
        name = names[rand() % SZ_NAMES];
        color = colors[rand() % SZ_COLORS];
        Goat tmp(name, age, color);
        trip.push_back(tmp);
    }
    
    // Goat Manager 3001 Engine
    int sel = main_menu();
    while (sel != 10) {
        switch (sel) {
            case 1:
                cout << "Adding a goat.\n";
                add_goat(trip, names, colors);
                break;
            case 2:    
                cout << "Removing a goat.\n";
                delete_goat(trip);
                break;
            case 3:    
                cout << "Displaying goat data.\n";
                display_trip(trip);
                break;
            //new case for each
            case 4:
                cout << "Listing goats with for_each.\n";
                list_with_for_each(trip);
                break;

            //milestone 2
            case 5:
                cout << "Enter age: ";
                cin >> ageLimit;
                cout << count_if(trip.begin(), trip.end(), older_than_limit) << " goats are older than " << ageLimit << ".\n";
                break;

            //milestone 3
            case 6: {
                cout << "Enter goat name: ";
                cin >> searchName;

                list<Goat>::iterator it =
                    find_if(trip.begin(), trip.end(), name_matches);

                if (it != trip.end()) {
                    cout << "Found: " << it->get_name() << " (" << it->get_age() << ", " << it->get_color() << ")\n";
                }
                else {
                    cout << searchName << " not found.\n";
                }

                break;
            }

            //milestone 4
            case 7:
                if (any_of(trip.begin(), trip.end(), at_max_age)) {
                    cout << "There is a goat at maximum age.\n";
                }
                else {
                    cout << "There are no goats at maximum age.\n";
                }
                break;

            //milestone 5
            case 8: {
                if (!trip.empty()) {
                    list<Goat>::iterator oldest = max_element(trip.begin(), trip.end(), by_age);
                    list<Goat>::iterator youngest = min_element(trip.begin(), trip.end(), by_age);

                    cout << "Oldest goat: " << oldest->get_name() << " (" << oldest->get_age() << ", " << oldest->get_color() << ")\n";
                    cout << "Youngest goat: "<< youngest->get_name() << " ("<< youngest->get_age() << ", "<< youngest->get_color() << ")\n";
                }
                else {
                    cout << "no goats.\n";
                }

                break;
            }

            //milestone 6
            case 9: {
                int totalAge = accumulate(trip.begin(), trip.end(), 0, add_age);

                cout << "Total age: " << totalAge << "\n";

                if (!trip.empty()) {
                    double averageAge = (double)totalAge / trip.size();
                    cout << "Average age: " << averageAge;
                    cout << endl;
                }

                break;
            }

            default:
                cout << "Invalid selection.\n";
                break;
        }

        cout << endl;
        sel = main_menu();
    }

    return 0;
}

int main_menu() {
    cout << "*** GOAT MANAGER 3001 ***\n";
    cout << "[1] Add a goat\n";
    cout << "[2] Delete a goat\n";
    cout << "[3] List goats\n";
    cout << "[4] List goats with for_each\n";
    cout << "[5] Count goats older than an age\n";
    cout << "[6] Search for a goat by name\n";
    cout << "[7] Check for goat at maximum generated age\n";
    cout << "[8] Find oldest and youngest goat\n";
    cout << "[9] Total and average goat age\n";
    cout << "[10] Quit\n";    cout << endl;
    
    cout << "Choice --> ";
    int choice;
    cin >> choice;
    while (choice < 1 || choice > 10) {
        cout << "Invalid, again --> ";
        cin >> choice;
    }
    return choice;
}

void delete_goat(list<Goat> &trip) {
    cout << "DELETE A GOAT\n";
    int index = select_goat(trip);
    auto it = trip.begin();
    advance(it, index-1);
    trip.erase(it);
    cout << "Goat deleted. New trip size: " << trip.size() << endl;
}

void add_goat(list<Goat> &trip, string nms[], string cls[]) {
    cout << "ADD A GOAT\n";
    int age = rand() % MAX_AGE;
    string nm = nms[rand() % SZ_NAMES];
    string cl = cls[rand() % SZ_COLORS];
    Goat tmp(nm, age, cl);
    trip.push_back(tmp);
    cout << "Goat added. New trip size: " << trip.size() << endl;
}

void display_trip(list<Goat> trp) {
    int i = 1;
    for (auto gt: trp)
        cout << "\t" 
             << "[" << i++ << "] "
             << gt.get_name() 
             << " (" << gt.get_age() 
             << ", " << gt.get_color() << ")\n";
}

int select_goat(list<Goat> trp) {
    int input;
    cout << "Make a selection:\n";
    display_trip(trp);
    cout << "Choice --> ";
    cin >> input;
    while (input < 1 or input > (int)trp.size()) {
        cout << "Invalid choice, again --> ";
        cin >> input;
    }
    return input;
}

void list_with_for_each(list<Goat> trip) {
    for_each(trip.begin(), trip.end(), [](const Goat &g) {
        cout << g.get_name() << " (" << g.get_age() << ", " << g.get_color() << ")" << endl;
    });
}

bool older_than_limit(const Goat &g) {
    return g.get_age() > ageLimit;
}

bool name_matches(const Goat &g) {
    return g.get_name() == searchName;
}

bool at_max_age(const Goat &g) {
    return g.get_age() >= MAX_AGE - 1;
}

bool by_age(const Goat &a, const Goat &b) {
    return a.get_age() < b.get_age();
}

int add_age(int sum, const Goat &g) {
    return sum + g.get_age();
}