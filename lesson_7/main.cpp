#include <iostream>
#include "students.pb.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <numeric>

using namespace std;

class FullName {
        string m_last_name;
        string m_first_name;
        string m_patronymic;
        public:
        FullName() { }
        FullName(string last_name, string first_name, string patronymic)
        : m_last_name(last_name), m_first_name(first_name), m_patronymic(patronymic) { }
        FullName(const protobuff::FullName& full_name) {
            m_last_name = full_name.last_name();
            m_first_name = full_name.first_name();
            m_patronymic = full_name.patronymic();
        }
        void ConvertToSerializationType(protobuff::FullName *fn) {
            fn->set_first_name(m_first_name);
            fn->set_last_name(m_last_name);
            fn->set_patronymic(m_patronymic);
        }
        friend bool operator==(const FullName& fn1, const FullName& fn2) {
            return fn1.m_last_name == fn2.m_last_name && fn1.m_first_name == fn2.m_first_name && fn1.m_patronymic == fn2.m_patronymic;
        }
        friend ostream& operator<<(ostream& out, const FullName& fn) {
            out << fn.m_last_name << " " << fn.m_last_name << " " << fn.m_patronymic;
            return out;
        }
};

class IRepository {
    virtual void Open() = 0; // бинарная десериализация в файл
    virtual void Save() = 0; // бинарная сериализация в файл
};

class IMethods {
    virtual double GetAverageScore(const FullName& name) = 0;
    virtual string GetAllInfo(const FullName& name) = 0;
    virtual string GetAllInfo() = 0;
};

class Student {

    FullName m_fullName;
    vector<int> m_grades;
    int m_average_grade;

public:
    Student(const FullName &fullName, vector<int> grades, int average_grade)
            : m_fullName(fullName), m_grades(grades), m_average_grade(average_grade) { }
    Student(const protobuff::Student& student) {
        m_average_grade = student.average_grade();
        for (size_t i = 0; i < student.grades_size(); ++i) {
            m_grades.push_back(student.grades(i));
        }
        m_fullName = FullName(student.fullname());
    }

    void ConvertToSerializationType(protobuff::Student* st) {
        for_each(m_grades.begin(), m_grades.end(), [&st](int value) { st->add_grades(value); });
        st->set_average_grade(m_average_grade);
        protobuff::FullName* fn = st->mutable_fullname();
        m_fullName.ConvertToSerializationType(fn);
    }

    friend ostream& operator<<(ostream& out, const Student& st) {
        out << st.m_fullName << " ";
        copy(st.m_grades.begin(), st.m_grades.end(), ostream_iterator<int>(out, " "));
        return out;
    }

    const FullName& fullName() const { return m_fullName; }
    const vector<int>& grades() const { return m_grades; }
};

class StudentsGroup : public IRepository, public IMethods {
private:
    vector<Student> m_students;
    protobuff::StudentsGroup ConvertToSerializationType(vector<Student> students) {
        protobuff::StudentsGroup sg;
        for (auto student : students) {
            protobuff::Student* st = sg.add_students();
            student.ConvertToSerializationType(st);
        }
        return sg;
    }
    vector<Student> ConvertFromSerializationType(protobuff::StudentsGroup students) {
        vector<Student> sg;
        for (size_t i = 0; i < students.students_size(); i++) {
            protobuff::Student st = students.students(i);
            sg.push_back(Student(students.students(i)));
        }
        return sg;
    }
public:
    StudentsGroup() { }
    StudentsGroup(vector<Student> students)
            : m_students(students) { }
    void Open() override {
        protobuff::StudentsGroup s;
        ifstream in("students.bin", ios_base::binary);

        if (!s.ParseFromIstream(&in)) {
            //throw new exception("error with serialization");
        }

        m_students = ConvertFromSerializationType(s);
    }
    void Save() override {

        protobuff::StudentsGroup s = ConvertToSerializationType(m_students);
        ofstream out("students.bin", ios_base::binary);
        s.SerializeToOstream(&out);
    }

    friend ostream& operator<<(ostream& out, const StudentsGroup& sg) {
        copy(sg.m_students.begin(), sg.m_students.end(), ostream_iterator<Student>(out, "\n"));
        return out;
    }

    double GetAverageScore(const FullName& name) override {
        auto finded = find_if(m_students.begin(), m_students.end(), [&](const Student& st) {return name == st.fullName(); });
        if (finded != m_students.end()) {
            auto grades = finded->grades();
            return accumulate(grades.begin(), grades.end(), 0) / grades.size();
        }
        return 0;
    }
    string GetAllInfo(const FullName& name) override {
        auto finded = find_if(m_students.begin(), m_students.end(), [&](const Student& st) {return name == st.fullName(); });
        if (finded != m_students.end()) {
            ostringstream ss;
            ss << *finded;
            return ss.str();
        }
        return string();
    }
    string GetAllInfo() override {
        ostringstream ss;
        ss << *this;
        return ss.str();
    }
};


int main() {

    StudentsGroup group(
            vector<Student> {
                    Student(FullName("Gorbunova", "Anna", "Aleksandrovna"), vector<int>{ 5, 2, 8, 6, 4 }, 5),
                    Student(FullName("Rogova", "Viktoriia", "Matveevna"), vector<int>{ 9,3,10,6,7 }, 7),
                    Student(FullName("Grekov", "Mark", "Egorovich"), vector<int>{ 6, 10, 8, 9, 7 }, 8)
            });
    // serialization
    group.Save();

    // deserialization
    StudentsGroup group2;
    group2.Open();
    cout << group2.GetAverageScore(FullName("Rogova", "Viktoriia", "Matveevna")) << endl;
    cout << group2.GetAllInfo(FullName("Rogova", "Viktoriia", "Matveevna")) << endl;
    cout << group2.GetAllInfo() << endl;

    return 0;
}
