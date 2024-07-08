#include <iostream>
#include <mysql.h>
#include <windows.h>
#include <sstream>
#include <iomanip> 

using namespace std;

// Database credentials
const char* HOST = "localhost";
const char* USER = "root";
const char* PASS = "Sugana@36";
const char* DB = "college";
MYSQL* obj;

// Admin class definition
class Admin {
private:
    int charge, available_seats;
    string event_name, date, time;
public:
    Admin(string s, int a, string m, string t) {
        event_name = s;
        charge = a;
        available_seats = 300; // Assuming default available seats is 300
        date = m;
        time = t;
    }

    int get_charge() const { return charge; }
    int get_available() const { return available_seats; }
    string get_event_name() const { return event_name; }
    string get_date() const { return date; }
    string get_time() const { return time; }
};

// Student class definition
class Student {
private:
    int seats;
    string name, id, event_name;
public:
    Student(string s, string t, string m, int n) {
        event_name = s;
        name = t;
        id = m;
        seats = n;
    }

    int get_seats() const { return seats; }
    string get_event() const { return event_name; }
    string get_name() const { return name; }
    string get_id() const { return id; }
};

void display(const char* &query, int x) {
    
    if (mysql_query(obj, query)) {
        cout << "Error: " << mysql_error(obj) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(obj);
    if (!result) {
        cout << "Error fetching events: " << mysql_error(obj) << endl;
        return;
    }

    int num_fields = mysql_num_fields(result);
    MYSQL_ROW row;
    if(x==1){
    	cout << setw(20) << left << "Event" << setw(20) << left << "Date" << setw(20) << left << "Time"
         << setw(20) << left << "Available Seats" << setw(20) << left << "Charge" << endl;
	}
	else{
		cout << setw(20) << left << "Name" << setw(20) << left << "ID" << setw(20) << left << "Event"
        <<setw(20) << left << "Seats" << endl;
	}

    while ((row = mysql_fetch_row(result))) {
        for (int i = 0; i < num_fields; i++) {
            cout << setw(20) << left << row[i];
        }
        cout << endl;
    }

    mysql_free_result(result);
}

void get_admin() {
    string s;
    cout << "Name of the event: ";
    cin >> s;
    int charge;
    cout << "Charge per student (in Rs.): ";
    cin >> charge;
    string date;
    cout << "Please enter the date of the event (DD/MM/YYYY): ";
    cin >> date;
    string time;
    cout << "Please enter the time of the event (24hr format): ";
    cin >> time;

    Admin a(s, charge, date, time);

    stringstream ss_charge, ss_available;
    ss_charge << a.get_charge();
    ss_available << a.get_available();

    string insert_query = "INSERT INTO event (name, date, time, available_seats, charge) VALUES ('" +
        a.get_event_name() + "','" + a.get_date() + "','" + a.get_time() + "'," + ss_available.str() + "," + ss_charge.str() + ")";

    if (mysql_query(obj, insert_query.c_str())) {
        cout << "Error: " << mysql_error(obj) << endl;
    }
    else {
        cout << "Event Added Successfully" << endl;
    }
}

// Function to delete an event from database
void delete_event() {
    string event_name;
    cout << "Enter the name of the event you want to delete: ";
    cin >> event_name;

    string delete_query = "DELETE FROM event WHERE name = '" + event_name + "'";
    if (mysql_query(obj, delete_query.c_str())) {
        cout << "Error deleting event: " << mysql_error(obj) << endl;
    }
    else {
        if (mysql_affected_rows(obj) == 0) {
            cout <<"Event '"<<event_name<<"' does not exist."<<endl;
            return;
        } else {
            cout<<"Event '"<< event_name<<"' deleted successfully." << endl;
        }
    }
    
    string query = "DELETE FROM student WHERE event_name = '" + event_name + "'";
    if (mysql_query(obj, query.c_str())) {
        cout << "Error deleting event: " << mysql_error(obj) << endl;
    }
    else {
        if (mysql_affected_rows(obj) > 0) {
            cout<<"Associated student entries deleted successfully."<<endl;
        }
    }
    
}

// Function to calculate revenue from an event
void calculate_revenue() {
    string event_name;
    cout << "Enter the name of the event to calculate revenue: ";
    cin >> event_name;

    string select_query = "SELECT available_seats, charge FROM event WHERE name = '" + event_name + "'";
    if (mysql_query(obj, select_query.c_str())) {
        cout << "Error: " << mysql_error(obj) << endl;
        return;
    }

    MYSQL_RES* result = mysql_store_result(obj);
    if (!result) {
        cout << "Error fetching event details: " << mysql_error(obj) << endl;
        return;
    }
    
    if (mysql_affected_rows(obj) == 0) {
            cout <<"Event '"<<event_name<<"' does not exist."<<endl;
            return;
        } 
    MYSQL_ROW row;
    if ((row = mysql_fetch_row(result))) {
        int available_seats = atoi(row[0]);
        int charge_per_student = atoi(row[1]);
        int total_revenue = charge_per_student * (300 - available_seats);
        cout << "Total Revenue for event '" << event_name << "' is Rs. " << total_revenue << endl;
    }

    mysql_free_result(result);
}

// Admin mode function
void admin() {
    while (true) {
        system("cls");
        int option;
        cout << "Welcome to Admin Mode" << endl;
        cout << "**********************************************************" << endl;
        cout << " 1. Add Event" << endl;
        cout << " 2. Delete Event" << endl;
        cout << " 3. Calculate Revenue for an Event" << endl;
        cout << " 4. Display the data of the student who have booked seats" << endl;
        cout << " 5. Exit"<<endl;
        cout << "Enter your choice: ";
        cin >> option;
        if(option==1){
        	get_admin();
		}
		else if(option==2){
			delete_event();
		}
		else if(option==3){
			calculate_revenue();
		}
		else if(option==4){
			const char* query = "SELECT * FROM student";
			display(query, 2);
		}
		else if(option==5){
			cout<<"Thank You!"<<endl;
			break;
		}
		else{
			cout<<"Wrong option Chosen! Please try again."<<endl;
		}
        system("pause");
    }
}

// Function to display all events

void check(const string& event_name, int &x) {
    // Step 1: Fetch current available seats for the event
    string query = "SELECT available_seats FROM event WHERE name = '" + event_name + "'";
    if (mysql_query(obj, query.c_str())) {
        cout << "Error: " << mysql_error(obj) << endl;
        x=-1;
        return;
    }

    MYSQL_RES* res = mysql_store_result(obj);
    if (!res) {
        cout << "Error: " << mysql_error(obj) << endl;
        x=-1;
        return;
    }

    MYSQL_ROW row;
    int available_seats = 0;

    if ((row = mysql_fetch_row(res))) {
        available_seats = atoi(row[0]);
    }
    else{
    	x=-1;
    	cout<<"You are accessing an event which does not exsist! Please try again"<<endl;
    	return;
	}

    mysql_free_result(res);

    // Step 2: Check if there are enough seats available
    if (available_seats >= x) {
        // Step 3: Update available seats in the database
        stringstream updateQuery;
        updateQuery << "UPDATE event SET available_seats = " << (available_seats - x) << " WHERE name = '" << event_name << "'";
        if (mysql_query(obj, updateQuery.str().c_str())) {
            cout << "Error updating available seats: " << mysql_error(obj) << endl;
            x=-1;
            return;
        }
    } else {
        cout << "Sorry! We do not have sufficient seats for you" << endl;
        x=-1;
    }
}


// Function to book tickets for an event
void book_tickets() {
    string event_name, name, id;
    int num_seats;

    cout << "Enter the name of the event: ";
    cin >> event_name;
    cout << "Enter your name: ";
    cin >> name;
    cout << "Enter your BITS ID: ";
    cin >> id;
    cout << "Enter the number of seats you want to book: ";
    cin >> num_seats;

    // Check if there are enough seats and update the database if booking is successful
    check(event_name, num_seats);
    if(num_seats==-1){
    	return;
	}
	
        Student student(event_name, name, id, num_seats);

        stringstream ss;
        ss << student.get_seats();
        string s_seats = ss.str();

        string insert_query = "INSERT INTO student (name, event_name, id, num) VALUES ('" +
            student.get_name() + "','" + student.get_event() + "','" + student.get_id() + "'," + s_seats + ")";

        if (mysql_query(obj, insert_query.c_str())) {
            cout << "Error: " << mysql_error(obj) << endl;
        }
        else {
            cout << "Booking done successfully" << endl;
        }
    }

// User mode function
void user() {
    while (true) {
        system("cls");
        int option;
        cout << "Welcome to User Mode" << endl;
        cout << "**********************************************************" << endl;
        cout << " 1. Display Events" << endl;
        cout << " 2. Book Tickets" << endl;
        cout << " 3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> option;

        if(option==1){
        	const char* query = "SELECT * FROM event";
        	display(query, 1);
		}
		else if(option==2){
			book_tickets();
		}
		else if(option==3){
			cout<<"Thank You!"<<endl;
			break;
		}
		else{
			cout<<"Wrong option chosen! Please try again"<<endl;
		}

        system("pause");
    }
}

int main() {
    // Initialize MySQL object
    obj = mysql_init(0);
    if (!obj) {
        cout << "Error initializing MySQL: " << mysql_error(obj) << endl;
        return 1;
    }

    // Connect to MySQL database
    if (!mysql_real_connect(obj, HOST, USER, PASS, DB, 3306, NULL, 0)) {
        cout << "Error connecting to database: " << mysql_error(obj) << endl;
        mysql_close(obj);
        return 1;
    }

    cout << "Logged IN" << endl;
    Sleep(1500);

    // Main menu loop
    while (true) {
        system("cls");
        int option;
        cout << "Welcome to BITS Pilani AUDI Ticket Booking System" << endl;
        cout << "**********************************************************" << endl;
        cout << "  1. Admin Mode" << endl;
        cout << "  2. User Mode" << endl;
        cout << "  3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> option;
        if(option==1){
        	admin();
		}
		else if(option==2){
			user();
		}
		else if(option==3){
			cout<<"Thank You"<<endl;
			break;
		}
		else{
			cout<<"Wrong option Chosen! Please try again."<<endl;
		}
        
    }

    mysql_close(obj);
    return 0;
}

