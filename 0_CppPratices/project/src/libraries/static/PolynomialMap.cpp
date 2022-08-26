#include "PolynomialMap.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>

using namespace std;
#define EPSILON 1.0e-10	// zero double
PolynomialMap::PolynomialMap(const PolynomialMap& other) {
    // TODO
	m_Polynomial = other.m_Polynomial;
}

PolynomialMap::PolynomialMap(const string& file) {
    ReadFromFile(file);
}

PolynomialMap::PolynomialMap(const double* cof, const int* deg, int n) {
	// TODO
	for (int i = 0; i < n; i++)
	{
	coff(deg[i]) = cof[i];
	}
}

PolynomialMap::PolynomialMap(const vector<int>& deg, const vector<double>& cof) {
	assert(deg.size() == cof.size());
	// TODO
	for (size_t i = 0; i < deg.size(); i++)
	{
	coff(deg[i]) = cof[i];
	}
}

double PolynomialMap::coff(int i) const {
	// TODO
	//double不行
	auto temp = m_Polynomial.find(i);
	if (temp == m_Polynomial.end())
	{
		return 0.0;
	}
	return temp->second;
}

double& PolynomialMap::coff(int i) {
	// TODO
	double temp = m_Polynomial[i];
	return temp;
}

void PolynomialMap::compress() {
	// TODO
	//不安全，需要改进
	map<int, double> tmpPoly = m_Polynomial;
	m_Polynomial.clear();
	for (const auto& item : tmpPoly) {
		if (fabs(item.second) > EPSILON)
			coff(item.first) = item.second;
	}
}

PolynomialMap PolynomialMap::operator+(const PolynomialMap& right) const {
	// TODO
	PolynomialMap temp(right);
	for (const auto& item : m_Polynomial)
		temp.coff(item.first) += item.second;

	temp.compress();
	return temp;
}

PolynomialMap PolynomialMap::operator-(const PolynomialMap& right) const {
	// TODO
	PolynomialMap temp(right);
	for (const auto& item : m_Polynomial)
		temp.coff(item.first) -= item.second;

	temp.compress();
	return temp;
}

PolynomialMap PolynomialMap::operator*(const PolynomialMap& right) const {
	// TODO
	PolynomialMap temp;
	for (const auto& item1 : m_Polynomial) {
		for (const auto& item2 : right.m_Polynomial) {
			int deg = item1.first + item2.first;
			double cof = item1.second * item2.second;
			temp.coff(deg) += cof;
		}
	}
	return temp;
}

PolynomialMap& PolynomialMap::operator=(const PolynomialMap& right) {
	// TODO
	m_Polynomial = right.m_Polynomial;
	return *this;
}

void PolynomialMap::Print() const {
	auto itr = m_Polynomial.begin();
	if (itr == m_Polynomial.end()) {
		cout << "0" << endl;
		return;
	}
	for (; itr != m_Polynomial.end(); itr++) {
		if (itr != m_Polynomial.begin()) {
			cout << " ";
			if (itr->second > 0)
			{
			cout << "+";
			}
		}
		cout << itr->second;
		if (itr->first > 0)
		{
			cout << "x^" << itr->first;
		}
	}
	cout << endl;
}

bool PolynomialMap::ReadFromFile(const string& file) {
    m_Polynomial.clear();
	// TODO
	ifstream inp;
	inp.open(file.c_str());
	//cout << "文件打开失败";
	if (!inp.is_open()) {
		cout << "ERROR::PolynomialList::ReadFromFile:" << endl
			<< "\t" << "file [" << file << "] opens failed" << endl;
		return false;
	}
	char ch;
	int n;
	inp >> ch;
	inp >> n;
	for (int i = 0; i < n; i++) {
		int deg;
		double cof;
		inp >> deg;
		inp >> cof;
		coff(deg) = cof;
	}
	inp.close();
	return true;
}
