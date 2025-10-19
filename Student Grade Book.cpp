#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Student {
    string id;
    string name;
    vector<double> grades; // aligned with subjects order
};

// --- Function Prototypes ---
void loadStudents(vector<Student>& students);
void saveStudents(const vector<Student>& students);
void loadSubjects(vector<string>& subjects);
void saveSubjects(const vector<string>& subjects);
void addStudent(vector<Student>& students, const vector<string>& subjects);
void addGrades(vector<Student>& students, const vector<string>& subjects);
void displayGrades(const vector<Student>& students, const vector<string>& subjects);
void calculateAverage(const vector<Student>& students, const vector<string>& subjects);
void addSubject(vector<string>& subjects);
void clearScreen();
void showMenu();

// --- Helper Functions ---
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void showMenu() {
    cout << "========== STUDENT GRADE MANAGER ==========\n";
    cout << "1. Add Student\n";
    cout << "2. Add Grades\n";
    cout << "3. Display Grades\n";
    cout << "4. Calculate Average\n";
    cout << "5. Add Subject\n";
    cout << "6. Save and Exit\n";
    cout << "===========================================\n";
    cout << "Enter choice: ";
}

void loadStudents(vector<Student>& students) {
    ifstream file("students.txt");
    if (!file) return;

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        Student s;
        string gradesPart;
        getline(ss, s.id, ',');
        getline(ss, s.name, ',');
        getline(ss, gradesPart);

        stringstream gs(gradesPart);
        string g;
        while (getline(gs, g, ' ')) {
            if (!g.empty())
                s.grades.push_back(stod(g));
        }
        students.push_back(s);
    }
    file.close();
}

void saveStudents(const vector<Student>& students) {
    ofstream file("students.txt");
    for (const auto& s : students) {
        file << s.id << "," << s.name << ",";
        for (double g : s.grades)
            file << g << " ";
        file << "\n";
    }
    file.close();
}

void loadSubjects(vector<string>& subjects) {
    ifstream file("subjects.txt");
    if (!file) {
        cerr << " subjects.txt not found. Creating new file...\n";
        ofstream newFile("subjects.txt");
        newFile.close();
        return;
    }

    string subject;
    while (getline(file, subject)) {
        if (!subject.empty())
            subjects.push_back(subject);
    }
    file.close();
}

void saveSubjects(const vector<string>& subjects) {
    ofstream file("subjects.txt");
    for (const auto& subj : subjects)
        file << subj << "\n";
    file.close();
}

void addSubject(vector<string>& subjects) {
    cin.ignore();
    string newSubj;
    cout << "Enter subject name: ";
    getline(cin, newSubj);

    // Check duplicates (case-insensitive)
    string lowerSubj = newSubj;
    transform(lowerSubj.begin(), lowerSubj.end(), lowerSubj.begin(), ::tolower);
    for (const auto& s : subjects) {
        string lowerS = s;
        transform(lowerS.begin(), lowerS.end(), lowerS.begin(), ::tolower);
        if (lowerS == lowerSubj) {
            cout << "❌ Subject already exists.\n";
            return;
        }
    }

    subjects.push_back(newSubj);
    saveSubjects(subjects);
    cout << " Subject added successfully!\n";
}

void addStudent(vector<Student>& students, const vector<string>& subjects) {
    Student s;
    cout << "Enter student ID: ";
    cin >> s.id;
    cin.ignore();
    cout << "Enter student name: ";
    getline(cin, s.name);

    // initialize empty grades
    s.grades = vector<double>(subjects.size(), 0.0);

    students.push_back(s);
    cout << " Student added successfully!\n";
}

void addGrades(vector<Student>& students, const vector<string>& subjects) {
    if (students.empty()) {
        cout << "️ No students available.\n";
        return;
    }
    if (subjects.empty()) {
        cout << " No subjects found. Please add subjects first.\n";
        return;
    }

    string id;
    cout << "Enter student ID: ";
    cin >> id;

    for (auto& s : students) {
        if (s.id == id) {
            cout << "\nAdding grades for " << s.name << " (" << s.id << ")\n";
            s.grades.clear();
            for (const auto& subj : subjects) {
                double mark;
                while (true) {
                    cout << subj << ": ";
                    cin >> mark;

                    // Validate grade input
                    if (cin.fail()) {
                        cin.clear();
                        cin.ignore(10000, '\n');
                        cout << "❌ Invalid input. Please enter a number between 0 and 100.\n";
                    } else if (mark < 0 || mark > 100) {
                        cout << "❌ Grade must be between 0 and 100. Please try again.\n";
                    } else {
                        break; // Valid input
                    }
                }
                s.grades.push_back(mark);
            }
            cout << " Grades added successfully!\n";
            return;
        }
    }
    cout << " Student not found.\n";
}

void displayGrades(const vector<Student>& students, const vector<string>& subjects) {
    if (students.empty()) {
        cout << " No student data to display.\n";
        return;
    }

    cout << left << setw(10) << "ID" << setw(20) << "Name";
    for (const auto& subj : subjects)
        cout << setw(10) << subj;
    cout << "\n-------------------------------------------------------------\n";

    for (const auto& s : students) {
        cout << left << setw(10) << s.id << setw(20) << s.name;
        for (size_t i = 0; i < subjects.size(); ++i) {
            if (i < s.grades.size())
                cout << setw(10) << s.grades[i];
            else
                cout << setw(10) << "-";
        }
        cout << "\n";
    }
}

void calculateAverage(const vector<Student>& students, const vector<string>& subjects) {
    if (students.empty()) {
        cout << " No student data available.\n";
        return;
    }

    for (const auto& s : students) {
        if (s.grades.empty()) {
            cout << s.name << " (" << s.id << "): No grades available.\n";
            continue;
        }
        double sum = 0;
        for (double g : s.grades)
            sum += g;
        double avg = sum / s.grades.size();
        cout << s.name << " (" << s.id << ") -> Average: " << fixed << setprecision(2) << avg << "\n";
    }
}

// --- Main ---
int main() {
    vector<Student> students;
    vector<string> subjects;

    loadSubjects(subjects);
    loadStudents(students);

    int choice;
    bool running = true;

    while (running) {
        clearScreen();
        showMenu();
        cin >> choice;
        clearScreen();

        switch (choice) {
            case 1: addStudent(students, subjects); break;
            case 2: addGrades(students, subjects); break;
            case 3: displayGrades(students, subjects); break;
            case 4: calculateAverage(students, subjects); break;
            case 5: addSubject(subjects); break;
            case 6:
                saveStudents(students);
                saveSubjects(subjects);
                cout << " Data saved successfully. Exiting...\n";
                running = false;
                break;
            default:
                cout << " Invalid choice. Try again.\n";
        }

        if (running) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
            cin.get();
        }
    }

    return 0;
}
