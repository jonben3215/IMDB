#include<iostream>
#include <vector>
#include <functional>
#include "MovieHashTable.hpp"
using namespace std;

// Constructor for MovieHashTable with default size
MovieHashTable::MovieHashTable() {
    // TODO 
    table_size = DEFAULT_HTABLE_CAPACITY;
    table = new MovieNode*[table_size];
    for(int i =0; i< table_size; i++)
        table[i] = NULL;
    n_collisions = 0;
}

// Constructor for MovieHashTable with given size
MovieHashTable::MovieHashTable(int s) {
    //TODO
    table_size = s;
    table = new MovieNode*[table_size];
    for(int i =0; i< table_size; i++)
        table[i] = NULL;
}

// Destructor for MovieHashTable that deletes all nodes in the hash table
MovieHashTable::~MovieHashTable() {
    // TODO
    MovieNode * curr;
    MovieNode * prev; 
    for(int i = 0; i < table_size; i++)
    {
       curr = table[i];
       while(curr!= NULL)
       {
           prev = curr;
           curr = curr->next;
           delete prev;
       }
    }
}

// Hash function for MovieHashTable that returns an index in the hash table for a given movie title.
// Students must use their identikey to come up with a creative hash function that minimizes collisions
// for the given IMDB-Movie database to ensure efficient insertion and retrieval of movie nodes.
int MovieHashTable::hash(string title) {
    // TODO
    int hash_value = 0;
    for (char c : title) {
        hash_value = hash_value * 31 + c;
    }
    return abs(hash_value) % table_size;

   
}

// Inserts a movie node into the hash table with the specified title
void MovieHashTable::insert(string title, MovieNode* movie) {
    // TODO
    int hash_index = hash(title);
    if(table[hash_index] == NULL)
    {
        movie->title = title;
        table[hash_index] = movie;
    }
    else
    {
        MovieNode *curr = table[hash_index];
        while(curr->next!= NULL)
            curr = curr->next;
        curr->next = movie;
    }
}

// Searches for a node in the hash table with the specified title
MovieNode* MovieHashTable::search(string title) {
    // TODO
    for(int i =0; i< table_size; i++)
    {
        MovieNode * curr = table[i];
        while(curr != NULL)
        {
            if(curr -> title == title)
                return curr; //If the title is found then return the pointer
            curr = curr -> next;
        }
    }
    return NULL; // If the title does not exist then return NULL
}

// Returns the number of collisions that have occurred during insertion into the hash table
int MovieHashTable::getCollisions() {
    // TODO
    int counter = 0;
    for(int i = 0; i < table_size; i++)
    {
        MovieNode * curr = table[i];
        while(curr != NULL)
        {
            counter++;
            curr = curr -> next;
        }
    }
    return counter;

}

// Increments the number of collisions that have occurred during insertion into the hash table
void MovieHashTable::setCollisions() {
    // TODO
    int counter = getCollisions();
}
