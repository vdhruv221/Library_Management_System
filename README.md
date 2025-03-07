# Library Management System

## Overview
A C++ library management system with user roles (Student/Faculty/Librarian), book management, borrowing/returning functionality, fines calculation, and data persistence. Automatically initializes with sample data if no data files exist.

## Getting Started

### Prerequisites
- C++ compiler (g++ recommended)
- Standard Library headers

### Compilation & Execution

### Data Persistence
The system uses 3 data files:
- `books.txt`: Book catalog
- `users.txt`: User accounts
- `accounts.txt`: Borrowing records & fines

*First run* creates these with default data:
- 10 sample books
- 5 Students (IDs 1-5)
- 3 Faculty (IDs 6-8)
- 1 Librarian (ID 9, password: admin)

## Class Overview

### 📚 Core Classes

#### 1. `Book`
- **Purpose**: Manage book metadata and status
- **Key Features**:
  - Track availability (Available/Borrowed/Reserved)
  - Store borrowing dates and borrower ID
  - Serialize/deserialize for data persistence
  - Handle ISBN validation

#### 2. `User` Hierarchy
- **Base Class**: `User`
  - **Derived Classes**:
    - `Student`: 
      - Borrow limit: 3 books
      - Loan period: 15 days
      - Fine: ₹10/day
    - `Faculty`:
      - Borrow limit: 5 books
      - Loan period: 30 days
      - 60-day grace period
    - `Librarian`: Full system access

#### 3. `Account`
- **Purpose**: Manage user borrowing activities
- **Features**:
  - Track current loans
  - Maintain borrowing history
  - Calculate/store fines
  - Handle payments
  - Check overdue status

#### 4. `Transaction`
- **Purpose**: Record book lending details
- **Tracks**:
  - Borrow/return dates
  - Calculated fines
  - Payment status

### 🏛️ System Classes

#### 1. `Library`
- **Core System**:
  - Manage all books/users
  - Handle borrowing/returning logic
  - Calculate fines
  - Load/save data
  - Search functionality

#### 2. `CLI`
- **User Interface**:
  - Role-based menus
  - Authentication system
  - Interactive prompts
  - Clear screen management

## Key Features
- **Role-Based Access**:
  - Students: Basic borrowing
  - Faculty: Extended privileges
  - Librarians: Full system control
- **Fine System**:
  - Automated calculation
  - Partial/full payment options
- **Search**:
  - Title/Author/ISBN lookup
- **Data Management**:
  - Automatic initialization
  - Manual save/load
  - Persistent storage

## Default Credentials
- **Librarian**: ID `9`, Password `admin`
- **Students**: IDs `1-5`, Password `ABC123`
- **Faculty**: IDs `6-8`, Password `ABC123`



