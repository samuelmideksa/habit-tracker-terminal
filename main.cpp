#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <regex>

using namespace std;

struct Habit
{
    string name;
    string notes;
    int numRepetitions;
    int interval;
    string unit;
    int position;
    int target;
    string targetType;
};

// Function prototypes
void displayMenu();
void displayHabits(const vector<Habit> &habits);
void addHabit(vector<Habit> &habits);
void saveHabits(const vector<Habit> &habits);
void loadHabits(vector<Habit> &habits);
void trackPerformance(const vector<Habit> &habits);
void displayPerformance(const vector<Habit> &habits);
void updatePerformance(const vector<Habit> &habits, const string &date, const string &habitName, const string &unit);

int main()
{
    vector<Habit> habits;
    loadHabits(habits);
    // Print the size of the habits vector
    cout << "Number of habits loaded: " << habits.size() << endl;

    int choice;
    do
    {
        displayMenu();
        cin >> choice;
        cin.ignore(); // Clear newline from input buffer

        switch (choice)
        {
        case 1:
            displayHabits(habits);
            break;
        case 2:
            addHabit(habits);
            break;
        case 3:
            trackPerformance(habits);
            break;
        case 4:
            displayPerformance(habits);
            break;
        case 5:
            // Quit
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);

    saveHabits(habits);

    return 0;
}

void displayMenu()
{
    cout << "Habit Tracker Menu:" << endl;
    cout << "1. Display Habits" << endl;
    cout << "2. Add Habit" << endl;
    cout << "3. Track Performance" << endl;
    cout << "4. Display Performance" << endl;
    cout << "5. Quit" << endl;
    cout << "Enter your choice: ";
}

void displayHabits(const vector<Habit> &habits)
{
    cout << "Habits List:" << endl;
    cout << left << setw(10) << "Position" << setw(15) << "Name" << setw(30) << "Notes"
         << setw(15) << "NumRepetitions" << setw(10) << "Interval" << setw(10) << "Unit"
         << setw(15) << "TargetType" << setw(15) << "Target" << endl;
    for (const Habit &habit : habits)
    {
        cout << left << setw(10) << habit.position << setw(15) << habit.name
             << setw(30) << habit.notes << setw(15) << habit.numRepetitions
             << setw(10) << habit.interval << setw(10) << habit.unit
             << setw(15) << habit.targetType << setw(15) << habit.target << endl;
    }
}

void addHabit(vector<Habit> &habits)
{
    Habit newHabit;
    cout << "Enter habit name: ";
    getline(cin, newHabit.name);

    // Ask user to choose between measurable or yes/no habit
    cout << "Choose habit type:" << endl;
    cout << "1. Yes/No" << endl;
    cout << "2. Measurable" << endl;
    int habitTypeChoice;
    while (!(cin >> habitTypeChoice) || (habitTypeChoice != 1 && habitTypeChoice != 2))
    {
        cout << "Invalid input, please enter either 1 or 2: ";
        cin.clear();
        cin.ignore();
    }
    cin.ignore(); // Clear newline from input buffer

    if (habitTypeChoice == 1)
    {
        newHabit.unit = "-";
        newHabit.targetType = "-";
        newHabit.target = -1;
    }
    else if (habitTypeChoice == 2)
    {
        cout << "Enter unit: ";
        getline(cin, newHabit.unit);
        cout << "Enter target in numbers: ";
        while (!(cin >> newHabit.target))
        {
            cout << "Invalid input. Please enter a valid number for target: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << "Enter target type" << endl;
        cout << "1. At least" << endl;
        cout << "2. At most" << endl;
        int habitTargetType;
        while (!(cin >> habitTargetType) || (habitTargetType != 1 && habitTargetType != 2))
        {
            cout << "Invalid input, please enter either 1 or 2: ";
            cin.clear();
            cin.ignore();
        }
        cin.ignore();
        if (habitTargetType == 1)
        {
            newHabit.targetType = "At least";
        }
        else
        {
            newHabit.targetType = "At most";
        }
    }

    cout << "Enter habit notes: ";
    getline(cin, newHabit.notes);
    cout << "For n times in n days" << endl;
    cout << "Enter number of repetitions per day: ";
    while (!(cin >> newHabit.numRepetitions))
    {
        cout << "Invalid input. Please enter a valid number for repetitions: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    }
    cout << "Enter interval of days: ";
    while (!(cin >> newHabit.interval))
    {
        cout << "Invalid input. Please enter a valid number for interval: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear the input buffer
    }

    newHabit.position = habits.size() + 1;
    habits.push_back(newHabit);
    cout << "Habit added successfully." << endl;
}

void saveHabits(const vector<Habit> &habits)
{
    ofstream file("./records/habits.csv", ios::out); // Overwrite existing content
    file << "Position,Name,Notes,NumRepetitions,Interval,Unit,TargetType,Target" << endl;
    for (const Habit &habit : habits)
    {
        file << habit.position << "," << habit.name << "," << habit.notes << ","
             << habit.numRepetitions << "," << habit.interval << "," << habit.unit << ","
             << habit.targetType << "," << habit.target << endl;
    }
    file.close();
}

void loadHabits(vector<Habit> &habits)
{
    ifstream file("./records/habits.csv");
    if (!file.is_open())
    {
        cout << "Habit file not found. Starting with an empty list." << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header line
    while (getline(file, line))
    {
        istringstream ss(line);
        Habit habit;

        // Extract data from each comma-separated field
        char comma; // Used to consume the commas in the CSV
        ss >> habit.position >> comma;
        getline(ss, habit.name, ',');
        getline(ss, habit.notes, ',');
        ss >> habit.numRepetitions >> comma >> habit.interval >> comma;
        getline(ss, habit.unit, ',');
        getline(ss, habit.targetType, ',');
        ss >> habit.target;

        habits.push_back(habit);
    }
    file.close();
}

void trackPerformance(const vector<Habit> &habits)
{
    displayHabits(habits);

    cout << "Enter date (YYYY-MM-DD): ";
    string date;
    cout << "Enter date (YYYY-MM-DD): ";

    // Regular expression pattern for matching the date format
    regex datePattern("\\d{4}-\\d{2}-\\d{2}");
    while (true)
    {
        cin >> date;
        if (regex_match(date, datePattern))
        {
            break;
        }
        else
        {
            cout << "Invalid date format. Please enter date in YYYY-MM-DD format: ";
        }
    }

    cout << "Enter habit name to track performance: ";
    string habitName;
    cin.ignore();
    getline(cin, habitName);

    updatePerformance(habits, date, habitName, "");
}

void displayPerformance(const vector<Habit> &habits)
{
    displayHabits(habits);
    cout << "Enter habit name to display performance for: ";
    string habitName;
    getline(cin, habitName);

    // Check if the entered habit name exists in the list of habits
    bool habitExists = false;
    for (const Habit &habit : habits)
    {
        if (habit.name == habitName)
        {
            habitExists = true;
            break;
        }
    }

    if (!habitExists)
    {
        cout << "Error: The habit '" << habitName << "' does not exist." << endl;
        return; // Exit the function
    }

    // Proceed to display performance data
    ifstream file("./records/habits/" + habitName + ".csv");
    if (!file.is_open())
    {
        cout << "Error: Unable to open " << habitName << " file." << endl;
        return;
    }

    cout << left << setw(12) << "Date" << setw(10) << "Amount" << endl;

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string date;
        int amount;

        // Read date and amount using comma as delimiter
        if (getline(ss, date, ',') && ss >> amount)
        {
            cout << left << setw(12) << date << setw(10) << amount << endl;
        }
        else
        {
            cout << "Error reading data from file." << endl;
        }
    }

    file.close();
}

void updatePerformance(const vector<Habit> &habits, const string &date, const string &habitName, const string &unit)
{
    ofstream file("./records/habits/" + habitName + ".csv", ios::app); // Open file in append mode
    if (!file.is_open())
    {
        cout << "Error: Unable to open .csv for writing." << endl;
        return;
    }

    file << date << ","; // Write date to the file

    // Check if the entered habit name exists in the list of habits
    bool habitExists = false;
    for (const Habit &habit : habits)
    {
        if (habit.name == habitName)
        {
            habitExists = true;
            break;
        }
    }

    if (!habitExists)
    {
        cout << "Error: The habit '" << habitName << "' does not exist." << endl;
        return; // Exit the function
    }

    // Find the habit with the given name
    bool habitFound = false;
    for (const Habit &habit : habits)
    {
        if (habit.name == habitName)
        {
            habitFound = true;

            // For yes/no habit
            if (habit.unit == "-")
            {
                cout << "Did you complete '" << habitName << "' on " << date << "?" << endl;
                cout << "1. Yes" << endl;
                cout << "0. No" << endl;
                int completed;
                while (!(cin >> completed) || (completed != 1 && completed != 2))
                {
                    cout << "Invalid input, please enter either 1 or 2: ";
                    cin.clear();
                    cin.ignore();
                }
                file << completed;
            }
            // For measurable habit
            else
            {
                cout << "Enter the amount of '" << habitName << "' done on " << date << " " << unit << ": ";
                double amount;
                while (!(cin >> amount))
                {
                    cout << "Invalid input. Please enter a valid number for amount: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                file << amount;
            }
            break;
        }
    }

    if (!habitFound)
    {
        cout << "Habit '" << habitName << "' not found." << endl;
    }

    file << endl; // End the line
    file.close();
}
