// COMSC-210 | Lab 19 | Movie Review System with External File Input
// Author: Chevin Jeon
// Date: 2025-10-05
//
// Goal:
//  - Create a Movie class that stores movie title and linked list of reviews
//  - Read review comments from external file
//  - Generate random ratings between 1.0-5.0
//  - Use container of Movie objects in main()
//  - Demonstrate with at least 4 movies, each with 3+ reviews
//
// Build (example):
//   g++ -std=c++11 -Wall -Wextra -pedantic -O2 lab19.cpp -o lab19
//
// ---------------------------------------------------------------------
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

// ReviewNode struct to store rating and review comment
struct ReviewNode {
    double rating;
    string comment;
    ReviewNode* next;
    
    // Constructor
    ReviewNode(double r, const string& c) : rating(r), comment(c), next(nullptr) {}
};

// Movie class with title and linked list of reviews
class Movie {
    private:
        string title;
        ReviewNode* head;
        int reviewCount;
        
        // Helper function to generate random rating between 1.0-5.0
        double generateRandomRating() {
            static random_device rd;
            static mt19937 gen(rd());
            static uniform_real_distribution<> dis(1.0, 5.0);
            return round(dis(gen) * 10.0) / 10.0; // Round to 1 decimal place
        }
    
    public:
        // Constructor
        Movie(const string& movieTitle) : title(movieTitle), head(nullptr), reviewCount(0) {}
        
        // Destructor to clean up memory
        ~Movie() {
            ReviewNode* current = head;
            while (current != nullptr) {
                ReviewNode* next = current->next;
                delete current;
                current = next;
            }
        }
        
        // Copy constructor (deep copy)
        Movie(const Movie& other) : title(other.title), head(nullptr), reviewCount(other.reviewCount) {
            if (other.head != nullptr) {
                head = new ReviewNode(other.head->rating, other.head->comment);
                ReviewNode* current = head;
                ReviewNode* otherCurrent = other.head->next;
                
                while (otherCurrent != nullptr) {
                    current->next = new ReviewNode(otherCurrent->rating, otherCurrent->comment);
                    current = current->next;
                    otherCurrent = otherCurrent->next;
                }
            }
        }
        
    // Assignment operator (deep copy)
    Movie& operator=(const Movie& other) {
        if (this != &other) {
            // Clean up existing data
            ReviewNode* current = head;
            while (current != nullptr) {
                ReviewNode* next = current->next;
                delete current;
                current = next;
            }
            
            // Copy new data
            title = other.title;
            reviewCount = other.reviewCount;
            head = nullptr;
            
            if (other.head != nullptr) {
                head = new ReviewNode(other.head->rating, other.head->comment);
                ReviewNode* current = head;
                ReviewNode* otherCurrent = other.head->next;
                
                while (otherCurrent != nullptr) {
                    current->next = new ReviewNode(otherCurrent->rating, otherCurrent->comment);
                    current = current->next;
                    otherCurrent = otherCurrent->next;
                }
            }
        }
        return *this;
    }