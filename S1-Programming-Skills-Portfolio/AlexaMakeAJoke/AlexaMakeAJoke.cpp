#include <iostream> //For cin and cout
#include <fstream> // For reading and loading text data from randomJokes.txt
#include <vector> // For creating dynamic lists
#include <string> // For using and manipulating text blocks
#include <random> // Modern random library tool

using namespace std;

// Structure to store the joke parts
struct Joke {
    string setup;
    string punchline;
};

int main() {
    // Set up the random engine 
    random_device rd;
    mt19937 gen(rd());
    
    // Open the text file containing the jokes
    ifstream jokeFile("randomJokes.txt");
    
    // Check if the file failed to open properly
    if (!jokeFile) {
        cout << "Error! Could not open randomJokes.txt\n";
        return 1; // Stop the programme with an error
    }

    // Create a vector list to hold all the individual joke objects
    vector<Joke> jokes;
    string line;

    // Loop through the text file line by line until the end is reached
    while (getline(jokeFile, line)) {
        // Find the position of the question mark symbol
        size_t delimiterPos = line.find('?');
        
        // If a question mark was found on this line
        if (delimiterPos != string::npos) {
            Joke tempJoke;
            
            // Cut the string from the start up to the question mark for the setup
            tempJoke.setup = line.substr(0, delimiterPos + 1);
            
            // Cut the rest of the string after the question mark for the punchline
            tempJoke.punchline = line.substr(delimiterPos + 1);
            
            // Save this completed joke into the main vector list
            jokes.push_back(tempJoke);
        }
    }
    // Close the text file because reading is finished
    jokeFile.close();

    // Make sure the list actually contains jokes before moving forward
    if (jokes.empty()) {
        cout << "Error! No jokes found in the file.\n";
        return 1; // Stop the programme because there is nothing to display
    }

    // Set up a number generator range that limits picks between zero and the last index slot
    uniform_int_distribution<size_t> distr(0, jokes.size() - 1);

    string userInput;
    
    // Run an infinite loop to handle user prompts repeatedly
    while (true) {
        cout << "Enter prompt ('Alexa, tell me a Joke' or 'quit'): ";
        getline(cin, userInput); // Read the full line of text entered by the user

        // Exit the entire loop if the user types 'quit'
        if (userInput == "quit") {
            break;
        } 
        // Run the joke routine if the user inputs the exact phrase required
        else if (userInput == "Alexa, tell me a joke") {
            size_t index = distr(gen);

            // Print out the setup part of the chosen joke
            cout << "\n" << jokes[index].setup << "\n";
            cout << "[Press Enter]";
            
            // Empty string used to pause execution until enter is clicked
            string waitKey;
            getline(cin, waitKey);

            // Print out the matching punchline part
            cout << jokes[index].punchline << "\n\n";
        } 
        // Handle incorrect inputs by displaying a helpful warning message
        else {
            cout << "Invalid prompt. Please use: \"Alexa, tell me a joke\"\n\n";
        }
    }

    return 0; // End of programme
}
