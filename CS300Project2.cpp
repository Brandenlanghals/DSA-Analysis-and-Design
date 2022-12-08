//============================================================================
// Name        : CS300Project2.cpp
// Author      : Branden Langhals
// Version     : 1.0
// Description : CS-300 SNHU Project Two
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Defined classes for Course data and Nodes of the Binary Tree
class Course
{
public:
    string id;
    string name;
    vector<string> prereqs;

    Course(string id, string name)
    {
        this->id = id;
        this->name = name;
    }
    Course()
    {
    }
};
class Node
{
public:
    Node *left;
    Node *right;
    Course course;

    Node(Course course)
    {
        this->course = course;
        left = nullptr;
        right = nullptr;
    }
};

// Root Pointer Declaration
Node *root = nullptr;

// Search the Binary Tree and return the Course object
Course Search(string idToFind)
{
    Node *temp = root;
    while (temp != nullptr)
    {
        if (temp->course.id == idToFind)
        {
            return temp->course;
        }
        else if (temp->course.id < idToFind)
        {
            temp = temp->right;
        }
        else
        {
            temp = temp->left;
        }
    }
    // Course was not found return empty
    return Course();
}

// Print ID, Name, and Prerequisites to the console
void PrintCourseInfo(Course course)
{
    cout << "Course ID:     " << course.id << "\nCourse name:   " << course.name << endl;
    cout << "Prerequisites: ";
    for (int i = 0; i < course.prereqs.size() - 1; i++)
    {
        cout << course.prereqs[i] << ", ";
    }
    // This prints the last item.
    cout << course.prereqs[course.prereqs.size() - 1];
}

// Iterate through each Node in the Binary Tree and print the ID and Name Alphanumerically
void PrintAllCourses(Node *node)
{
    if (node == nullptr)
    {
        return;
    }
    PrintAllCourses(node->left);
    cout << node->course.id << ", " << node->course.name << endl;
    PrintAllCourses(node->right);
}

// Creates Course object, converts to Node, then inserts in binary tree at sorted location
void InsertCourse(string line, vector<string> courseIDs)
{
    // Split the string to parts vector
    vector<string> parts;
    int start = 0;
    int end = line.find(',');
    while (end != -1)
    {
        parts.push_back(line.substr(start, end - start));
        start = end + 1;
        end = line.find(',', start);
    }
    parts.push_back(line.substr(start, line.length() - start));

    // Create Course object with ID, name, and any Prerequisites if applicable
    Course temp = Course(parts[0], parts[1]);
    for (int i = 2; i < parts.size(); i++)
    {
        // Validate each Prerequisite
        for (int j = 0; j < courseIDs.size(); j++)
        {
            if (courseIDs[j] == parts[i])
            {
                // If a match is found add the Prerequisite to the Course object
                temp.prereqs.push_back(parts[i]);
            }
        }
    }

    // Create Node object from new Course object and insert as the root if the root is null
    Node *newNode = new Node(temp);
    Node *currNode = root;
    if (currNode == nullptr)
    {
        // If the root is null set the root to newNode and return.
        root = newNode;
        return;
    }

    // Root node not null find and insert into the correct location.
    while (currNode != nullptr)
    {
        if (newNode->course.id < currNode->course.id)
        {
            // Insert on left if null, else traverse left.
            if (currNode->left == nullptr)
            {
                currNode->left = newNode;
                return;
            }
            currNode = currNode->left;
        }
        else
        {
            // Insert on right if null, else traverse right.
            if (currNode->right == nullptr)
            {
                currNode->right = newNode;
                return;
            }
            currNode = currNode->right;
        }
    }
}

// Converts opened CSV file to Course objects and inserts them into the Binary Tree
void LoadCoursesFromFile()
{
    ifstream inStream;
    inStream.open("courses.csv");
    string line;
    vector<string> courseIDs;

    if (inStream.is_open())
    {
        // Add the course id to vector
        while (getline(inStream, line))
        {
            int end = line.find(',');
            if (end != -1)
            {
                courseIDs.push_back(line.substr(0, end));
            }
        }
        // Post traversal clear inStream and set to 0
        inStream.clear();
        inStream.seekg(0);

        // Iterate through each line and add the course to the tree
        while (getline(inStream, line))
        {
            if (line.find(','))
            {
                // If comma exists there are at least two items on the line (course ID and Name)
                InsertCourse(line, courseIDs);
            }
        }
        inStream.close();
    }
    else
    {
        cout << "Failed to open file." << endl;
    }
}

int main()
{
    cout << "Default data file: courses.csv";
    bool loaded = false;

    string userInput = "";
    while (userInput != "9")
    {
        cout << "Welcome to the course planner.\n\n"
             << "1: Load Data Structure\n"
             << "2: Print Course List\n"
             << "3: Print Course\n"
             << "9: Exit\n\n"
             << "What would you like to do?";
        cin >> userInput;
        cout << "\n";

        // Load Coursees
        if (userInput == "1")
        {
            if (!loaded)
            {
                LoadCoursesFromFile();
                loaded = true;
            }
            else
            {
                cout << "The Courses have already been loaded, please choose a different option." << endl;
            }
        }
        // Print Courses
        else if (userInput == "2")
        {
            PrintAllCourses(root);
        }
        // Locate and print single course
        else if (userInput == "3")
        {
            cout << "What course would you like to know about? ";
            cin >> userInput;
            cout << "\n";

            Course temp = Search(userInput);
            if (temp.id == "")
            {
                cout << "Course " << userInput << " was not found." << endl;
            }
            else
            {
                PrintCourseInfo(temp);
            }
        }
        // Exit
        else if (userInput == "9")
        {
            cout << "Thank you for using the course planner!";
            return 9;
        }
        // If entry is invalid
        else
        {
            cout << userInput << "Is not a valid option. \n";
            continue;
        }
    }
}
