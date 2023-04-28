#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"

using namespace std;

// Function prototypes
MovieNode* parseMovieLine(string line);
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList);
void display_menu();


// Main function
int main(int argc, char* argv[]) {
    // TODO
    if(argc != 4)
    {
        cout  << "Invalid number of arguments." << endl;
        cout << "Usage: ./<program name> <csv file> <hashTable size> <skipList size>" << endl;
        return -1;
    }

    int Table_Size = atoi(argv[2]);
    int Skip_Size = atoi(argv[3]);
    MovieHashTable movieTable(Table_Size);
    DirectorSkipList directList(Skip_Size, DEFAULT_LEVELS);
    readMovieCSV(argv[1], movieTable, directList);
    //directList.prettyPrint();
    int coll = movieTable.getCollisions();
    cout << "Collisions: " << coll << endl;


    string line;
    int Choice = 0;

    while(Choice != 5)
    {
        
        display_menu();
        cout << "Enter an Option: ";
        cin >> Choice;

        switch(Choice)
        {
            case 1:
            {
                MovieNode* movie;
                DirectorSLNode* direct;
                string movieName;
                cout << "Enter a movie name: ";
                cin.ignore(); // Ignore the newline character from previous input
                getline(cin, movieName);
                movie = movieTable.search(movieName);
                if (movie == nullptr) 
                {
                    cout << "Movie not found." << endl;
                    break;
                }
                
                 else 
                 {
                   direct = directList.search(movie->director);
                   if(direct == NULL)
                   {
                    cout << "Director Not found" << endl;
                    break;
                   }
                   else
                    cout << "The director of " << movie->title << " is " << direct->director << endl;
                }
                cout << endl;
                break;
            }
            case 2:
            {
                MovieNode * movie;
                DirectorSLNode * direct;
                int num_movies = 0;
                string input;
                cout << "Enter director name: "; 
                getline(cin.ignore(), input);
                direct = directList.search(input);
                cout << input << " directed " << direct->movies.size() << " movies." << endl;
                cout << endl;
                break;
            }
            case 3:
            {
                MovieNode * movie;
                DirectorSLNode * direct;
                string input;
                cout << "Enter a movie name: ";
                getline(cin.ignore(), input);
                movie = movieTable.search(input);
                cout << "Summary: " << movie->title << " is a " << movie -> year;
                cout << " ( " << movie -> genre << " )" << " film featuring " << movie -> actors << "." << endl;
                cout << "Plot: " << movie->description << endl;
                break;
            }
            case 4:
            {
                MovieNode * movie;
                DirectorSLNode * direct;
                int num_movies = 0;
                string input;

                cout << "Enter director name: "; 
                getline(cin.ignore(), input);
                direct = directList.search(input);
                cout << input << " directed the following movies:" << endl;
                if(direct == NULL)
                {
                    cout << "Director Not Found" << endl;
                    break;
                }
                for(int i = 0; i < (direct->movies.size()); i++)
                {
                    cout << i << ": " << direct->movies[i] -> title << endl;
                }
                cout << endl;
                break;
            }
            default:
            {
                movieTable.~MovieHashTable();
                directList.~DirectorSkipList();
                break;
            }
        }
    }
}

MovieNode* parseMovieLine(string line) {
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '\"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i+1] == '\"') {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12) {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode* movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}
// Function to read a CSV file into a vector of MovieNode objects
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList) {
    // TODO
ifstream infile(filename);
    if (!infile.is_open()) {
        cerr << "Error: Unable to open file \"" << filename << "\"" << endl;
        return;
    }

    string line;
    int lineNum = 0;
    while (getline(infile, line)) 
    {
        lineNum++;
        if (lineNum == 1)  // skip the first line
            continue;
        
        MovieNode* movie = parseMovieLine(line);
        if (movie != nullptr) 
        {
            movieTable.insert(movie->title, movie);
            directorList.insert(movie->director, movie);
        }
    }

    infile.close();
}

// Function to display the menu options
void display_menu() {
    cout << "------------MENU------------" << endl;
    cout << "1. Find the director of a movie" << endl;
    cout << "2. Find the number of movies by a director" << endl;
    cout << "3. Find the description of a movie" << endl;
    cout << "4. List the movies by a director" << endl;
    cout << "5. Quit" << endl;
}
