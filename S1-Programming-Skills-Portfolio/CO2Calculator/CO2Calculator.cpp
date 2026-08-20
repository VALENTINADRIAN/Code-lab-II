#include <iostream> // For cin and cout
#include <cctype>   // For toupper() to fix lowercase letters

using namespace std;

// Function to get the miles and check for errors
double getDistance() {
    double distance;
    
    // Loop keeps running if the user types letters or negative numbers
    while (cout << "How many miles did you drive? " && (!(cin >> distance) || distance < 0)) {
        cin.clear();            // Resets cin because it broke from bad input
        cin.ignore(10000, '\n'); // Removes the bad input text
        cout << "Error! Distance must be a positive number.\n";
    }
    return distance; // Sends the valid distance value back to main
}

// Function to get the fuel type and check if it is correct
char getFuel() {
    char fuel;
    
    // Loop keeps running until the input matches P or D
    while (cout << "Which fuel type did you use? (P for Petrol, D for Diesel): " && cin >> fuel) {
        fuel = toupper(fuel); // Changes lowercase to uppercase automatically
        
        // If the choice is valid, it returns the value and exits the function
        if (fuel == 'P' || fuel == 'D') return fuel;
        
        cout << "Error! Invalid selection.\n";
    }
    return 'P'; // Backup return statement just in case
}

int main() {
    // Shows the programme title
    cout << "=== CO2 Calculator ===\n";
    
    // Calls the functions above to get the data
    double distance = getDistance();
    char fuel = getFuel();
    
    // Inline shortcut to pick the CO2 number based on the fuel choice
    // If the fuel is P, 2.31 is used. Otherwise (Diesel), 2.68 is used
    double emissionFactor = (fuel == 'P') ? 2.31 : 2.68;
    
    // Formula = (Miles/100) x 9.66 gives litres used, then it's multiplied by the emission factor
    double totalEmissions = (distance / 100.0) * 9.66 * emissionFactor;
    
    // Forces output to show standard decimal points
    cout.setf(ios::fixed);
    // Sets the output to show exactly 2 decimal places
    cout.precision(2);     
    
    // Prints the final output
    cout << "Total CO2 released for your journey is: " << totalEmissions << " kg.\n";
    
    return 0; // End of programme
}
