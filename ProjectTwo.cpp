#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

// Course structure
struct Course {
    string courseId;
    string title;
    vector<string> prerequisites;
};

// BST node structure
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course c) {
        course = c;
        left = nullptr;
        right = nullptr;
    }
};

// Binary Search Tree
class BinarySearchTree {
private:
    Node* root;

    // Add node to BST
    void addNode(Node* node, Course course) {
        if (course.courseId < node->course.courseId) {
            if (node->left == nullptr) {
                node->left = new Node(course);
            } else {
                addNode(node->left, course);
            }
        } else {
            if (node->right == nullptr) {
                node->right = new Node(course);
            } else {
                addNode(node->right, course);
            }
        }
    }

    // Search helper
    Node* searchNode(Node* node, string courseId) {
        if (node == nullptr) return nullptr;

        if (node->course.courseId == courseId) {
            return node;
        }
        else if (courseId < node->course.courseId) {
            return searchNode(node->left, courseId);
        }
        else {
            return searchNode(node->right, courseId);
        }
    }

    // In-order traversal prints sorted courses
    void inOrder(Node* node) {
        if (node != nullptr) {
            inOrder(node->left);
            cout << node->course.courseId << ", " << node->course.title << endl;
            inOrder(node->right);
        }
    }

public:
    BinarySearchTree() { root = nullptr; }

    // Insert a course
    void Insert(Course course) {
        if (root == nullptr) {
            root = new Node(course);
        } else {
            addNode(root, course);
        }
    }

    // Search for a course
    Course Search(string courseId) {
        Node* result = searchNode(root, courseId);
        if (result != nullptr) {
            return result->course;
        }
        Course empty;
        empty.courseId = "";
        return empty;
    }

    // Print sorted course list
    void PrintInOrder() {
        inOrder(root);
    }
};

// Load courses from CSV into BST
void loadCourses(string filename, BinarySearchTree* bst) {
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;

    while (getline(file, line)) {
        if (line.size() == 0) continue;

        stringstream ss(line);
        vector<string> tokens;
        string token;

        // Split CSV line
        while (getline(ss, token, ',')) {
            // Trim whitespace
            token.erase(0, token.find_first_not_of(" \t\r\n"));
            token.erase(token.find_last_not_of(" \t\r\n") + 1);
            tokens.push_back(token);
        }

        if (tokens.size() < 2) continue;

        Course course;
        course.courseId = tokens[0];
        course.title = tokens[1];

        // Load prerequisites
        for (size_t i = 2; i < tokens.size(); i++) {
            if (!tokens[i].empty()) {
                course.prerequisites.push_back(tokens[i]);
            }
        }

        bst->Insert(course);
    }

    cout << "Courses loaded successfully." << endl;
}

// Print course info including prerequisite titles
void printCourseInfo(BinarySearchTree* bst, string courseId) {
    Course course = bst->Search(courseId);

    if (course.courseId == "") {
        cout << "Course not found." << endl;
        return;
    }

    cout << course.courseId << ", " << course.title << endl;

    if (course.prerequisites.size() == 0) {
        cout << "Prerequisites: None" << endl;
        return;
    }

    cout << "Prerequisites: ";

    // Print prerequisite ID + Title
    for (size_t i = 0; i < course.prerequisites.size(); i++) {
        string prereqId = course.prerequisites[i];
        Course prereqCourse = bst->Search(prereqId);

        cout << prereqId;

        if (prereqCourse.courseId != "") {
            cout << " (" << prereqCourse.title << ")";
        }

        if (i < course.prerequisites.size() - 1) {
            cout << ", ";
        }
    }

    cout << endl;
}

// Display menu
void displayMenu() {
    cout << endl;
    cout << "  1. Load Data Structure" << endl;
    cout << "  2. Print Course List" << endl;
    cout << "  3. Print Course" << endl;
    cout << "  9. Exit" << endl;
    cout << endl;
    cout << "What would you like to do? ";
}

// Main
int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    int choice = 0;
    string filename;
    string courseId;

    cout << "Welcome to the ABCU Course Planner!" << endl;

    while (choice != 9) {
        displayMenu();

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Invalid input. Try again." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                cout << "Enter file name: ";
                cin >> filename;
                loadCourses(filename, bst);
                break;

            case 2:
                cout << endl << "Here is the course list:" << endl;
                bst->PrintInOrder();
                break;

            case 3:
                cout << "Enter course ID: ";
                cin >> courseId;
                transform(courseId.begin(), courseId.end(), courseId.begin(), ::toupper);
                cout << endl;
                printCourseInfo(bst, courseId);
                break;

            case 9:
                cout << "Thank you for using the ABCU Course Planner!" << endl;
                break;

            default:
                cout << "Invalid option." << endl;
        }
    }

    delete bst;
    return 0;
}