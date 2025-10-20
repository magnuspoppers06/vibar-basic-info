#include <iostream>
#include <iomanip>
#include <string>
#include <algorithm>
#include <limits>
#include <ctime>
#include <sstream>

using namespace std;


const double STANDARD_RATE = 3000.0;
const double DELUXE_RATE = 5000.0;
const double FAMILY_SUITE_RATE = 8000.0;
const double EXTRA_GUEST_RATE = 500.0;


struct Date {
    int year, month, day;
    Date() : year(0), month(0), day(0) {}
    string toString() const {
        const string months[] = {"", "January", "February", "March", "April", "May", "June",
                                 "July", "August", "September", "October", "November", "December"};
        stringstream ss;
        ss << months[month] << " " << day << ", " << year;
        return ss.str();
    }
};


struct Reservation {
    string customerName;
    Date checkInDate;
    string roomType;
    int numNights, numGuests;
    double totalCost, payment, change;
    Reservation() : customerName(""), roomType(""), numNights(0), numGuests(0),
                    totalCost(0.0), payment(0.0), change(0.0) {}
};


void displayBanner();
void displayMenu();
void displayRoomOptions();
void getValidIntInput(int& input, const string& prompt, int minVal = 1, int maxVal = INT_MAX);
bool isLeapYear(int year);
bool isValidDate(int year, int month, int day);
bool isFutureOrToday(const Date& date);
void getValidDateInput(Date& date);
void transactionModule(Reservation& res);
void reservationProcessingModule(Reservation& res, double rate);
void paymentModule(Reservation& res);
void receiptModule(const Reservation& res);

int main() {
    Reservation res;
    int choice;
    bool exitProgram = false;

    displayBanner();

    do {
        displayMenu();
        cout << "Enter your choice: ";
        getValidIntInput(choice, "", 1, 4);

        switch (choice) {
            case 1:
                displayRoomOptions();
                break;
            case 2:
                transactionModule(res);
                break;
            case 3:
                receiptModule(res);
                break;
            case 4:
                cout << "\nThank you for using the Hotel Reservation System!\n";
                exitProgram = true;
                break;
            default:
                cout << "\nInvalid choice. Please try again.\n";
        }
    } while (!exitProgram);

    return 0;
}

void displayBanner() {
    cout << "\n";
    cout << "\n";
    cout << "MMP\"\"MM\"\"YMM                                    `7MMF'                                                  mm                           \n";
    cout << "P'   MM   `7                                      MM                                                    MM                           \n";
    cout << "     MM  .gP\"Ya   ,6\"Yb.  `7MMpMMMb.pMMMb.        MM  `7MMpMMMb.  `7MMpMMMb.  ,pW\"Wq.`7M'   `MF',6\"Yb.mmMMmm ,pW\"Wq.`7Mb,od8 ,pP\"Ybd \n";
    cout << "     MM ,M'   Yb 8)   MM    MM    MM    MM        MM    MM    MM    MM    MM 6W'   `Wb VA   ,V 8)   MM  MM  6W'   `Wb MM' \"' 8I   `\" \n";
    cout << "     MM 8M\"\"\"\"\"\"  ,pm9MM    MM    MM    MM        MM    MM    MM    MM    MM 8M     M8  VA ,V   ,pm9MM  MM  8M     M8 MM     `YMMMa. \n";
    cout << "     MM YM.    , 8M   MM    MM    MM    MM        MM    MM    MM    MM    MM YA.   ,A9   VVV   8M   MM  MM  YA.   ,A9 MM     L.   I8 \n";
    cout << "   .JMML.`Mbmmd' `Moo9^Yo..JMML  JMML  JMML.    .JMML..JMML  JMML..JMML  JMML.`Ybmd9'     W    `Moo9^Yo.`Mbmo`Ybmd9'.JMML.   M9mmmP' \n";
    cout << "\n";
    cout << "\n";
    cout << "\n";
    cout << "                                                     ,,                                                                              \n";
    cout << "                            `7MMF'  `7MMF'          *MM                                                                              \n";
    cout << "                              MM      MM             MM                                                                              \n";
    cout << "                              MM      MM `7MM  `7MM  MM,dMMb.                                                                         \n";
    cout << "                              MMmmmmmmMM   MM    MM  MM    `Mb                                                                        \n";
    cout << "                              MM      MM   MM    MM  MM     M8                                                                        \n";
    cout << "                              MM      MM   MM    MM  MM.   ,M9                                                                        \n";
    cout << "                            .JMML.  .JMML. `Mbod\"YML.P^YbmdP'                                                                         \n";
    cout << "\n";
    cout << "\n";
}

void displayMenu() {
    cout << "\nMAIN MENU\n";
    cout << "1. View Available Room Types\n";
    cout << "2. Make a Reservation\n";
    cout << "3. View Receipt\n";
    cout << "4. Exit\n";
}

void displayRoomOptions() {
    cout << "\nAvailable Room Types and Rates (per night):\n";
    cout << "1. Standard Room - " << fixed << setprecision(2) << STANDARD_RATE << " Php\n";
    cout << "2. Deluxe Room   - " << fixed << setprecision(2) << DELUXE_RATE << " Php\n";
    cout << "3. Family Suite  - " << fixed << setprecision(2) << FAMILY_SUITE_RATE << " Php\n";
}

void getValidIntInput(int& input, const string& prompt, int minVal, int maxVal) {
    cout << prompt;
    while (!(cin >> input) || input < minVal || input > maxVal) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between " << minVal << " and " << maxVal << ": ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

bool isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

bool isValidDate(int year, int month, int day) {
    if (year < 2025 || year > 2026 || month < 1 || month > 12 || day < 1) return false;
    int daysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeapYear(year)) daysInMonth[2] = 29;
    return day <= daysInMonth[month];
}

bool isFutureOrToday(const Date& date) {
    time_t now = time(0);
    tm* currentTime = localtime(&now);
    int currentYear = currentTime->tm_year + 1900;
    int currentMonth = currentTime->tm_mon + 1;
    int currentDay = currentTime->tm_mday;
    if (date.year > currentYear) return true;
    if (date.year < currentYear) return false;
    if (date.month > currentMonth) return true;
    if (date.month < currentMonth) return false;
    return date.day >= currentDay;
}

void getValidDateInput(Date& date) {
    int year, month, day;
    bool valid = false;
    do {
        cout << "\nCheck-in Date Selection\n";
        getValidIntInput(year, "Enter year (2025-2026): ", 2025, 2026);
        getValidIntInput(month, "Enter month (1-12): ", 1, 12);
        getValidIntInput(day, "Enter day (1-31): ", 1, 31);
        if (!isValidDate(year, month, day)) {
            cout << "Invalid date (e.g., February has 28/29 days). Please try again.\n";
            continue;
        }
        Date tempDate;
        tempDate.year = year;
        tempDate.month = month;
        tempDate.day = day;
        if (!isFutureOrToday(tempDate)) {
            cout << "Date must be today or in the future. Please try again.\n";
            continue;
        }
        date = tempDate;
        valid = true;
    } while (!valid);
}

void transactionModule(Reservation& res) {
    int roomChoice;
    double rate = 0.0;

    do {
        cout << "\nEnter Customer Name: ";
        getline(cin, res.customerName);
        if (res.customerName.empty()) {
            cout << "Customer name cannot be empty. Please try again.\n";
        }
    } while (res.customerName.empty());

    getValidDateInput(res.checkInDate);

    cout << "\nSelect Room Type:\n";
    displayRoomOptions();
    getValidIntInput(roomChoice, "Enter your choice (1-3): ", 1, 3);

    switch (roomChoice) {
        case 1: res.roomType = "Standard Room"; rate = STANDARD_RATE; break;
        case 2: res.roomType = "Deluxe Room"; rate = DELUXE_RATE; break;
        case 3: res.roomType = "Family Suite"; rate = FAMILY_SUITE_RATE; break;
    }

    getValidIntInput(res.numNights, "Enter number of nights : ", 1);
    getValidIntInput(res.numGuests, "Enter number of guests : ", 1);

    reservationProcessingModule(res, rate);
    paymentModule(res);
    receiptModule(res);
}

void reservationProcessingModule(Reservation& res, double rate) {
    int extraGuests = max(0, res.numGuests - 2);
    double extraCharge = res.numNights * extraGuests * EXTRA_GUEST_RATE;
    res.totalCost = (rate * res.numNights) + extraCharge;
}

void paymentModule(Reservation& res) {
    cout << fixed << setprecision(2);
    cout << "\nTotal Cost: " << res.totalCost << " Php\n";

    do {
        cout << "Enter payment amount (at least " << res.totalCost << " Php): ";
        if (!(cin >> res.payment) || res.payment < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid payment. Please enter a non-negative amount.\n";
            res.payment = 0.0;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (res.payment < res.totalCost) {
            cout << "Insufficient payment. Please enter a valid amount.\n";
        }
    } while (res.payment < res.totalCost);

    res.change = res.payment - res.totalCost;
}

void receiptModule(const Reservation& res) {
    if (res.customerName.empty() || res.roomType.empty() || res.checkInDate.year == 0) {
        cout << "\nNo valid reservation found. Please make a reservation first.\n";
        return;
    }

    cout << fixed << setprecision(2);
    cout << "\nBOOKING RECEIPT\n";
    cout << "Customer Name: " << res.customerName << endl;
    cout << "Check-in Date: " << res.checkInDate.toString() << endl;
    cout << "Room Type: " << res.roomType << endl;
    cout << "Number of Nights: " << res.numNights << endl;
    cout << "Number of Guests: " << res.numGuests << endl;
    cout << "----------------\n";
    cout << "Total Cost: Php " << res.totalCost << endl;
    cout << "Payment: Php " << res.payment << endl;
    cout << "Change: Php " << res.change << endl;
    cout << "Reservation Confirmed! Enjoy your stay!\n";
}

