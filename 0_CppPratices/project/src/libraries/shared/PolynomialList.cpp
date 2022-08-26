#include "PolynomialList.h"
#include <cassert>
#include <iostream>
#include <fstream>
using namespace std;
#define EPSILON 1.0e-10	// zero double

PolynomialList::PolynomialList(const PolynomialList& other) {
    // TODO
    m_Polynomial = other.m_Polynomial;
}

PolynomialList::PolynomialList(const string& file) {
    // TODO
    ReadFromFile(file);
}

PolynomialList::PolynomialList(const double* cof, const int* deg, int n) {
    // TODO
    for (int i = 0; i < n; i++)
    {
        AddOneTerm(Term(deg[i], cof[i]));
    }
}

PolynomialList::PolynomialList(const vector<int>& deg, const vector<double>& cof) {
    // TODO
    assert(deg.size() == cof.size());
    for (size_t i = 0; i < deg.size(); i++)
    {
    AddOneTerm(Term(deg[i], cof[i]));
    } 
}

double PolynomialList::coff(int i) const {
    // TODO
    for (const Term& term : m_Polynomial) {
        if (term.deg < i)
        {
        break;
        }
        if (term.deg == i)
        {
        return term.cof;
        }
    }
    return 0.; // you should return a correct value
}

double& PolynomialList::coff(int i) {
    // TODO
    double temp= AddOneTerm(Term(i, 0)).cof;
    return temp; // you should return a correct value
}

void PolynomialList::compress() {
    // TODO
    /*double temp = m_Polynomial.begin();*/
    auto temp = m_Polynomial.begin();
    while (temp != m_Polynomial.end()) {
        //if (fabs((*temp).cof) < 0.0)
        if (fabs((*temp).cof) < EPSILON)
        {
        temp = m_Polynomial.erase(temp);
        }
        else
        {
        temp++;
        }
            
    }
}

PolynomialList PolynomialList::operator+(const PolynomialList& right) const {
    // TODO
    PolynomialList temp(*this);
    for (const auto& term : right.m_Polynomial)
    {
    temp.AddOneTerm(term);
    }
    temp.compress();
    return temp;
}

PolynomialList PolynomialList::operator-(const PolynomialList& right) const {
    // TODO
    PolynomialList temp(*(this));
    for (const auto& term : right.m_Polynomial)
    {
        temp.AddOneTerm(Term(term.deg, -term.cof));
    }
    temp.compress();
    return temp;
}

PolynomialList PolynomialList::operator*(const PolynomialList& right) const {
    // TODO
    PolynomialList result;
    for (auto & term1 : m_Polynomial) {
        for (const auto& term2 : right.m_Polynomial) {
            double cof = term1.cof * term2.cof;
            int deg = term1.deg + term2.deg;
            result.AddOneTerm(Term(deg, cof));
        }
    }
    result.compress();
    return result;
}

PolynomialList& PolynomialList::operator=(const PolynomialList& right) {
    // TODO
    m_Polynomial = right.m_Polynomial;
    //return *this;
    return *this;
}

void PolynomialList::Print() const {
    // TODO
    PolynomialList result;
    result.compress();
    auto temp = m_Polynomial.begin();
    if (temp == m_Polynomial.end()) {
        cout << "0" << endl;
        return;
    }
    for (; temp != m_Polynomial.end(); temp++) {
        if (temp != m_Polynomial.begin()) {
            cout << " ";
            if (temp->cof > 0)
                cout << "+";
        }
        cout << temp->cof;
        if (temp->deg > 0)
            cout << "x^" << temp->deg;
    }
    cout << endl;
}

bool PolynomialList::ReadFromFile(const string& file) {
    // TODO
    m_Polynomial.clear();
    ifstream inp;
    inp.open(file.c_str());
    if (!inp.is_open()) {
        //cout << "´ò¿ªÊ§°Ü" << endl;
        cout << "ERROR::PolynomialList::ReadFromFile:" << endl
            << "\t" << "file [" << file << "] opens failed" << endl;
        return false;
    }
    char ch;
    int n;
    inp >> ch;
    inp >> n;
    for (int i = 0; i < n; i++) {
        Term nd;
        inp >> nd.deg;
        inp >> nd.cof;

        AddOneTerm(nd);
    }
    inp.close();
    return true;
}

PolynomialList::Term& PolynomialList::AddOneTerm(const Term& term) {
    // TODO
    auto temp = m_Polynomial.begin();
    for (; temp != m_Polynomial.end(); temp++) {
        if (temp->deg == term.deg) {
            temp->cof += term.cof;
            return *temp;
        }

        if (temp->deg > term.deg)
            break;
    }
    return *m_Polynomial.insert(temp, term);
}
