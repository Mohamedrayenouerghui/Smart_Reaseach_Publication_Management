#include "headers/reviewer.h"

Reviewer::Reviewer()
    : id(0), nom(""), email("")
{
}

Reviewer::Reviewer(int i, const QString& n, const QString& e)
    : id(i), nom(n), email(e)
{
}
