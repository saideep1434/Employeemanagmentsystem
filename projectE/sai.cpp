#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <iostream>
#include <string>
#include <algorithm>

#define max 20
using namespace std;

// Structure of Employee
struct Employee {
    string name;
    long int code;
    string designation;
    int exp;
    int age;
};

class EmployeeManagementSystem {
private:
    Employee emp[max];
    int num;

    // MySQL Connector variables
    sql::mysql::MySQL_Driver* driver;
    sql::Connection* con;

    // Function to initialize MySQL Connector
    void initMySQL() {
        try {
            driver = sql::mysql::get_mysql_driver_instance();
            con = driver->connect("tcp://localhost:3306", "root", "PHW#84#jeor");
            con->setSchema("quickstartdb");
        }
        catch (sql::SQLException& e) {
            cout << "MySQL Connection Error: " << e.what() << endl;
            // Handle the error, perhaps exit the program or return an error code.
        }
    }


    // Function to close MySQL Connection
    void closeMySQL() {
        delete con;
    }

    // Function to insert an employee into MySQL
    void insertIntoMySQL(int index) {
        sql::Statement* stmt = con->createStatement();
        stmt->execute("INSERT INTO EmployeeTable (Name, EmployeeID, Designation, Experience, Age) VALUES ('" + emp[index].name + "', " + to_string(emp[index].code) + ", '" + emp[index].designation + "', " + to_string(emp[index].exp) + ", " + to_string(emp[index].age) + ")");
        delete stmt;
    }

    // Function to calculate the average experience of all employees
    void calculateAverageExperience() {
        initMySQL(); // Initialize MySQL connection

        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT AVG(Experience) AS AvgExp FROM EmployeeTable");

        if (res->next()) {
            cout << "Average Experience of all employees: " << res->getDouble("AvgExp") << " years\n";
        }
        else {
            cout << "Error calculating average experience\n";
        }

        delete res;
        delete stmt;

        closeMySQL(); // Close MySQL connection

        showMenu();
    }

    // Function to sort employees by their ID
    void sortEmployeesByID() {
        initMySQL(); // Initialize MySQL connection

        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM EmployeeTable ORDER BY EmployeeID");

        cout << "Employee Records Sorted by ID:\n";
        cout << "----------------------------------------\n";
        cout << "ID\tName\t\tDesignation\tExperience\tAge\n";
        cout << "----------------------------------------\n";

        while (res->next()) {
            cout << res->getInt("EmployeeID") << "\t" << res->getString("Name") << "\t" << res->getString("Designation") << "\t" << res->getInt("Experience") << "\t" << res->getInt("Age") << "\n";
        }

        cout << "----------------------------------------\n";

        delete res;
        delete stmt;

        closeMySQL(); // Close MySQL connection

        showMenu();
    }

public:
    // Function to build the given datatype
    void build() {
        cout << "insert into The Table\n";
        cout << "Maximum Entries can be " << max << "\n";

        cout << "Enter the number of Entries required: ";
        cin >> num;

        if (num > 20) {
            cout << "Maximum number of Entries is 20\n";
            num = 20;
        }

        initMySQL(); // Initialize MySQL connection

        cout << "Enter the following data:\n";

        for (int i = 0; i < num; i++) {
            cout << "Name: ";
            cin >> emp[i].name;

            cout << "Employee ID: ";
            cin >> emp[i].code;

            cout << "Designation: ";
            cin >> emp[i].designation;

            cout << "Experience: ";
            cin >> emp[i].exp;

            cout << "Age: ";
            cin >> emp[i].age;

            // Insert into MySQL
            insertIntoMySQL(i);
        }

        closeMySQL(); // Close MySQL connection

        showMenu();
    }

    // Function to update employee data
    void update() {
        initMySQL(); // Initialize MySQL connection

        cout << "Enter the Employee ID to update: ";
        long int empID;
        cin >> empID;

        // Check if the employee ID exists
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM EmployeeTable WHERE EmployeeID = " + to_string(empID));

        if (!res->next()) {
            cout << "Employee not found!\n";
        }
        else {
            // Update employee data
            cout << "Enter the new data:\n";
            cout << "Name: ";
            cin >> emp[0].name; // Assuming only the name is updated for simplicity

            stmt->execute("UPDATE EmployeeTable SET Name = '" + emp[0].name + "' WHERE EmployeeID = " + to_string(empID));
            cout << "Employee data updated successfully!\n";
        }

        delete res;
        delete stmt;

        closeMySQL(); // Close MySQL connection

        showMenu();
    }

    // Function to delete an employee by ID
    void deleteEmployee() {
        initMySQL(); // Initialize MySQL connection

        cout << "Enter the Employee ID to delete: ";
        long int empID;
        cin >> empID;

        // Check if the employee ID exists
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM EmployeeTable WHERE EmployeeID = " + to_string(empID));

        if (!res->next()) {
            cout << "Employee not found!\n";
        }
        else {
            // Delete employee
            stmt->execute("DELETE FROM EmployeeTable WHERE EmployeeID = " + to_string(empID));
            cout << "Employee deleted successfully!\n";
        }

        delete res;
        delete stmt;

        closeMySQL(); // Close MySQL connection

        showMenu();
    }

    // Function to search for an employee by ID
    void searchByID() {
        initMySQL(); // Initialize MySQL connection

        cout << "Enter the Employee ID to search: ";
        long int empID;
        cin >> empID;

        // Check if the employee ID exists
        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM EmployeeTable WHERE EmployeeID = " + to_string(empID));

        if (!res->next()) {
            cout << "Employee not found!\n";
        }
        else {
            // Display employee data
            cout << "Employee found!\n";
            cout << "ID\tName\t\tDesignation\tExperience\tAge\n";
            cout << res->getInt("EmployeeID") << "\t" << res->getString("Name") << "\t" << res->getString("Designation") << "\t" << res->getInt("Experience") << "\t" << res->getInt("Age") << "\n";
        }

        delete res;
        delete stmt;

        closeMySQL(); // Close MySQL connection

        showMenu();
    }

    // Function to display all employee records
    void display() {
        initMySQL(); // Initialize MySQL connection

        sql::Statement* stmt = con->createStatement();
        sql::ResultSet* res = stmt->executeQuery("SELECT * FROM EmployeeTable");

        cout << "Employee Records:\n";
        cout << "----------------------------------------\n";
        cout << "ID\tName\tDesignation\tExperience\tAge\n";
        cout << "----------------------------------------\n";

        while (res->next()) {
            cout << res->getInt("EmployeeID") << "\t" << res->getString("Name") << "\t" << res->getString("Designation") << "\t" << "\t" << res->getInt("Experience") << "\t" << "\t" << res->getInt("Age") << "\n";
        }

        cout << "----------------------------------------\n";

        delete res;
        delete stmt;

        closeMySQL(); // Close MySQL connection

        showMenu();
    }

    // Function to display the menu
    void showMenu() {
        int choice;

        cout << "1. Insert into Table\n";
        cout << "2. Update Employee Data\n";
        cout << "3. Delete Employee\n";
        cout << "4. Search by Employee ID\n";
        cout << "5. Display Employee Records\n";
        cout << "6. Sort Employees by ID\n";
        cout << "7. Calculate Average Experience\n";
        cout << "8. Exit\n";

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            build();
            break;
        case 2:
            update();
            break;
        case 3:
            deleteEmployee();
            break;
        case 4:
            searchByID();
            break;
        case 5:
            display();
            break;
        case 6:
            sortEmployeesByID();
            break;
        case 7:
            calculateAverageExperience();
            break;
        case 8:
            closeMySQL(); // Close MySQL connection
            exit(0);
        default:
            cout << "Invalid choice! Please try again.\n";
            showMenu();
        }
    }
};

int main() {
    EmployeeManagementSystem ems;
    ems.showMenu();
    return 0;
}