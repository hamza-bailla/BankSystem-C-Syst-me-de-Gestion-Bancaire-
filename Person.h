#pragma once
#include <string>
using namespace std;

class Person {
protected:
    string firstName;
    string lastName;
    string phone;
    string email;

public:
    // Constructeur par défaut (nécessaire pour Client()=default)
    Person() = default;

    // Constructeur principal (LN, FN, PH, EM)
    Person(const string& ln, const string& fn, const string& ph, const string& em)
        : lastName(ln), firstName(fn), phone(ph), email(em) {
    }

    // Getters et Setters rendus PUBLICS (pour résoudre C2039)
    const string& getLastName() const { return lastName; }
    const string& getFirstName() const { return firstName; }
    const string& getPhone() const { return phone; }
    const string& getEmail() const { return email; }

    void setLastName(const string& v) { lastName = v; }
    void setFirstName(const string& v) { firstName = v; }
    void setPhone(const string& v) { phone = v; }
    void setEmail(const string& v) { email = v; }
};
