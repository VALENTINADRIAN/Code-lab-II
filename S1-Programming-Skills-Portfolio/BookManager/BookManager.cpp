#include <iostream> // For handling cin and cout
#include <fstream>  // For reading and leading data from bookData.txt
#include <vector>   // For creating a dynamic list of books
#include <string>   // For storing text fields like titles
#include <sstream>  // For splitting comma-separated text lines

using namespace std;

// Class defining a library book object with the attributes
class Book {
private:
    string title;
    string author;
    int pages;
    string id;
    bool isBorrowed;

public:
    // Constructor to initialise a book object when data is loaded
    Book(string t, string a, int p, string i, bool status) {
        title = t;
        author = a;
        pages = p;
        id = i;
        isBorrowed = status;
    }

    // Getter function to share the book ID 
    string getId() const {
        return id;
    }

    // Getter function to share the book title 
    string getTitle() const {
        return title;
    }

    // Getter function to check if the book is currently out
    bool getBorrowedStatus() const {
        return isBorrowed;
    }

    // Setter function to update the availability status
    void setBorrowedStatus(bool status) {
        isBorrowed = status;
    }

    // Member function to print the full book details
    void displayInfo() const {
        cout << "[" << id << "] " << title << " by " << author 
             << " (" << pages << " pages) | Status: ";
        if (isBorrowed) {
            cout << "BORROWED\n";
        } else {
            cout << "AVAILABLE\n";
        }
    }
};

int main() {
    vector<Book> libraryCatalog;
    ifstream file("bookData.txt");

    // Check if the file failed to open properly
    if (!file.is_open()) {
        cout << "Error! Could not open bookData.txt\n";
        return 1;
    }

    string line;
    // Process the file line by line to extract variables
    while (getline(file, line)) {
        stringstream ss(line);
        string t, a, p_str, i, status_str;

        // Split the line by commas into temporary strings
        getline(ss, t, ',');
        getline(ss, a, ',');
        getline(ss, p_str, ',');
        getline(ss, i, ',');
        getline(ss, status_str, ',');

        // Only create the book if basic parts are successfully extracted
        int p = stoi(p_str); // Convert the page string to an integer
        bool status = (status_str == "true"); 
        
        // Create a book object and move it into the vector
        libraryCatalog.push_back(Book(t, a, p, i, status));
    }
    file.close();

    int choice = 0;
    
    // Main interface menu loop
    while (choice != 5) {
        cout << "\n=== Library Management System ===\n";
        cout << "1) Display All Books\n";
        cout << "2) Search Individual Book by ID\n";
        cout << "3) Borrow a Book\n";
        cout << "4) Return a Book\n";
        cout << "5) Exit Program\n";
        cout << "Enter selection (1-5): ";
        cin >> choice;

        // Protect against loop crashes if letters are typed into the integer input
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            choice = 0;
            cout << "Invalid entry. Numbers only.\n";
            continue;
        }

        // Action 1: Print the entire inventory list
        if (choice == 1) {
            cout << "\n--- Current Library Catalog ---\n";
            for (size_t k = 0; k < libraryCatalog.size(); k++) {
                libraryCatalog[k].displayInfo();
            }
        }
        // Actions 2, 3, and 4 all require searching the vector for a match first
        else if (choice >= 2 && choice <= 4) {
            string searchId;
            cout << "Enter the Book ID: ";
            cin >> searchId;

            bool found = false;
            // Loop through the catalogue to check IDs
            for (size_t k = 0; k < libraryCatalog.size(); k++) {
                if (libraryCatalog[k].getId() == searchId) {
                    found = true;

                    // Handle search display
                    if (choice == 2) {
                        cout << "\n--- Book Record Found ---\n";
                        libraryCatalog[k].displayInfo();
                    }
                    // Handle checkout rules
                    else if (choice == 3) {
                        if (libraryCatalog[k].getBorrowedStatus()) {
                            cout << "Transaction Declined: This book has been checked out already.\n";
                        } else {
                            libraryCatalog[k].setBorrowedStatus(true);
                            cout << "Success: Checked out \"" << libraryCatalog[k].getTitle() << "\".\n";
                        }
                    }
                    // Handle return rules
                    else if (choice == 4) {
                        if (!libraryCatalog[k].getBorrowedStatus()) {
                            cout << "Transaction Declined: This book has not been borrowed.\n";
                        } else {
                            libraryCatalog[k].setBorrowedStatus(false);
                            cout << "Success: Returned \"" << libraryCatalog[k].getTitle() << "\".\n";
                        }
                    }
                    break; 
                }
            }
            // Error handling message if the target ID was missing
            if (!found) {
                cout << "Error: Book ID \"" << searchId << "\" not found in system.\n";
            }
        }
    }

    cout << "\nExiting Library Management System. Goodbye.\n";
    return 0; // End of programme
}
