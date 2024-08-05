# BITS Pilani AUDI Ticket Booking System

This project is a console-based ticket booking system for events at BITS Pilani. The system provides separate modes for admin and user functionalities. Admins can manage events, while users can book tickets for events.

## Features

### Admin Mode

- **Add Event:**
  - Admins can add new events by specifying the event name, date, time, charge per student, and available seats (default is 300 seats).
  
- **Delete Event:**
  - Admins can delete an existing event from the database. Deleting an event also removes all associated student bookings for that event.
  
- **Calculate Revenue:**
  - Admins can calculate the revenue generated from a specific event by multiplying the number of booked seats with the charge per student.
  
- **View Bookings:**
  - Admins can view the list of all students who have booked seats for any event. The details include the student's name, ID, event name, and the number of seats booked.

### User Mode

- **Display Events:**
  - Users can view the list of all available events along with their details such as date, time, available seats, and charge per student.
  
- **Book Tickets:**
  - Users can book tickets for any available event by providing their name, BITS ID, and the number of seats they wish to book.

## Prerequisites

- **MySQL:** Make sure you have MySQL installed and running on your system.
- **C++ Compiler:** Any standard C++ compiler that supports the C++11 standard or later.
- **MySQL Connector for C++:** Ensure that you have MySQL C++ connector installed and properly configured.

## Installation and Setup

1. **Clone the repository:**
   ```bash
   git clone <repository-url>

2. **Create a database named college:**
   Create the following table in the database:
   `````bash
   CREATE TABLE event (
    name VARCHAR(50) PRIMARY KEY,
    date VARCHAR(20),
    time VARCHAR(10),
    available_seats INT,
    charge INT );
   
   CREATE TABLE student (
    name VARCHAR(50),
    event_name VARCHAR(50),
    id VARCHAR(15),
    num INT,
    FOREIGN KEY (event_name) REFERENCES event(name)
   );


3.  **Compile the code using your g++ compiler:**
Compile the c++ code using your compiler:
```bash
g++ -o ticket_booking_system main.cpp -lmysql
```

4.  **Run the program application:**
   Run teh compiled file:
```bash
./ticket_booking_system
