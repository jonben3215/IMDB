#include<iostream>
#include <vector>
#include "DirectorSkipList.hpp"
using namespace std;

// Constructor for DirectorSkipList with default capacity and levels
DirectorSkipList::DirectorSkipList() {
    
    capacity = DEFAULT_CAPACITY;
    levels = DEFAULT_LEVELS;
    head = NULL;
    size  = 0;
}

// Constructor for DirectorSkipList with given capacity and levels
DirectorSkipList::DirectorSkipList(int _cap, int _levels) {
    // TODO
    capacity = _cap;
    levels = _levels;
    head = new DirectorSLNode("", levels);
    for (int i = 0; i < levels; i++) {
        head->next[i] = NULL;
    }
    
}

// Destructor for DirectorSkipList that deletes all nodes in the skip list (but not the MovieNodes, which are shared with other data structures)
DirectorSkipList::~DirectorSkipList() {
    // TODO
    DirectorSLNode * prev;
    DirectorSLNode * curr = head;
    while (curr != NULL)
    {
        prev = curr;
        curr = curr->next[0];
        delete prev;
    }
    head = NULL;
}

void DirectorSkipList::insert(string director, MovieNode* _movie) {
    // Search for existing node for director
    DirectorSLNode* curr = head; // Start at head
    DirectorSLNode* update[levels]; // Create an array to hold update nodes for each level

    // Traverse down the levels to find the correct position to insert the new node
    for (int i = levels - 1; i >= 0; i--) {
        // While loop to find the correct position in the current level
        while (curr->next[i] != nullptr && curr->next[i]->director < director) {
            curr = curr->next[i]; // Move to the next node in this level
        }
        update[i] = curr; // Keep track of the last node visited in this level
    }

    // Move to the top level of the skip list
    curr = curr->next[0];

    // If the director already exists in the list, add the movie to the existing node
    if (curr != nullptr && curr->director == director) {
        curr->movies.push_back(_movie);
    } else {
        // If the director doesn't exist, create a new node and insert it into the skip list
        int n_levels = 1;
        // Randomly choose the number of levels for the new node
        while (n_levels < levels && rand() % 2 == 0) {
            n_levels++;
        }
        DirectorSLNode* new_node = new DirectorSLNode(director, n_levels);
        new_node->movies.push_back(_movie);

        // Link the new node to the skip list at each level
        for (int i = 0; i < n_levels; i++) {
            new_node->next[i] = update[i]->next[i];
            update[i]->next[i] = new_node;
        }

        // If the new node has more levels than any existing node, update the head nodes accordingly
        if (n_levels > levels) {
            for (int i = levels; i < n_levels; i++) {
                head->next[i] = new_node;
            }
            levels = n_levels;
        }
        size++; // Increment the size of the skip list
    }
}



// Searches for a node in the skip list with the specified director
DirectorSLNode *DirectorSkipList::search(string director) {
    // TODO
    DirectorSLNode * curr = head;
    while(curr != NULL)
    {
        if(curr -> director == director)
            return curr;
        curr = curr ->next[0];
    }
    return NULL;
}

// Pretty-prints the skip list
void DirectorSkipList::prettyPrint() {
    // Traverse all nodes at the highest level
    DirectorSLNode* curr = head;
    if(curr == NULL)
    {
        cout << "The skip list is empty!" << endl;
        return;
    }
    string prev_director = "";
    cout << "Level " << levels - 1 << ": ";
    while (curr != NULL) {
        if (curr->next[levels - 1] == NULL)
            cout << "NULL" << "\t";
        else if (curr->director != prev_director) {
            cout << curr->director << "\t";
            prev_director = curr->director;
        }
        curr = curr->next[levels - 1];
    }
    cout << endl;

    // Traverse all nodes at the remaining levels
    for (int i = levels - 2; i >= 0; i--) {
        curr = head->next[i];
        cout << "Level " << i << ": ";
        while (curr != NULL && curr->next[i] != NULL) { // Check for null pointer here
            string curr_director = curr->director;
            if (curr_director != prev_director) {
                cout << curr_director << "\t";
                prev_director = curr_director;
            }
            curr = curr->next[i];
        }
        cout << endl;
        prev_director = "";
    }
}


