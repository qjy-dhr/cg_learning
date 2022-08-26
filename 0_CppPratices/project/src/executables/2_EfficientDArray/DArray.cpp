// implementation of class DArray
#include "DArray.h"
#include <iostream>
using namespace std;
// default constructor
DArray::DArray() {
	Init();
}

// set an array with default values
DArray::DArray(int nSize, double dValue) {
	//TODO
	m_pData = new double[nSize];
	m_nSize = nSize;
	m_nMax = nSize;
	for (int i = 0; i < nSize; i++)
	{
		m_pData[i] = dValue;
	}
}

DArray::DArray(const DArray& arr) {
	//TODO
	m_pData = new double[arr.m_nSize];
	m_nSize = arr.m_nSize;
	m_nMax = arr.m_nSize;
	for (int i = 0; i < arr.m_nSize; i++)
	{
		m_pData[i] = arr.m_pData[i];
	}
}

// deconstructor
DArray::~DArray() {
	Free();
}

// display the elements of the array
void DArray::Print() const {
	//TODO
	for (int i = 0; i < m_nSize; i++)
	{
		cout << " " << GetAt(i);
	}
	cout << endl;
}

// initilize the array
void DArray::Init() {
	//TODO
	m_pData = nullptr;
	m_nMax = 0;
	m_nSize = 0;
}

// free the array
void DArray::Free() {
	//TODO
	delete[] m_pData;
	m_nSize = 0;
	m_pData = nullptr;
	m_nMax = 0;
}

// get the size of the array
int DArray::GetSize() const {
	//TODO
	return m_nSize;
}
//为什么要反转？
void DArray::Reserve(int nSize) {
	if (m_nMax >= nSize)
		return;

	while (m_nMax < nSize)
		m_nMax = m_nMax == 0 ? 1 : 2 * m_nMax;

	double* pData = new double[m_nMax];
	memcpy(pData, m_pData, m_nSize * sizeof(double));

	delete[] m_pData;
	m_pData = pData;
}

// set the size of the array
void DArray::SetSize(int nSize) {
	//TODO
	if (m_nSize == nSize)
		return;
	//m_nSize = nSize;
	Reserve(nSize);

	for (int i = m_nSize; i < nSize; i++)
		m_pData[i] = 0.;

	m_nSize = nSize;
}

// get an element at an index
const double& DArray::GetAt(int nIndex) const {
	//TODO
	static double ERROR; // you should delete this line
	return ERROR; // you should return a correct value
}

// set the value of an element 
void DArray::SetAt(int nIndex, double dValue) {
	//TODO
}

// overload operator '[]'
double& DArray::operator[](int nIndex) {
	// TODO
	static double ERROR; // you should delete this line
	return ERROR; // you should return a correct value
}

// overload operator '[]'
const double& DArray::operator[](int nIndex) const {
	//TODO
	static double ERROR; // you should delete this line
	return ERROR; // you should return a correct value
}

// add a new element at the end of the array
void DArray::PushBack(double dValue) {
	//TODO
}

// delete an element at some index
void DArray::DeleteAt(int nIndex) {
	//TODO
}

// insert a new element at some index
void DArray::InsertAt(int nIndex, double dValue) {
	//TODO
}

// overload operator '='
DArray& DArray::operator = (const DArray& arr) {
	//TODO
	return *this;
}
