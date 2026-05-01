/*#include <iostream>
using namespace std;

class Course;

class Student {
public:
    string name;
    Course* courses[2];
    int count;

    Student(string n) : name(n), count(0) {}

    void enroll(Course* c);
};

class Course {
public:
    string title;
    Student* students[2];
    int count;

    Course(string t) : title(t), count(0) {}

    void addStudent(Student* s) {
        students[count++] = s;
    }
};

void Student::enroll(Course* c) {
    courses[count++] = c;
    c->addStudent(this);
}
void show(){
    for(int i=0;i<count;i++){
        cout<<courses[i]
    }
}

int main() {
    Student s1("Ali"), s2("Sara");
    Course c1("OOP"), c2("DS");

    s1.enroll(&c1);
    s1.enroll(&c2);
    s2.enroll(&c1);
    return 0;
}
#include <iostream>
using namespace std;

class Base {
public:
    virtual void show() = 0;

    void display() {
        cout << "Base display" << endl;
    }
};

class Derived : public Base {
public:
    void show() {
        cout << "Derived show" << endl;
    }

    void display() {
        cout << "Derived display" << endl;
    }
};

int main() {
    Base* b;
    Derived d;

    b = &d;

    b->show();
    b->display();
}*/
#include <iostream>
using namespace std;

class Shape {
public:
    virtual void draw() = 0;
};

class Circle : public Shape {
public:
    void draw() {
        cout << "Circle draw" << endl;
    }
};

int main() {
    Circle c;
    Shape s = c;  // line X
    s.draw();
}