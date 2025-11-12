# 🛠️ Hostel Maintenance Management System (C++ OOPS Project)

A console-based application designed to manage and track maintenance requests submitted by students in a hostel. It provides separate interfaces for **Students** to log issues and provide feedback, and **Managers** to track, prioritize, and resolve pending requests.

## 🚀 Key Features

* **User Authentication:** Separate login/registration for **Students** (with room number) and **Managers**.
* **Request Submission:** Students can submit maintenance requests, specifying the category (Electrical, Plumbing, etc.) and marking them as **Emergency** for urgent prioritization.
* **Request Management:** Managers can view pending requests, which are **prioritized by emergency status**, and mark them as resolved.
* **Feedback System:** Students can provide a **rating (1-5 stars)** and feedback on resolved requests.
* **Data Persistence:** Requests and feedback are saved to text files (`requests.txt`, `feedback.txt`) to maintain state between program executions.
* **Activity Logging:** All user actions (login, registration, request submission, resolution) are logged to a `log.txt` file for auditing.

---

## 💡 OOPS Concepts Implemented

This project effectively demonstrates core Object-Oriented Programming principles in C++:

* **Classes and Objects:** Real-world entities are modeled using classes: `Person`, `Student`, `Request`, and `Manager`.
* **Inheritance:** The `Student` and `Manager` classes **inherit** from the base `Person` class, reusing common attributes (`name`, `designation`) and methods.
* **Encapsulation:** Data members are kept `private` and accessed/modified only through public member functions, ensuring data integrity.
* **Polymorphism (Runtime):** The `displayInfo()` method is declared `virtual` in the base `Person` class and **overridden** in derived classes, allowing different behavior based on the object type.
* **Data Abstraction:** Complex file handling logic (saving and loading requests) is hidden within the `Manager` and `Request` classes.

---

## ⚙️ Requirements & Setup

* **Language:** C++
* **Compiler:** G++ or any standard C++ compiler.
* **Operating System:** Console-based.

### How to Compile and Run

1.  Save the code as a `.cpp` file (e.g., `main.cpp`).
2.  Compile using a C++ compiler:
    ```bash
    g++ main.cpp -o hostel_maintenance
    ```
3.  Run the executable:
    ```bash
    ./hostel_maintenance
    ```
