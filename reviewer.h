#ifndef REVIEWER_H
#define REVIEWER_H

#include <QString>

class Reviewer
{
public:
    Reviewer();
    Reviewer(int id, const QString& nom, const QString& email);

    // Getters
    int getId() const { return id; }
    QString getNom() const { return nom; }
    QString getEmail() const { return email; }

    // Setters
    void setId(int i) { id = i; }
    void setNom(const QString& n) { nom = n; }
    void setEmail(const QString& e) { email = e; }

private:
    int id;
    QString nom;
    QString email;
};

#endif // REVIEWER_H
