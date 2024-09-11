#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm> 
#include <limits>

using namespace std;

class Student {
public:
    string name;
    string regNo;
    string branch;
    string cl;

    Student() = default;
    Student(string n, string r, string b, string c) : name(n), regNo(r), branch(b), cl(c) {}
};

class StudentDatabase {
private:
    vector<Student> students;

    void show(size_t index) const {
        const Student& s = students[index];
        cout << "Student " << index + 1 << " Details:\n";
        cout << "Name: " << s.name << "\n";
        cout << "Registration Number: " << s.regNo << "\n";
        cout << "Branch: " << s.branch << "\n";
        cout << "Class: " << s.cl << "\n\n";
    }

    bool isValidRegistrationNumber(const string& regNo) const {
        return !regNo.empty();
    }

public:
    void add() {
        int num;
        cout << "Enter number of students to add: ";
        cin >> num;
        cin.ignore(); 

        for (int i = 0; i < num; ++i) {
            string name, regNo, branch, cl;
            cout << "Enter details for student " << i + 1 << ":\n";
            cout << "Name: ";
            getline(cin, name);
            cout << "Registration Number: ";
            getline(cin, regNo);

            if (!isValidRegistrationNumber(regNo)) {
                cout << "Invalid Registration Number. Try again.\n";
                --i; 
                continue;
            }

            cout << "Branch: ";
            getline(cin, branch);
            cout << "Class: ";
            getline(cin, cl);
            students.emplace_back(name, regNo, branch, cl);
        }
    }

    void showAll() const {
        if (students.empty()) {
            cout << "No student records available.\n";
            return;
        }

        for (size_t i = 0; i < students.size(); ++i) {
            show(i);
        }
    }

    void search() const {
        int choice;
        cout << "Search by: 1. Registration Number 2. Name 3. Branch\n";
        cin >> choice;
        cin.ignore(); 

        string query;
        cout << "Enter search query: ";
        getline(cin, query);

        for (size_t i = 0; i < students.size(); ++i) {
            if ((choice == 1 && students[i].regNo == query) ||
                (choice == 2 && students[i].name == query) ||
                (choice == 3 && students[i].branch == query)) {
                show(i);
                return;
            }
        }
        cout << "No student found.\n";
    }

    void update() {
        string regNo;
        cout << "Enter Registration Number to update: ";
        cin >> regNo;
        cin.ignore(); 

        for (size_t i = 0; i < students.size(); ++i) {
            if (students[i].regNo == regNo) {
                string field;
                cout << "Which detail to update (name/branch/class)? ";
                cin >> field;
                cin.ignore(); 

                if (field == "name") {
                    cout << "Enter new name: ";
                    getline(cin, students[i].name);
                } else if (field == "branch") {
                    cout << "Enter new branch: ";
                    getline(cin, students[i].branch);
                } else if (field == "class") {
                    cout << "Enter new class: ";
                    getline(cin, students[i].cl);
                } else {
                    cout << "Invalid field. Please enter name, branch, or class.\n";
                    return;
                }

                cout << "Updated details:\n";
                show(i);
                return;
            }
        }
        cout << "No student found with Registration Number: " << regNo << endl;
    }

    void deleteStudent() {
        int choice;
        cout << "Enter 0 to delete all students or 1 to delete a specific student: ";
        cin >> choice;
        cin.ignore(); 

        if (choice == 0) {
            students.clear();
            cout << "All student records have been deleted.\n";
        } else if (choice == 1) {
            string regNo;
            cout << "Enter Registration Number of student to delete: ";
            cin >> regNo;
            cin.ignore(); 

            auto it = remove_if(students.begin(), students.end(),
                                [&regNo](const Student& s) { return s.regNo == regNo; });
            if (it != students.end()) {
                students.erase(it, students.end());
                cout << "Student with Registration Number " << regNo << " has been deleted.\n";
            } else {
                cout << "No student found with Registration Number: " << regNo << endl;
            }
        } else {
            cout << "Invalid choice. Please enter 0 or 1.\n";
        }
    }

    void sortByName() {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.name < b.name;
        });
        cout << "Students sorted by name.\n";
    }

    void filterByBranch() const {
        string branch;
        cout << "Enter branch to filter: ";
        getline(cin, branch);

        for (size_t i = 0; i < students.size(); ++i) {
            if (students[i].branch == branch) {
                show(i);
            }
        }
    }

    void saveToFile(const string& filename) const {
        try {
            ofstream file(filename);
            if (!file) throw runtime_error("Unable to open file for writing.");
            
            for (const auto& student : students) {
                file << "Name of the student is : " << student.name << '\n'
                     << "Registration Number of the student is : " << student.regNo << '\n'
                     << "Branch of the student is : " << student.branch << '\n'
                     << "Class of the student is : " << student.cl << '\n';
            }
            cout << "Data saved to " << filename << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void loadFromFile(const string& filename) {
        try {
            ifstream file(filename);
            if (!file) throw runtime_error("Unable to open file for reading.");
            
            students.clear();
            string name, regNo, branch, cl;
            while (getline(file, name) && getline(file, regNo) &&
                   getline(file, branch) && getline(file, cl)) {
                students.emplace_back(name, regNo, branch, cl);
            }
            cout << "Data loaded from " << filename << endl;
        } catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
        }
    }

    void displayMenu() const {
        cout << "\nStudent Database Menu:\n";
        cout << "1. Add Student\n";
        cout << "2. Show All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Save to File\n";
        cout << "7. Load from File\n";
        cout << "8. Sort by Name\n";
        cout << "9. Filter by Branch\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
    }
};

int main() {
    StudentDatabase db;
    db.loadFromFile("students.txt"); 

    while (true) {
        db.displayMenu();
        int choice;
        cin >> choice;
        cin.ignore(); 

        switch (choice) {
            case 1: db.add(); break;
            case 2: db.showAll(); break;
            case 3: db.search(); break;
            case 4: db.update(); break;
            case 5: db.deleteStudent(); break;
            case 6: db.saveToFile("students.txt"); break;
            case 7: db.loadFromFile("students.txt"); break;
            case 8: db.sortByName(); break;
            case 9: db.filterByBranch(); break;
            case 0: return 0; 
            default: cout << "Invalid choice. Please enter a number between 0 and 9.\n"; break;
        }
    }
}