#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <stdexcept>
#include <ctime>

using namespace std;

enum MaintenanceCategory
{
    ELECTRICAL,
    PLUMBING,
    FURNITURE,
    HOUSEKEEPING,
    INTERNET,
    OTHER 
};

// Log user activity
void logActivity(const string &activity)
{
    ofstream logFile("log.txt", ios::app);
    if (!logFile.is_open())
    {
        cerr << "Error: Unable to open log file.\n";
        return;
    }

    // Get current time
    time_t now = time(0);
    char *dt = ctime(&now);

    logFile << "[" << dt << "] " << activity << "\n";
    logFile.close();
}

void createFileIfNotExists(const string &filename)
{
    ifstream checkFile(filename);
    if (!checkFile.is_open())
    {
        ofstream newFile(filename);
        newFile.close();
    }
    checkFile.close();
}

bool login(const string &filename, const string &name, const string &roomNumber = "")
{
    createFileIfNotExists(filename);

    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Unable to open data file.\n";
        return false;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string storedName, storedRoomNumber;
        getline(ss, storedName, ',');
        if (roomNumber != "")
            getline(ss, storedRoomNumber, ',');

        if (storedName == name && (roomNumber == "" || storedRoomNumber == roomNumber))
        {
            file.close();
            logActivity(name + " logged in.");
            return true;
        }
    }

    file.close();
    return false;
}

void registerUser(const string &filename, const string &name, const string &roomNumber = "")
{
    createFileIfNotExists(filename);

    ofstream file(filename, ios::app);
    if (!file.is_open())
    {
        cout << "Error: Unable to open data file.\n";
        return;
    }

    file << name;
    if (roomNumber != "")
        file << "," << roomNumber;
    file << "\n";
    file.close();
    cout << "Registration successful!\n";
    logActivity(name + " registered.");
}

class Person
{
protected:
    string name;
    string designation;

public:
    Person(string name, string designation) : name(name), designation(designation) {}

    virtual void displayInfo() const
    {
        cout << "Name: " << name << "\n";
        cout << "Designation: " << designation << "\n";
    }
};

class Request
{
private:
    int requestID;
    string studentName;
    string roomNumber;
    int floor;
    string issueDescription;
    MaintenanceCategory category;
    bool isEmergency; // New attribute
    bool isResolved;
    int rating;
    string feedback;

public:
    
    Request(int id, string name, string room, int flr, string issue, MaintenanceCategory cat, bool emergency = false, bool resolved = false)
        : requestID(id), studentName(name), roomNumber(room), floor(flr),
          issueDescription(issue), category(cat), isEmergency(emergency), isResolved(resolved), rating(0) {}

    
    bool getIsEmergency() const { return isEmergency; }

    void displayRequest() const
    {
        cout << "Request ID: " << requestID << "\n";
        cout << "Student Name: " << studentName << "\n";
        cout << "Room Number: " << roomNumber << "\n";
        cout << "Floor: " << floor << "\n";
        cout << "Category: " << getCategoryString() << "\n";
        cout << "Issue: " << issueDescription << "\n";
        cout << "Status: " << (isResolved ? "Resolved" : "Pending") << "\n";
        cout << "Emergency: " << (isEmergency ? "Yes" : "No") << "\n"; // Display emergency status
        if (rating > 0)
        {
            cout << "Rating: " << rating << " stars\n";
            cout << "Feedback: " << feedback << "\n";
        }
    }

    string getCategoryString() const
    {
        switch (category)
        {
        case ELECTRICAL:
            return "Electrical";
        case PLUMBING:
            return "Plumbing";
        case FURNITURE:
            return "Furniture";
        case HOUSEKEEPING:
            return "Housekeeping";
        case INTERNET:
            return "Internet";
        default:
            return "Other";
        }
    }

    void setFeedback(int stars, const string &feedbackText)
    {
        if (stars >= 1 && stars <= 5)
        {
            rating = stars;
            feedback = feedbackText;

            ofstream feedbackFile("feedback.txt", ios::app);
            if (feedbackFile.is_open())
            {
                feedbackFile << "Request ID: " << requestID << "\n"
                             << "Student: " << studentName << "\n"
                             << "Rating: " << rating << " stars\n"
                             << "Feedback: " << feedback << "\n"
                             << "----------------------------------------\n";
                feedbackFile.close();
            }

            logActivity(studentName + " submitted feedback for request ID " + to_string(requestID) + ".");
        }
    }

    string toTXT() const
    {
        return "REQUEST_ID: " + to_string(requestID) + "\n" +
               "STUDENT: " + studentName + "\n" +
               "ROOM: " + roomNumber + "\n" +
               "FLOOR: " + to_string(floor) + "\n" +
               "CATEGORY: " + getCategoryString() + "\n" +
               "ISSUE: " + issueDescription + "\n" +
               "STATUS: " + (isResolved ? "Resolved" : "Pending") + "\n" +
               (rating > 0 ? "RATING: " + to_string(rating) + "\n" +
                                 "FEEDBACK: " + feedback + "\n"
                           : "") +
               "----------------------------------------\n";
    }

    static Request *fromTXT(const vector<string> &lines)
    {
        int requestID = 0;
        string studentName, roomNumber, issueDescription;
        int floor = 0;
        bool isResolved = false;
        MaintenanceCategory category = OTHER;
        int rating = 0;
        string feedback;

        for (const string &line : lines)
        {
            if (line.find("REQUEST_ID: ") == 0)
                requestID = stoi(line.substr(12));
            else if (line.find("STUDENT: ") == 0)
                studentName = line.substr(9);
            else if (line.find("ROOM: ") == 0)
                roomNumber = line.substr(6);
            else if (line.find("FLOOR: ") == 0)
                floor = stoi(line.substr(7));
            else if (line.find("CATEGORY: ") == 0)
            {
                string catStr = line.substr(10);
                if (catStr == "Electrical")
                    category = ELECTRICAL;
                else if (catStr == "Plumbing")
                    category = PLUMBING;
                else if (catStr == "Furniture")
                    category = FURNITURE;
                else if (catStr == "Housekeeping")
                    category = HOUSEKEEPING;
                else if (catStr == "Internet")
                    category = INTERNET;
                else
                    category = OTHER;
            }
            else if (line.find("ISSUE: ") == 0)
                issueDescription = line.substr(7);
            else if (line.find("STATUS: ") == 0)
                isResolved = (line.substr(8) == "Resolved");
            else if (line.find("RATING: ") == 0)
                rating = stoi(line.substr(8));
            else if (line.find("FEEDBACK: ") == 0)
                feedback = line.substr(10);
        }

        Request *req = new Request(requestID, studentName, roomNumber, floor,
                                   issueDescription, category, isResolved);
        if (rating > 0)
        {
            req->setFeedback(rating, feedback);
        }
        return req;
    }

    void resolve()
    {
        isResolved = true;
        logActivity("Request ID " + to_string(requestID) + " resolved.");
    }
    bool getIsResolved() const { return isResolved; }
    int getRequestID() const { return requestID; }
    MaintenanceCategory getCategory() const { return category; }

    bool operator==(const Request &other) const
    {
        return requestID == other.requestID;
    }
};

class Student : public Person
{
private:
    string roomNumber;
    int floor;

public:
    Student(string name, string room, int floor)
        : Person(name, "Student"), roomNumber(room), floor(floor) {}

    void displayInfo() const override
    {
        Person::displayInfo();
        cout << "Room Number: " << roomNumber << "\n";
        cout << "Floor: " << floor << "\n";
    }

    string getRoomNumber() const { return roomNumber; }
    string getName() const { return name; }
    int getFloor() const { return floor; }
};

class Manager : public Person
{
public:
    vector<Request *> requestList;
    string filename;
    int nextRequestID;

    void createFileIfNotExists()
    {
        ifstream checkFile(filename);
        if (!checkFile.is_open())
        {
            ofstream newFile(filename);
            newFile.close();
        }
        checkFile.close();
    }

    Manager(string name, string file)
        : Person(name, "Manager"), filename(file), nextRequestID(1)
    {
        loadRequests();
    }

    void viewRequests()
    {
        if (requestList.empty())
        {
            cout << "No maintenance requests.\n";
            return;
        }

        cout << "\nPending Emergency Requests:\n";
        for (Request *req : requestList)
        {
            if (!req->getIsResolved() && req->getIsEmergency())
            {
                req->displayRequest();
            }
        }

        cout << "\nPending Non-Emergency Requests:\n";
        for (Request *req : requestList)
        {
            if (!req->getIsResolved() && !req->getIsEmergency())
            {
                req->displayRequest();
            }
        }
    }

    void resolveRequest(int requestID)
    {
        for (Request *req : requestList)
        {
            if (req->getRequestID() == requestID && !req->getIsResolved())
            {
                req->resolve();
                saveRequests();
                return;
            }
        }
        cout << "Request not found or already resolved.\n";
    }

    void loadRequests()
    {
        createFileIfNotExists();
        ifstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error: Unable to open file.\n";
            return;
        }

        string line;
        vector<string> requestLines;
        while (getline(file, line))
        {
            if (line == "----------------------------------------")
            {
                Request *req = Request::fromTXT(requestLines);
                requestList.push_back(req);
                requestLines.clear();
            }
            else
            {
                requestLines.push_back(line);
            }
        }
        file.close();
    }

    void saveRequests()
    {
        ofstream file(filename);
        if (!file.is_open())
        {
            cerr << "Error: Unable to open file.\n";
            return;
        }

        for (Request *req : requestList)
        {
            file << req->toTXT();
        }
        file.close();
    }

    void submitRequest(Student &student, string issueDescription, bool isEmergency, MaintenanceCategory category)
    {
        Request *req = new Request(nextRequestID++, student.getName(), student.getRoomNumber(),
                                   student.getFloor(), issueDescription, category, isEmergency);
        requestList.push_back(req);
        saveRequests();
        logActivity(student.getName() + " submitted a new request with ID " + to_string(req->getRequestID()) +
                    (isEmergency ? " (Emergency)" : "") + ".");
    }

    void displayInfo() const override
    {
        Person::displayInfo();
    }

    void viewFeedback()
    {
        ifstream feedbackFile("feedback.txt");
        if (!feedbackFile.is_open())
        {
            cout << "No feedback available.\n";
            return;
        }
        cout << "\n\n";
        string line;
        while (getline(feedbackFile, line))
        {
            cout << line << "\n";
        }
        cout << "\n\n";
        feedbackFile.close();
    }
};

int main()
{
    Manager manager("Admin", "requests.txt");

    string choice, name, roomNumber, role;

    while (true)
    {
        cout << "Welcome to Hostel Maintenance System!\n";
        cout << "Please choose an option:\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == "1")
        {
            cout << "Are you logging in as a student or manager? (Enter 'student' or 'manager'): ";
            cin >> role;

            if (role == "student")
            {
                cout << "Enter your name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter your room number: ";
                getline(cin, roomNumber);

                if (!login("login_data.txt", name, roomNumber))
                {
                    cout << "Login failed! Incorrect details.\n";
                }
                else
                {
                    cout << "Login successful!\n";
                    break;
                }
            }
            else if (role == "manager")
            {
                cout << "Enter your name: ";
                cin.ignore();
                getline(cin, name);

                if (!login("manager_data.txt", name))
                {
                    cout << "Login failed! Incorrect details.\n";
                }
                else
                {
                    cout << "Login successful!\n";
                    break;
                }
            }
            else
            {
                cout << "Invalid role.\n";
            }
        }
        else if (choice == "2")
        {
            cout << "Are you registering as a student or manager? (Enter 'student' or 'manager'): ";
            cin >> role;

            if (role == "student")
            {
                cout << "Enter your name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter your room number: ";
                getline(cin, roomNumber);

                registerUser("login_data.txt", name, roomNumber);
            }
            else if (role == "manager")
            {
                cout << "Enter your name: ";
                cin.ignore();
                getline(cin, name);

                registerUser("manager_data.txt", name);
            }
            else
            {
                cout << "Invalid role.\n";
            }
            cout << "Please login with your registered credentials.\n";
        }
        else
        {
            cout << "Invalid choice.\n";
        }
    }

    if (role == "student")
    {
        Student student(name, roomNumber, 1); // Assuming default floor as 1 for simplicity

        while (true)
        {
            cout << "\nStudent Menu:\n";
            cout << "1. Submit Request\n";
            cout << "2. Provide Feedback\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == "1")
            {
                string issueDescription;
                bool isEmergency;
                int categoryInput;

                cout << "Describe your issue: ";
                cin.ignore();
                getline(cin, issueDescription);
                cout << "Is this an emergency? (1 for Yes, 0 for No): ";
                cin >> isEmergency;

                cout << "Select the category of your issue:\n";
                cout << "1. Electrical\n2. Plumbing\n3. Furniture\n4. Housekeeping\n5. Internet\n6. Other\n";
                cout << "Enter category number: ";
                cin >> categoryInput;

                MaintenanceCategory category = static_cast<MaintenanceCategory>(categoryInput - 1);
                manager.submitRequest(student, issueDescription, isEmergency, category);
            }

            else if (choice == "2")
            {
                int requestID, rating;
                string feedback;

                cout << "Enter the request ID for which you want to give feedback: ";
                cin >> requestID;
                cout << "Enter rating (1-5): ";
                cin >> rating;
                cout << "Provide your feedback: ";
                cin.ignore();
                getline(cin, feedback);

                for (Request *req : manager.requestList)
                {
                    if (req->getRequestID() == requestID)
                    {
                        req->setFeedback(rating, feedback);
                        break;
                    }
                }
            }
            else if (choice == "3")
            {
                break;
            }
            else
            {
                cout << "Invalid choice.\n";
            }
        }
    }
    else if (role == "manager")
    {
        while (true)
        {
            cout << "\nManager Menu:\n";
            cout << "1. View Requests\n";
            cout << "2. Resolve Request\n";
            cout << "3. View Feedback\n";
            cout << "4. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            if (choice == "1")
            {
                manager.viewRequests();
            }
            else if (choice == "2")
            {
                int requestID;
                cout << "Enter the request ID to resolve: ";
                cin >> requestID;
                manager.resolveRequest(requestID);
            }
            else if (choice == "3")
            {
                manager.viewFeedback();
            }
            else if (choice == "4")
            {
                break;
            }
            else
            {
                cout << "Invalid choice.\n";
            }
        }
    }

    return 0;
}
//  & ".\OOPS PROJECT\temp7.exe"