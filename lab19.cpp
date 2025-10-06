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
    
    // Add review to head of linked list
    void addReview(const string& comment) {
        double rating = generateRandomRating();
        ReviewNode* newNode = new ReviewNode(rating, comment);
        newNode->next = head;
        head = newNode;
        reviewCount++;
    }
    
    // Display movie title and all reviews
    void displayReviews() const {
        cout << "\n=== " << title << " ===" << endl;
        
        if (head == nullptr) {
            cout << "No reviews available." << endl;
            return;
        }
        
        ReviewNode* current = head;
        double totalRating = 0.0;
        int reviewNumber = 1;
        
        while (current != nullptr) {
            cout << "Review #" << reviewNumber << ": " 
                 << fixed << setprecision(1) << current->rating 
                 << " - " << current->comment << endl;
            
            totalRating += current->rating;
            current = current->next;
            reviewNumber++;
        }
        
        double average = totalRating / reviewCount;
        cout << "Average Rating: " << fixed << setprecision(2) << average << endl;
        cout << "Total Reviews: " << reviewCount << endl;
    }
    
    // Get movie title
    string getTitle() const {
        return title;
    }
    
    // Get number of reviews
    int getReviewCount() const {
        return reviewCount;
    }
};

// Function to read review comments from file
vector<string> readReviewComments(const string& filename) {
    vector<string> comments;
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return comments;
    }
    
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            comments.push_back(line);
        }
    }
    
    file.close();
    return comments;
}

int main() {
    // Seed random number generator
    srand(static_cast<unsigned>(time(nullptr)));
    
    // Read review comments from file
    vector<string> reviewComments = readReviewComments("reviews.txt");
    
    if (reviewComments.empty()) {
        cerr << "Error: No review comments found in file." << endl;
        return 1;
    }
    
    cout << "=== Movie Review System ===" << endl;
    cout << "Loaded " << reviewComments.size() << " review comments from file." << endl;
    
    // Create container of Movie objects using vector
    vector<Movie> movies;
    
    // Add movies to the container
    movies.push_back(Movie("The Matrix"));
    movies.push_back(Movie("Inception"));
    movies.push_back(Movie("Interstellar"));
    movies.push_back(Movie("Blade Runner 2049"));
    
    // Add reviews to each movie (at least 3 reviews per movie)
    for (int i = 0; i < 4; i++) {
        // Add 4-6 random reviews to each movie
        int numReviews = 4 + (rand() % 3); // Random number between 4-6
        
        for (int j = 0; j < numReviews; j++) {
            // Select random comment from the file
            int commentIndex = rand() % reviewComments.size();
            movies[i].addReview(reviewComments[commentIndex]);
        }
    }
    
    // Display all movies and their reviews
    cout << "\n=== MOVIE REVIEWS SUMMARY ===" << endl;
    for (const auto& movie : movies) {
        movie.displayReviews();
    }
    
    // Display overall statistics
    cout << "\n=== OVERALL STATISTICS ===" << endl;
    int totalMovies = movies.size();
    int totalReviews = 0;
    
    for (const auto& movie : movies) {
        totalReviews += movie.getReviewCount();
    }
    
    cout << "Total Movies: " << totalMovies << endl;
    cout << "Total Reviews: " << totalReviews << endl;
    cout << "Average Reviews per Movie: " << fixed << setprecision(1) 
         << static_cast<double>(totalReviews) / totalMovies << endl;
    
    return 0;
}
