#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>

using namespace std;

class Flight {
private:
    string flight_number;
    string departure;
    string arrival;
    string status;
    int available_seats;

public:
    // Constructor
    Flight(string flight_num, string dep, string arr, string stat)
        : flight_number(flight_num), departure(dep), arrival(arr), status(stat), available_seats(60) {}

    // Display flight details
    void display() const {
        cout << "|| Flight Number: " << flight_number
             << " || Departure: " << departure
             << " || Arrival: " << arrival
             << " || Status: " << status
             << " || Available Seats: " << available_seats << " ||" << endl;
    }

    // Getter for flight number
    string getFlightNumber() const { return flight_number; }

    // Getter for departure
    string getDeparture() const { return departure; }

    // Getter for arrival
    string getArrival() const { return arrival; }

    // Check seat availability
    bool hasAvailableSeats() const { return available_seats > 0; }

    // Book a seat
    bool bookSeat() {
        if (available_seats > 0) {
            available_seats--;
            return true;
        }
        return false;
    }

    // Cancel a seat
    void cancelSeat() {
        if (available_seats < 60) {
            available_seats++;
        }
    }
};

class BookingSystem {
private:
    vector<Flight> flights;

    // Helper function to extract a value for a given key from a JSON-like string
    string extractValue(const string& data, const string& key) {
        size_t key_pos = data.find("\"" + key + "\"");
        if (key_pos == string::npos) return "N/A";

        size_t colon_pos = data.find(":", key_pos);
        size_t start_quote = data.find("\"", colon_pos + 1);
        size_t end_quote = data.find("\"", start_quote + 1);

        if (colon_pos == string::npos || start_quote == string::npos || end_quote == string::npos)
            return "N/A";

        return data.substr(start_quote + 1, end_quote - start_quote - 1);
    }

    // Parse flights from JSON-like output
    vector<Flight> parseFlights(const string& json_data) {
        vector<Flight> parsed_flights;

        size_t start = 0;
        while ((start = json_data.find("{", start)) != string::npos) {
            size_t end = json_data.find("}", start);
            if (end == string::npos) break;

            string flight_data = json_data.substr(start, end - start + 1);
            Flight flight(
                extractValue(flight_data, "flight_number"),
                extractValue(flight_data, "departure"),
                extractValue(flight_data, "arrival"),
                extractValue(flight_data, "status")
            );
            parsed_flights.push_back(flight);

            start = end + 1;
        }

        return parsed_flights;
    }

    // Fetch flights by invoking the Python script
    vector<Flight> fetchFlights(const string& source, const string& destination) {
        vector<Flight> fetched_flights;

        // Create the command to call the Python script and redirect output to a file
        string command = "python fetch_flights.py " + source + " " + destination + " > flights_output.json";

        // Execute the command
        int result = system(command.c_str());
        if (result != 0) {
            cerr << "Error: Python script execution failed.\n";
            return fetched_flights;
        }

        // Read the output from the file
        ifstream file("flights_output.json");
        if (!file) {
            cerr << "Error: Unable to open flights_output.json.\n";
            return fetched_flights;
        }

        stringstream buffer;
        buffer << file.rdbuf();
        file.close();

        // Parse the JSON-like content
        fetched_flights = parseFlights(buffer.str());
        return fetched_flights;
    }

public:
    // Initialize system with flights
    void initializeFlights(const string& source, const string& destination) {
        flights = fetchFlights(source, destination);
    }

    // Display available flights
    void displayFlights() const {
        if (flights.empty()) {
            cout << "|| No flights found for the given route. ||\n";
            return;
        }

        cout << "|| Available Flights: ||\n";
        for (size_t i = 0; i < flights.size(); ++i) {
            cout << "-- Flight " << i + 1 << " --\n";
            flights[i].display();
        }
    }

    // Book a flight
    Flight* bookFlight() {
        if (flights.empty()) {
            cout << "|| No flights available for booking. ||\n";
            return nullptr;
        }

        int choice;
        cout << "|| Enter the flight number to book: ";
        cin >> choice;

        if (choice > 0 && choice <= flights.size()) {
            if (flights[choice - 1].bookSeat()) {
                cout << "|| Booking successful for Flight: "
                     << flights[choice - 1].getFlightNumber() << " ||\n";
                return &flights[choice - 1];
            } else {
                cout << "|| No seats available on this flight. ||\n";
            }
        } else {
            cout << "|| Invalid choice. ||\n";
        }
        return nullptr;
    }

    // Cancel a flight
    void cancelBooking() {
        int choice;
        cout << "|| Enter the flight number to cancel: ";
        cin >> choice;

        if (choice > 0 && choice <= flights.size()) {
            flights[choice - 1].cancelSeat();
            cout << "|| Cancellation successful for Flight: "
                 << flights[choice - 1].getFlightNumber() << " ||\n";
        } else {
            cout << "|| Invalid flight number. ||\n";
        }
    }
};

class User {
private:
    string name;
    string email;
    int age;
    string mobile;

public:
    // Collect user details
    void collectUserDetails() {
        cout << R"(
             _____ _____        ______           _______     __
     /\   |_   _|  __ \      |  ____|   /\    / ____\ \   / /
    /  \    | | | |__) ______| |__     /  \  | (___  \ \_/ / 
   / /\ \   | | |  _  |______|  __|   / /\ \  \___ \  \   /  
  / ____ \ _| |_| | \ \      | |____ / ____ \ ____) |  | |   
 /_/    \_|_____|_|  \_\     |______/_/    \_|_____/   |_|   
                                                             
                                                              
)";
        cout << "\n|| Welcome to Air-Easy Ticket Booking System ||\n";
        cout << "|| Please enter your details. ||\n";
        cout << "|| Name: ";
        cin.ignore();
        getline(cin, name);  // Use getline to read full name with spaces
        cout << "|| Email: ";
        cin >> email;
        cout << "|| Age: ";
        cin >> age;
        cout << "|| Mobile: ";
        cin >> mobile;
    }

    // Display ticket - CORRECTED to show user's name
    void displayTicket(const Flight& flight) const {
        cout << R"(
  _______ _____ _____ _  ________ _______ 
 |__   __|_   _/ ____| |/ |  ____|__   __|
    | |    | || |    | ' /| |__     | |   
    | |    | || |    |  < |  __|    | |   
    | |   _| || |____| . \| |____   | |   
    |_|  |_____\_____|_|\_|______|  |_|   
                                          
                                          
                                                             
                                                              
)";
      
        cout << "|| Passenger Name: " << name << " ||\n";  // Explicitly show name now
        cout << "|| Email: " << email << " ||\n";
        cout << "|| Flight Number: " << flight.getFlightNumber() << " ||\n";
        cout << "|| Departure: " << flight.getDeparture() << " ||\n";
        cout << "|| Arrival: " << flight.getArrival() << " ||\n";
        cout << "**************************************\n";
    }
};

// Main program
int main() {
    try {
        User user;
        user.collectUserDetails();

        string source, destination;
        cout << "|| Enter source (IATA code): ";
        cin >> source;
        cout << "|| Enter destination (IATA code): ";
        cin >> destination;

        BookingSystem bookingSystem;
        bookingSystem.initializeFlights(source, destination);

        bookingSystem.displayFlights();
        
        int action;
        cout << "\n|| Select action: ||\n";
        cout << "1. Book a flight\n2. Cancel a flight\n";
        cout << "Enter choice: ";
        cin >> action;

        if (action == 1) {
            Flight* bookedFlight = bookingSystem.bookFlight();
            if (bookedFlight) {
                user.displayTicket(*bookedFlight);
            }
        } else if (action == 2) {
            bookingSystem.cancelBooking();
        } else {
            cout << "|| Invalid action. ||\n";
        }

    } catch (const exception& e) {
        cerr << "|| Error: " << e.what() << " ||\n";
        return 1;
    }

    return 0;
}