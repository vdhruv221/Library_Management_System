#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <algorithm>
#include <limits>

using namespace std;

// Forward declarations
class Book;
class User;
class Account;
class Library;

// Helper functions for date handling
time_t getCurrentTime() {
    return time(nullptr);
}

time_t parseDateString(const string& dateStr) {
    tm timeStruct = {};
    istringstream ss(dateStr);
    ss >> get_time(&timeStruct, "%Y-%m-%d");
    return mktime(&timeStruct);
}

string timeToString(time_t time) {
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", localtime(&time));
    return string(buffer);
}

int calculateDayGap(time_t start, time_t end) {
    const int SECONDS_PER_DAY = 86400;
    return static_cast<int>(difftime(end, start) / SECONDS_PER_DAY);
}

time_t addDays(time_t date, int days) {
    return date + (days * 24 * 60 * 60);
}

// Book class
class Book {
private:
    string title;
    string author;
    string publisher;
    int year;
    string ISBN;
    string status;  // "Available", "Borrowed", "Reserved"
    time_t borrowDate;
    time_t dueDate;
    int borrowerID;

public:
    Book() : year(0), status("Available"), borrowDate(0), dueDate(0), borrowerID(-1) {}
    
    Book(const string& title, const string& author, const string& publisher, 
         int year, const string& ISBN) 
        : title(title), author(author), publisher(publisher), 
          year(year), ISBN(ISBN), status("Available"), 
          borrowDate(0), dueDate(0), borrowerID(-1) {}

    // Getters
    string getTitle() const { return title; }
    string getAuthor() const { return author; }
    string getPublisher() const { return publisher; }
    int getYear() const { return year; }
    string getISBN() const { return ISBN; }
    string getStatus() const { return status; }
    time_t getBorrowDate() const { return borrowDate; }
    time_t getDueDate() const { return dueDate; }
    int getBorrowerID() const { return borrowerID; }

    // Setters
    void setTitle(const string& newTitle) { title = newTitle; }
    void setAuthor(const string& newAuthor) { author = newAuthor; }
    void setPublisher(const string& newPublisher) { publisher = newPublisher; }
    void setYear(int newYear) { year = newYear; }
    void setISBN(const string& newISBN) { ISBN = newISBN; }
    void setStatus(const string& newStatus) { status = newStatus; }
    void setBorrowDate(time_t date) { borrowDate = date; }
    void setDueDate(time_t date) { dueDate = date; }
    void setBorrowerID(int id) { borrowerID = id; }

    // Display book details
    void displayDetails() const {
        cout << "ISBN: " << ISBN << endl;
        cout << "Title: " << title << endl;
        cout << "Author: " << author << endl;
        cout << "Publisher: " << publisher << endl;
        cout << "Year: " << year << endl;
        cout << "Status: " << status << endl;
        
        if (status == "Borrowed") {
            cout << "Borrowed Date: " << timeToString(borrowDate) << endl;
            cout << "Due Date: " << timeToString(dueDate) << endl;
        }
        cout << "------------------------" << endl;
    }

    // Serialize book data to string
    string serialize() const {
        stringstream ss;
        ss << title << "|" << author << "|" << publisher << "|" << year << "|" 
           << ISBN << "|" << status << "|" << borrowDate << "|" 
           << dueDate << "|" << borrowerID;
        return ss.str();
    }

    // Deserialize string to book data
    void deserialize(const string& data) {
        stringstream ss(data);
        string token;
        
        getline(ss, title, '|');
        getline(ss, author, '|');
        getline(ss, publisher, '|');
        
        string yearStr;
        getline(ss, yearStr, '|');
        year = stoi(yearStr);
        
        getline(ss, ISBN, '|');
        getline(ss, status, '|');
        
        string dateStr;
        getline(ss, dateStr, '|');
        borrowDate = stoll(dateStr);
        
        getline(ss, dateStr, '|');
        dueDate = stoll(dateStr);
        
        string idStr;
        getline(ss, idStr, '|');
        borrowerID = stoi(idStr);
    }
};

// Forward declarations for Account
class Account;

// User base class
class User {
protected:
    int userID;
    string name;
    string password;
    string role;
    Account* account;

public:
    User() : userID(0), account(nullptr) {}
    
    User(int id, const string& name, const string& password, const string& role) 
        : userID(id), name(name), password(password), role(role), account(nullptr) {}
    
    virtual ~User() {}

    // Getters
    int getUserID() const { return userID; }
    string getName() const { return name; }
    string getPassword() const { return password; }
    string getRole() const { return role; }
    Account* getAccount() const { return account; }

    // Setters
    void setUserID(int id) { userID = id; }
    void setName(const string& newName) { name = newName; }
    void setPassword(const string& newPassword) { password = newPassword; }
    void setRole(const string& newRole) { role = newRole; }
    void setAccount(Account* acc) { account = acc; }

    // Display user details
    virtual void displayDetails() const {
        cout << "User ID: " << userID << endl;
        cout << "Name: " << name << endl;
        cout << "Role: " << role << endl;
    }

    // Virtual functions for book operations to be overridden by derived classes
    virtual bool canBorrowBook() const { return false; }
    virtual int getMaxBooks() const { return 0; }
    virtual int getMaxDays() const { return 0; }
    virtual bool hasFineExemption() const { return false; }
    
    // Serialize user data to string
    virtual string serialize() const {
        stringstream ss;
        ss << userID << "|" << name << "|" << password << "|" << role;
        return ss.str();
    }

    // Deserialize string to user data
    virtual void deserialize(const string& data) {
        stringstream ss(data);
        string token;
        
        string idStr;
        getline(ss, idStr, '|');
        userID = stoi(idStr);
        
        getline(ss, name, '|');
        getline(ss, password, '|');
        getline(ss, role, '|');
    }
};

// Student class
class Student : public User {
public:
    Student() : User() {
        role = "Student";
    }
    
    Student(int id, const string& name, const string& password) 
        : User(id, name, password, "Student") {}
    
    // Override virtual functions
    bool canBorrowBook() const override { return true; }
    int getMaxBooks() const override { return 3; }
    int getMaxDays() const override { return 15; }
    bool hasFineExemption() const override { return false; }
};

// Faculty class
class Faculty : public User {
public:
    Faculty() : User() {
        role = "Faculty";
    }
    
    Faculty(int id, const string& name, const string& password) 
        : User(id, name, password, "Faculty") {}
    
    // Override virtual functions
    bool canBorrowBook() const override { return true; }
    int getMaxBooks() const override { return 5; }
    int getMaxDays() const override { return 30; }
    bool hasFineExemption() const override { return true; }
};

// Librarian class
class Librarian : public User {
public:
    Librarian() : User() {
        role = "Librarian";
    }
    
    Librarian(int id, const string& name, const string& password) 
        : User(id, name, password, "Librarian") {}
    
    // Override virtual functions
    bool canBorrowBook() const override { return false; }
};

// Transaction class to track borrowing history
class Transaction {
private:
    string ISBN;
    time_t borrowDate;
    time_t returnDate;
    time_t dueDate;
    int fine;
    bool finePaid;

public:
    Transaction() : borrowDate(0), returnDate(0), dueDate(0), fine(0), finePaid(false) {}
    
    Transaction(const string& ISBN, time_t borrowDate, time_t dueDate) 
        : ISBN(ISBN), borrowDate(borrowDate), returnDate(0), 
          dueDate(dueDate), fine(0), finePaid(false) {}

    // Getters
    string getISBN() const { return ISBN; }
    time_t getBorrowDate() const { return borrowDate; }
    time_t getReturnDate() const { return returnDate; }
    time_t getDueDate() const { return dueDate; }
    int getFine() const { return fine; }
    bool isFinePaid() const { return finePaid; }

    // Setters
    void setISBN(const string& newISBN) { ISBN = newISBN; }
    void setBorrowDate(time_t date) { borrowDate = date; }
    void setReturnDate(time_t date) { returnDate = date; }
    void setDueDate(time_t date) { dueDate = date; }
    void setFine(int newFine) { fine = newFine; }
    void setFinePaid(bool paid) { finePaid = paid; }

    // Serialize transaction data
    string serialize() const {
        stringstream ss;
        ss << ISBN << "|" << borrowDate << "|" << returnDate << "|" 
           << dueDate << "|" << fine << "|" << (finePaid ? "1" : "0");
        return ss.str();
    }

    // Deserialize string to transaction data
    void deserialize(const string& data) {
        stringstream ss(data);
        
        getline(ss, ISBN, '|');
        
        string dateStr;
        getline(ss, dateStr, '|');
        borrowDate = stoll(dateStr);
        
        getline(ss, dateStr, '|');
        returnDate = stoll(dateStr);
        
        getline(ss, dateStr, '|');
        dueDate = stoll(dateStr);
        
        string fineStr;
        getline(ss, fineStr, '|');
        fine = stoi(fineStr);
        
        string paidStr;
        getline(ss, paidStr, '|');
        finePaid = (paidStr == "1");
    }
};

// Account class
class Account {
private:
    int userID;
    vector<string> currentlyBorrowedISBNs;
    vector<Transaction> borrowHistory;
    int totalFine;

public:
    Account() : userID(0), totalFine(0) {}
    
    Account(int id) : userID(id), totalFine(0) {}

    // Getters
    int getUserID() const { return userID; }
    const vector<string>& getCurrentlyBorrowedISBNs() const { return currentlyBorrowedISBNs; }
    const vector<Transaction>& getBorrowHistory() const { return borrowHistory; }
    int getTotalFine() const { return totalFine; }

    // Setters
    void setUserID(int id) { userID = id; }
    void setTotalFine(int fine) { totalFine = fine; }

    // Add a book to currently borrowed list
    void addBorrowedBook(const string& ISBN) {
        currentlyBorrowedISBNs.push_back(ISBN);
    }

    // Remove a book from currently borrowed list
    void removeBorrowedBook(const string& ISBN) {
        auto it = find(currentlyBorrowedISBNs.begin(), currentlyBorrowedISBNs.end(), ISBN);
        if (it != currentlyBorrowedISBNs.end()) {
            currentlyBorrowedISBNs.erase(it);
        }
    }

    // Add a transaction to borrow history
    void addTransaction(const Transaction& transaction) {
        borrowHistory.push_back(transaction);
    }

    // Update transaction when a book is returned
    void updateTransaction(const string& ISBN, time_t returnDate, int fine) {
        for (auto& transaction : borrowHistory) {
            if (transaction.getISBN() == ISBN && transaction.getReturnDate() == 0) {
                transaction.setReturnDate(returnDate);
                transaction.setFine(fine);
                totalFine += fine;
                break;
            }
        }
    }

    // Pay fines
    bool payFine(int amount) {
        if (amount <= totalFine) {
            totalFine -= amount;
            
            // Mark transactions as paid based on the amount paid
            int remainingAmount = amount;
            for (auto& transaction : borrowHistory) {
                if (!transaction.isFinePaid() && transaction.getFine() > 0) {
                    if (remainingAmount >= transaction.getFine()) {
                        remainingAmount -= transaction.getFine();
                        transaction.setFinePaid(true);
                    } else {
                        // Partial payment not supported in this implementation
                        // We could extend this to handle partial payments if needed
                        break;
                    }
                }
            }
            return true;
        }
        return false;
    }

    // Pay all fines
    void payAllFines() {
        for (auto& transaction : borrowHistory) {
            if (!transaction.isFinePaid() && transaction.getFine() > 0) {
                transaction.setFinePaid(true);
            }
        }
        totalFine = 0;
    }

    // Check if there are any overdue books
    bool hasOverdueBooks(int userType, time_t currentTime) const {
        int maxDays = (userType == 1) ? 15 : 30;  // 1 for Student, 2 for Faculty
        int maxOverdueDays = (userType == 2) ? 60 : 0;  // Faculty has 60 days grace period
        
        for (const auto& isbn : currentlyBorrowedISBNs) {
            for (const auto& transaction : borrowHistory) {
                if (transaction.getISBN() == isbn && transaction.getReturnDate() == 0) {
                    // Calculate days overdue
                    int daysOverdue = calculateDayGap(transaction.getDueDate(), currentTime);
                    
                    // For Faculty, only consider it overdue if past the grace period
                    if (userType == 2 && daysOverdue > maxOverdueDays) {
                        return true;
                    }
                    // For Students, any overdue is a problem
                    else if (userType == 1 && daysOverdue > 0) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    // Display account details
    void displayDetails(const map<string, Book>& books) const{
        cout << "Account for User ID: " << userID << endl;
        cout << "Currently Borrowed Books: " << currentlyBorrowedISBNs.size() << endl;
        
        if(!currentlyBorrowedISBNs.empty()){
            cout << "Borrowed Books:" << endl;
            for(const auto& isbn : currentlyBorrowedISBNs){
                auto bookIt = books.find(isbn);
                if(bookIt != books.end()){
                    cout << "  - " << bookIt->second.getTitle() << " (ISBN: " << isbn << ")" << endl;
                    
                    // Find the relevant transaction to get due date
                    for(const auto& transaction : borrowHistory){
                        if(transaction.getISBN() == isbn && transaction.getReturnDate() == 0){
                            cout << "    Due Date: " << timeToString(transaction.getDueDate()) << endl;
                            break;
                        }
                    }
                }
            }
        }
        
        if(totalFine > 0){
            cout << "Outstanding Fine: " << totalFine << " rupees" << endl;
        }
        
        if(!borrowHistory.empty()){
            cout << "\nBorrowing History:" << endl;
            for(const auto& transaction : borrowHistory) {
                auto bookIt = books.find(transaction.getISBN());
                if (bookIt != books.end()) {
                    cout << "  - " << bookIt->second.getTitle() << " (ISBN: " << transaction.getISBN() << ")" << endl;
                    cout << "    Borrowed: " << timeToString(transaction.getBorrowDate()) << endl;
                    cout << "    Due: " << timeToString(transaction.getDueDate()) << endl;
                    
                    if (transaction.getReturnDate() > 0) {
                        cout << "    Returned: " << timeToString(transaction.getReturnDate()) << endl;
                        
                        if (transaction.getFine() > 0) {
                            cout << "    Fine: " << transaction.getFine() << " rupees";
                            if (transaction.isFinePaid()) {
                                cout << " (Paid)";
                            } else {
                                cout << " (Unpaid)";
                            }
                            cout << endl;
                        }
                    } else {
                        cout << "    Status: Not returned yet" << endl;
                        
                        // Check if overdue
                        time_t currentTime = getCurrentTime();
                        if (currentTime > transaction.getDueDate()) {
                            int daysOverdue = calculateDayGap(transaction.getDueDate(), currentTime);
                            cout << "    Overdue by " << daysOverdue << " days" << endl;
                        }
                    }
                    cout << endl;
                }
            }
        }
    }

    // Serialize account data
    string serialize() const {
        stringstream ss;
        ss << userID << "|" << totalFine << "|" << currentlyBorrowedISBNs.size();
        
        for (const auto& isbn : currentlyBorrowedISBNs) {
            ss << "|" << isbn;
        }
        
        ss << "|" << borrowHistory.size();
        for (const auto& transaction : borrowHistory) {
            ss << "|" << transaction.serialize();
        }
        
        return ss.str();
    }

    // Deserialize string to account data
    void deserialize(const string& data) {
        stringstream ss(data);
        string token;
        
        getline(ss, token, '|');
        userID = stoi(token);
        
        getline(ss, token, '|');
        totalFine = stoi(token);
        
        getline(ss, token, '|');
        int borrowedCount = stoi(token);
        
        currentlyBorrowedISBNs.clear();
        for (int i = 0; i < borrowedCount; i++) {
            getline(ss, token, '|');
            currentlyBorrowedISBNs.push_back(token);
        }
        
        getline(ss, token, '|');
        int historyCount = stoi(token);
        
        borrowHistory.clear();
        for (int i = 0; i < historyCount; i++) {
            getline(ss, token, '|');
            Transaction transaction;
            transaction.deserialize(token);
            borrowHistory.push_back(transaction);
        }
    }
};

// Library class
class Library {
private:
    map<string, Book> books;  // ISBN -> Book
    map<int, User*> users;    // UserID -> User
    map<int, Account> accounts;  // UserID -> Account
    int nextUserID;

public:
    Library() : nextUserID(1) {}
    
    ~Library() {
        for (auto& pair : users) {
            delete pair.second;
        }
    }

    // Initialize the library with default data
    void initialize() {
        // Add default books
        addBook(Book("The Silent Patient", "Alex Michaelides", "Celadon Books", 2019, "1000"));
        addBook(Book("Project Hail Mary", "Andy Weir", "Ballantine Books", 2021, "1001"));
        addBook(Book("The Night Circus", "Erin Morgenstern", "Doubleday", 2011, "1002"));
        addBook(Book("Pride and Prejudice", "Jane Austen", "T. Egerton", 1813, "1003"));
        addBook(Book("Educated", "Tara Westover", "Random House", 2018, "1004"));
        addBook(Book("The Hobbit", "J.R.R. Tolkien", "George Allen & Unwin", 1937, "1005"));
        addBook(Book("The Lord of the Rings", "J.R.R. Tolkien", "George Allen & Unwin", 1954, "1006"));
        addBook(Book("Harry Potter and the Philosopher's Stone", "J.K. Rowling", "Bloomsbury", 1997, "1007"));
        addBook(Book("Klara and the Sun", "Kazuo Ishiguro", "Knopf", 2021, "1008"));
        addBook(Book("The Da Vinci Code", "Dan Brown", "Doubleday", 2003, "1009"));

        // Add default users
        addUser(new Student(nextUserID++, "Ram", "ABC123"));
        addUser(new Student(nextUserID++, "Shyam", "ABC123"));
        addUser(new Student(nextUserID++, "Ghanshyam", "ABC123"));
        addUser(new Student(nextUserID++, "Dhruv", "ABC123"));
        addUser(new Student(nextUserID++, "Kartik", "ABC123"));
        
        addUser(new Faculty(nextUserID++, "Abhishek", "ABC123"));
        addUser(new Faculty(nextUserID++, "Venkatesh", "ABC123"));
        addUser(new Faculty(nextUserID++, "Shubham", "ABC123"));
        
        addUser(new Librarian(nextUserID++, "Librarian", "admin"));
        
        // Create accounts for all users
        for (const auto& pair : users) {
            if (pair.second->getRole() != "Librarian") {
                accounts[pair.first] = Account(pair.first);
                pair.second->setAccount(&accounts[pair.first]);
            }
        }
    }

    // Book management
    void addBook(const Book& book) {
        books[book.getISBN()] = book;
    }
    
    bool removeBook(const string& ISBN) {
        // Check if the book is currently borrowed
        auto it = books.find(ISBN);
        if (it != books.end()) {
            if (it->second.getStatus() == "Borrowed") {
                cout << "Cannot remove book: it is currently borrowed." << endl;
                return false;
            }
            books.erase(it);
            return true;
        }
        return false;
    }
    
    void updateBook(const Book& book) {
        auto it = books.find(book.getISBN());
        if (it != books.end()) {
            // Preserve the status, borrowDate, dueDate, and borrowerID
            string status = it->second.getStatus();
            time_t borrowDate = it->second.getBorrowDate();
            time_t dueDate = it->second.getDueDate();
            int borrowerID = it->second.getBorrowerID();
            
            books[book.getISBN()] = book;
            
            books[book.getISBN()].setStatus(status);
            books[book.getISBN()].setBorrowDate(borrowDate);
            books[book.getISBN()].setDueDate(dueDate);
            books[book.getISBN()].setBorrowerID(borrowerID);
        }
    }

    // User management
    void addUser(User* user) {
        users[user->getUserID()] = user;
        
        // Create an account for the user if they're not a librarian
        if (user->getRole() != "Librarian") {
            accounts[user->getUserID()] = Account(user->getUserID());
            user->setAccount(&accounts[user->getUserID()]);
        }
    }
    
    bool removeUser(int userID){
        auto it = users.find(userID);
        if(it != users.end()){
            // Check if user has any borrowed books
            if(it->second->getRole() != "Librarian" && 
                !accounts[userID].getCurrentlyBorrowedISBNs().empty()){
                cout << "Cannot remove user: they have borrowed books." << endl;
                return false;
            }
            
            delete it->second;
            users.erase(it);
            
            // Remove the user's account
            accounts.erase(userID);
            return true;
        }
        return false;
    }

    // Book borrowing functionalities
    bool borrowBook(int userID, const string& ISBN, time_t currentTime) {
        auto userIt = users.find(userID);
        auto bookIt = books.find(ISBN);
        
        if(userIt == users.end() || bookIt == books.end()) {
            cout << "User or book not found." << endl;
            return false;
        }
        
        User* user = userIt->second;
        Book& book = bookIt->second;
        
        // Check if user can borrow books
        if(!user->canBorrowBook()){
            cout << "This user role cannot borrow books." << endl;
            return false;
        }
        
        // Check if book is available
        if(book.getStatus() != "Available"){
            cout << "Book is not available for borrowing." << endl;
            return false;
        }
        
        // Get the user's account
        Account& account = accounts[userID];
        
        // Check borrowing limits
        if (account.getCurrentlyBorrowedISBNs().size() >= user->getMaxBooks()) {
            cout << "User has reached their borrowing limit." << endl;
            return false;
        }
        
        // Check for unpaid fines
        if (account.getTotalFine() > 0) {
            cout << "User has unpaid fines. Please clear fines before borrowing." << endl;
            return false;
        }
        
        // Check for overdue books (for Faculty only)
        if (user->getRole() == "Faculty" && account.hasOverdueBooks(2, currentTime)) {
            cout << "User has books overdue for more than 60 days. Cannot borrow more." << endl;
            return false;
        }
        
        // Update book status
        book.setStatus("Borrowed");
        book.setBorrowerID(userID);
        book.setBorrowDate(currentTime);
        book.setDueDate(addDays(currentTime, user->getMaxDays()));
        
        // Update user account
        account.addBorrowedBook(ISBN);
        
        // Create transaction record
        Transaction transaction(ISBN, currentTime, book.getDueDate());
        account.addTransaction(transaction);
        
        cout << "Book borrowed successfully. Due date: " 
             << timeToString(book.getDueDate()) << endl;
        return true;
    }

    bool returnBook(int userID, const string& ISBN, time_t currentTime) {
        auto userIt = users.find(userID);
        auto bookIt = books.find(ISBN);
        
        if (userIt == users.end() || bookIt == books.end()) {
            cout << "User or book not found." << endl;
            return false;
        }
        
        User* user = userIt->second;
        Book& book = bookIt->second;
        
        // Check if the book is borrowed by this user
        if (book.getStatus() != "Borrowed" || book.getBorrowerID() != userID) {
            cout << "This book is not borrowed by this user." << endl;
            return false;
        }
        
        // Calculate fine if any
        int fine = 0;
        if (!user->hasFineExemption() && currentTime > book.getDueDate()) {
            int daysLate = calculateDayGap(book.getDueDate(), currentTime);
            fine = daysLate * 10;  // 10 rupees per day
            cout << "Book is " << daysLate << " days late. Fine: " << fine << " rupees." << endl;
        }
        
        // Update book status
        book.setStatus("Available");
        book.setBorrowerID(-1);
        book.setBorrowDate(0);
        book.setDueDate(0);
        
        // Update user account
        Account& account = accounts[userID];
        account.removeBorrowedBook(ISBN);
        account.updateTransaction(ISBN, currentTime, fine);
        
        cout << "Book returned successfully." << endl;
        if (fine > 0) {
            cout << "Fine added to account: " << fine << " rupees." << endl;
        }
        
        return true;
    }

    // Display functions
    void displayAllBooks() const {
        cout << "==== All Books ====" << endl;
        if (books.empty()) {
            cout << "No books in the library." << endl;
        } else {
            for (const auto& pair : books) {
                pair.second.displayDetails();
            }
        }
    }
    
    void displayAvailableBooks() const {
        cout << "==== Available Books ====" << endl;
        bool found = false;
        for (const auto& pair : books) {
            if (pair.second.getStatus() == "Available") {
                pair.second.displayDetails();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No available books in the library." << endl;
        }
    }
    
    void displayAllUsers() const {
        cout << "==== All Users ====" << endl;
        if (users.empty()) {
            cout << "No users in the system." << endl;
        } else {
            for (const auto& pair : users) {
                pair.second->displayDetails();
                cout << "------------------------" << endl;
            }
        }
    }
    
    void displayUsersByRole(const string& role) const {
        cout << "==== " << role << "s ====" << endl;
        bool found = false;
        for (const auto& pair : users) {
            if (pair.second->getRole() == role) {
                pair.second->displayDetails();
                cout << "------------------------" << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No " << role << "s found in the system." << endl;
        }
    }
    
    void displayUserAccount(int userID) const {
        auto userIt = users.find(userID);
        auto accountIt = accounts.find(userID);
        
        if (userIt != users.end() && accountIt != accounts.end()) {
            cout << "==== Account Details for " << userIt->second->getName() << " ====" << endl;
            accountIt->second.displayDetails(books);
        } else {
            cout << "User or account not found." << endl;
        }
    }
    
    void displayBookDetails(const string& ISBN) const {
        auto it = books.find(ISBN);
        if (it != books.end()) {
            it->second.displayDetails();
        } else {
            cout << "Book not found." << endl;
        }
    }

    // Search functions
    void searchBooksByTitle(const string& title) const {
        cout << "==== Search Results for Title: " << title << " ====" << endl;
        bool found = false;
        for (const auto& pair : books) {
            if (pair.second.getTitle().find(title) != string::npos) {
                pair.second.displayDetails();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No books found with that title." << endl;
        }
    }
    
    void searchBooksByAuthor(const string& author) const {
        cout << "==== Search Results for Author: " << author << " ====" << endl;
        bool found = false;
        for (const auto& pair : books) {
            if (pair.second.getAuthor().find(author) != string::npos) {
                pair.second.displayDetails();
                found = true;
            }
        }
        
        if (!found) {
            cout << "No books found by that author." << endl;
        }
    }
    
    void searchBooksByISBN(const string& ISBN) const {
        cout << "==== Search Results for ISBN: " << ISBN << " ====" << endl;
        auto it = books.find(ISBN);
        if (it != books.end()) {
            it->second.displayDetails();
        } else {
            cout << "No book found with that ISBN." << endl;
        }
    }
    
    // File operations
    void saveToFiles() const {
        // Save books
        ofstream bookFile("books.txt");
        if (bookFile.is_open()) {
            for (const auto& pair : books) {
                bookFile << pair.second.serialize() << endl;
            }
            bookFile.close();
        }
        
        // Save users
        ofstream userFile("users.txt");
        if (userFile.is_open()) {
            for (const auto& pair : users) {
                userFile << pair.second->getRole() << "|" << pair.second->serialize() << endl;
            }
            userFile.close();
        }
        
        // Save accounts
        ofstream accountFile("accounts.txt");
        if (accountFile.is_open()) {
            for (const auto& pair : accounts) {
                accountFile << pair.second.serialize() << endl;
            }
            accountFile.close();
        }
        
        cout << "Data saved successfully." << endl;
    }
    
    void loadFromFiles() {
        // Clear existing data
        for (auto& pair : users) {
            delete pair.second;
        }
        users.clear();
        books.clear();
        accounts.clear();
        
        // Load books
        ifstream bookFile("books.txt");
        if (bookFile.is_open()) {
            string line;
            while (getline(bookFile, line)) {
                Book book;
                book.deserialize(line);
                books[book.getISBN()] = book;
            }
            bookFile.close();
        }
        
        // Load users
        ifstream userFile("users.txt");
        if (userFile.is_open()) {
            string line;
            while (getline(userFile, line)) {
                stringstream ss(line);
                string role;
                getline(ss, role, '|');
                
                string userData;
                getline(ss, userData);
                
                User* user = nullptr;
                if (role == "Student") {
                    user = new Student();
                } else if (role == "Faculty") {
                    user = new Faculty();
                } else if (role == "Librarian") {
                    user = new Librarian();
                }
                
                if (user) {
                    user->deserialize(userData);
                    users[user->getUserID()] = user;
                    
                    // Update nextUserID
                    if (user->getUserID() >= nextUserID) {
                        nextUserID = user->getUserID() + 1;
                    }
                }
            }
            userFile.close();
        }
        
        // Load accounts
        ifstream accountFile("accounts.txt");
        if (accountFile.is_open()) {
            string line;
            while (getline(accountFile, line)) {
                Account account;
                account.deserialize(line);
                accounts[account.getUserID()] = account;
                
                // Link account to user
                auto userIt = users.find(account.getUserID());
                if (userIt != users.end()) {
                    userIt->second->setAccount(&accounts[account.getUserID()]);
                }
            }
            accountFile.close();
        }
        
        cout << "Data loaded successfully." << endl;
    }
    
    // Helper functions
    bool authenticateUser(int userID, const string& password) const {
        auto it = users.find(userID);
        if (it != users.end()) {
            return it->second->getPassword() == password;
        }
        return false;
    }
    
    User* getUser(int userID) const {
        auto it = users.find(userID);
        if (it != users.end()) {
            return it->second;
        }
        return nullptr;
    }
    
    bool bookExists(const string& ISBN) const {
        return books.find(ISBN) != books.end();
    }
    
    bool userExists(int userID) const {
        return users.find(userID) != users.end();
    }
    
    int getNextUserID() const {
        return nextUserID;
    }
};

// CLI class to handle user interface
class CLI {
private:
    Library library;
    User* currentUser;
    
    void clearScreen() {
        // For Windows
        #ifdef _WIN32
            system("cls");
        // For Unix/Linux
        #else
            system("clear");
        #endif
    }
    
    void waitForInput() {
        cout << "\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
    }
    
    int getChoice(int min, int max) {
        int choice;
        while (true) {
            cout << "Enter your choice: ";
            cin >> choice;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number between " << min << " and " << max << "." << endl;
            } else if (choice < min || choice > max) {
                cout << "Choice out of range. Please enter a number between " << min << " and " << max << "." << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return choice;
            }
        }
    }
    
    string getLine(const string& prompt) {
        string input;
        cout << prompt;
        getline(cin, input);
        return input;
    }
    
    int getInt(const string& prompt) {
        int input;
        while (true) {
            cout << prompt;
            cin >> input;
            
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a number." << endl;
            } else {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return input;
            }
        }
    }

public:
    CLI() : currentUser(nullptr) {}
    
    void start() {
        // Try to load data from files, or initialize with default data
        ifstream testFile("books.txt");
        if (testFile.good()) {
            testFile.close();
            library.loadFromFiles();
        } else {
            library.initialize();
        }
        
        while (true) {
            clearScreen();
            if (!currentUser) {
                showLoginMenu();
            } else if (currentUser->getRole() == "Student") {
                showStudentMenu();
            } else if (currentUser->getRole() == "Faculty") {
                showFacultyMenu();
            } else if (currentUser->getRole() == "Librarian") {
                showLibrarianMenu();
            }
        }
    }
    
    void showLoginMenu() {
        cout << "===== Library Management System =====" << endl;
        cout << "1. Login" << endl;
        cout << "2. Exit" << endl;
        
        int choice = getChoice(1, 2);
        
        switch (choice) {
            case 1:
                login();
                break;
            case 2:
                exit(0);
                break;
        }
    }
    
    void login() {
        clearScreen();
        cout << "===== Login =====" << endl;
        
        int userID = getInt("Enter User ID: ");
        string password = getLine("Enter Password: ");
        
        if (library.authenticateUser(userID, password)) {
            currentUser = library.getUser(userID);
            cout << "Login successful. Welcome, " << currentUser->getName() << "!" << endl;
            waitForInput();
        } else {
            cout << "Login failed. Invalid User ID or Password." << endl;
            waitForInput();
        }
    }
    
    void logout() {
        currentUser = nullptr;
        cout << "Logged out successfully." << endl;
        waitForInput();
    }
    
    void showStudentMenu() {
        cout << "===== Student Menu =====" << endl;
        cout << "Welcome, " << currentUser->getName() << " (Student)" << endl;
        cout << "1. View Available Books" << endl;
        cout << "2. Search Books" << endl;
        cout << "3. Borrow a Book" << endl;
        cout << "4. Return a Book" << endl;
        cout << "5. View My Account" << endl;
        cout << "6. Pay Fines" << endl;
        cout << "7. Logout" << endl;
        
        int choice = getChoice(1, 7);
        
        switch (choice) {
            case 1:
                library.displayAvailableBooks();
                waitForInput();
                break;
            case 2:
                showSearchMenu();
                break;
            case 3:
                borrowBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                library.displayUserAccount(currentUser->getUserID());
                waitForInput();
                break;
            case 6:
                payFines();
                break;
            case 7:
                logout();
                break;
        }
    }
    
    void showFacultyMenu() {
        cout << "===== Faculty Menu =====" << endl;
        cout << "Welcome, " << currentUser->getName() << " (Faculty)" << endl;
        cout << "1. View Available Books" << endl;
        cout << "2. Search Books" << endl;
        cout << "3. Borrow a Book" << endl;
        cout << "4. Return a Book" << endl;
        cout << "5. View My Account" << endl;
        cout << "6. Logout" << endl;
        
        int choice = getChoice(1, 6);
        
        switch (choice) {
            case 1:
                library.displayAvailableBooks();
                waitForInput();
                break;
            case 2:
                showSearchMenu();
                break;
            case 3:
                borrowBook();
                break;
            case 4:
                returnBook();
                break;
            case 5:
                library.displayUserAccount(currentUser->getUserID());
                waitForInput();
                break;
            case 6:
                logout();
                break;
        }
    }
    
    void showLibrarianMenu() {
        cout << "===== Librarian Menu =====" << endl;
        cout << "Welcome, " << currentUser->getName() << " (Librarian)" << endl;
        cout << "1. View All Books" << endl;
        cout << "2. Add a Book" << endl;
        cout << "3. Remove a Book" << endl;
        cout << "4. Update a Book" << endl;
        cout << "5. View All Users" << endl;
        cout << "6. Add a User" << endl;
        cout << "7. Remove a User" << endl;
        cout << "8. View User Account" << endl;
        cout << "9. Search Books" << endl;
        cout << "10. Save Data" << endl;
        cout << "11. Logout" << endl;
        
        int choice = getChoice(1, 11);
        
        switch (choice) {
            case 1:
                library.displayAllBooks();
                waitForInput();
                break;
            case 2:
                addBook();
                break;
            case 3:
                removeBook();
                break;
            case 4:
                updateBook();
                break;
            case 5:
                showUserListMenu();
                break;
            case 6:
                addUser();
                break;
            case 7:
                removeUser();
                break;
            case 8:
                viewUserAccount();
                break;
            case 9:
                showSearchMenu();
                break;
            case 10:
                library.saveToFiles();
                waitForInput();
                break;
            case 11:
                logout();
                break;
        }
    }
    
    void showSearchMenu() {
        clearScreen();
        cout << "===== Search Books =====" << endl;
        cout << "1. Search by Title" << endl;
        cout << "2. Search by Author" << endl;
        cout << "3. Search by ISBN" << endl;
        cout << "4. Back" << endl;
        
        int choice = getChoice(1, 4);
        
        switch (choice) {
            case 1: {
                string title = getLine("Enter Title: ");
                library.searchBooksByTitle(title);
                waitForInput();
                break;
            }
            case 2: {
                string author = getLine("Enter Author: ");
                library.searchBooksByAuthor(author);
                waitForInput();
                break;
            }
            case 3: {
                string ISBN = getLine("Enter ISBN: ");
                library.searchBooksByISBN(ISBN);
                waitForInput();
                break;
            }
            case 4:
                return;
        }
    }
    
    void showUserListMenu() {
        clearScreen();
        cout << "===== View Users =====" << endl;
        cout << "1. All Users" << endl;
        cout << "2. Students" << endl;
        cout << "3. Faculty" << endl;
        cout << "4. Librarians" << endl;
        cout << "5. Back" << endl;
        
        int choice = getChoice(1, 5);
        
        switch (choice) {
            case 1:
                library.displayAllUsers();
                waitForInput();
                break;
            case 2:
                library.displayUsersByRole("Student");
                waitForInput();
                break;
            case 3:
                library.displayUsersByRole("Faculty");
                waitForInput();
                break;
            case 4:
                library.displayUsersByRole("Librarian");
                waitForInput();
                break;
            case 5:
                return;
        }
    }
    
    void borrowBook() {
        clearScreen();
        cout << "===== Borrow a Book =====" << endl;
        
        string ISBN = getLine("Enter ISBN of the book to borrow: ");
        
        if (!library.bookExists(ISBN)) {
            cout << "Book not found." << endl;
            waitForInput();
            return;
        }
        
        library.borrowBook(currentUser->getUserID(), ISBN, getCurrentTime());
        waitForInput();
    }
    
    void returnBook() {
        clearScreen();
        cout << "===== Return a Book =====" << endl;
        
        // Display currently borrowed books
        library.displayUserAccount(currentUser->getUserID());
        
        string ISBN = getLine("Enter ISBN of the book to return: ");
        
        if (!library.bookExists(ISBN)) {
            cout << "Book not found." << endl;
            waitForInput();
            return;
        }
        
        library.returnBook(currentUser->getUserID(), ISBN, getCurrentTime());
        waitForInput();
    }
    
    void payFines() {
        clearScreen();
        cout << "===== Pay Fines =====" << endl;
        
        // Display current account and fines
        library.displayUserAccount(currentUser->getUserID());
        
        Account* account = currentUser->getAccount();
        if (!account) {
            cout << "Account not found." << endl;
            waitForInput();
            return;
        }
        
        int totalFine = account->getTotalFine();
        if (totalFine <= 0) {
            cout << "You have no outstanding fines." << endl;
            waitForInput();
            return;
        }
        
        cout << "Total Fine: " << totalFine << " rupees" << endl;
        cout << "1. Pay All Fines" << endl;
        cout << "2. Pay Partial Amount" << endl;
        cout << "3. Back" << endl;
        
        int choice = getChoice(1, 3);
        
        switch (choice) {
            case 1:
                account->payAllFines();
                cout << "All fines paid successfully." << endl;
                break;
            case 2: {
                int amount = getInt("Enter amount to pay: ");
                if (amount <= 0) {
                    cout << "Invalid amount." << endl;
                } else if (amount > totalFine) {
                    cout << "Amount is greater than total fine. Paying " << totalFine << " rupees." << endl;
                    account->payAllFines();
                } else {
                    bool paid = account->payFine(amount);
                    if (paid) {
                        cout << "Payment successful. Remaining fine: " 
                             << account->getTotalFine() << " rupees." << endl;
                    } else {
                        cout << "Payment failed." << endl;
                    }
                }
                break;
            }
            case 3:
                return;
        }
        
        waitForInput();
    }
    
    void addBook() {
        clearScreen();
        cout << "===== Add a Book =====" << endl;
        
        string title = getLine("Enter Title: ");
        string author = getLine("Enter Author: ");
        string publisher = getLine("Enter Publisher: ");
        int year = getInt("Enter Year: ");
        string ISBN = getLine("Enter ISBN: ");
        
        if (library.bookExists(ISBN)) {
            cout << "A book with this ISBN already exists." << endl;
            waitForInput();
            return;
        }
        
        Book book(title, author, publisher, year, ISBN);
        library.addBook(book);
        
        cout << "Book added successfully." << endl;
        waitForInput();
    }
    
    void removeBook() {
        clearScreen();
        cout << "===== Remove a Book =====" << endl;
        
        string ISBN = getLine("Enter ISBN of the book to remove: ");
        
        if (!library.bookExists(ISBN)) {
            cout << "Book not found." << endl;
            waitForInput();
            return;
        }
        
        bool removed = library.removeBook(ISBN);
        if (removed) {
            cout << "Book removed successfully." << endl;
        }
        waitForInput();
    }
    
    void updateBook() {
        clearScreen();
        cout << "===== Update a Book =====" << endl;
        
        string ISBN = getLine("Enter ISBN of the book to update: ");
        
        if (!library.bookExists(ISBN)) {
            cout << "Book not found." << endl;
            waitForInput();
            return;
        }
        
        // Display current book details
        library.displayBookDetails(ISBN);
        
        // Get new details
        string title = getLine("Enter New Title (leave empty to keep current): ");
        string author = getLine("Enter New Author (leave empty to keep current): ");
        string publisher = getLine("Enter New Publisher (leave empty to keep current): ");
        
        int year = -1;
        string yearStr = getLine("Enter New Year (leave empty to keep current): ");
        if (!yearStr.empty()) {
            try {
                year = stoi(yearStr);
            } catch (...) {
                cout << "Invalid year. Keeping current value." << endl;
            }
        }
        
        // Create a temporary book object to hold the updates
        Book updatedBook;
        updatedBook.setISBN(ISBN);  // ISBN cannot be changed
        
        // Get the current book to preserve any values not being updated
        Book currentBook;
        ifstream bookFile("books.txt");
        if (bookFile.is_open()) {
            string line;
            while (getline(bookFile, line)) {
                Book book;
                book.deserialize(line);
                if (book.getISBN() == ISBN) {
                    currentBook = book;
                    break;
                }
            }
            bookFile.close();
        }
        
        // Set the updated values, or keep the current ones if not provided
        updatedBook.setTitle(title.empty() ? currentBook.getTitle() : title);
        updatedBook.setAuthor(author.empty() ? currentBook.getAuthor() : author);
        updatedBook.setPublisher(publisher.empty() ? currentBook.getPublisher() : publisher);
        updatedBook.setYear(year == -1 ? currentBook.getYear() : year);
        
        // Update the book
        library.updateBook(updatedBook);
        
        cout << "Book updated successfully." << endl;
        waitForInput();
    }
    
    void addUser() {
        clearScreen();
        cout << "===== Add a User =====" << endl;
        
        string name = getLine("Enter Name: ");
        string password = getLine("Enter Password: ");
        
        cout << "Select Role:" << endl;
        cout << "1. Student" << endl;
        cout << "2. Faculty" << endl;
        int roleChoice = getChoice(1, 2);
        int userID = library.getNextUserID();
        
        User* newUser = nullptr;
        switch (roleChoice) {
            case 1:
                newUser = new Student(userID, name, password);
                break;
            case 2:
                newUser = new Faculty(userID, name, password);
                break;
        }
        
        if (newUser) {
            library.addUser(newUser);
            cout << "User added successfully. User ID: " << userID << endl;
        }
        
        waitForInput();
    }
    
    void removeUser() {
        clearScreen();
        cout << "===== Remove a User =====" << endl;
        
        int userID = getInt("Enter User ID to remove: ");
        
        if (!library.userExists(userID)) {
            cout << "User not found." << endl;
            waitForInput();
            return;
        }
        
        bool removed = library.removeUser(userID);
        if (removed) {
            cout << "User removed successfully." << endl;
        }
        waitForInput();
    }
    
    void viewUserAccount() {
        clearScreen();
        cout << "===== View User Account =====" << endl;
        
        int userID = getInt("Enter User ID: ");
        
        if (!library.userExists(userID)) {
            cout << "User not found." << endl;
            waitForInput();
            return;
        }
        
        library.displayUserAccount(userID);
        waitForInput();
    }
};

int main() {
    CLI cli;
    cli.start();
    return 0;
}