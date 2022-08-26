// implementation of class DArray
#include "DArray.h"
#include <cassert>
#include <iostream>
using namespace std;
// default constructor
DArray::DArray() {
	Init();
}

// set an array with default values
DArray::DArray(int nSize, double dValue) 
{
	//TODO
	m_pData = new double[nSize];
	m_nSize = nSize;
	for (int i = 0; i < nSize; i++)
	{
		m_pData[i] = dValue;
	}
}
 
DArray::DArray(const DArray& arr) 
{
	m_pData = new double[arr.m_nSize];
	m_nSize = arr.m_nSize;
	//TODO
	for (int i = 0; i < m_nSize; i++)
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
		cout<< GetAt(i)<<" ";
	}
	cout << endl;
}

// initilize the array
void DArray::Init() {
	//TODO
	m_pData = nullptr;
	m_nSize = 0;
}

// free the array
void DArray::Free() {
	//TODO
	delete[] m_pData;
	m_pData = nullptr;
	m_nSize = 0;
}

// get the size of the array
int DArray::GetSize() const {
	//TODO
	return m_nSize;
}

// set the size of the array
void DArray::SetSize(int nSize) {
	//TODO
	/*
	* m_nSize=nSize;
	*/
	if (m_nSize == nSize)
		return;

	double* pData = new double[nSize];

	int copyNum = nSize < m_nSize ? nSize : m_nSize;
	for (int i = 0; i < copyNum; i++)
		pData[i] = m_pData[i];
	for (int i = copyNum; i < nSize; i++)
		pData[i] = 0.;

	delete[] m_pData;
	m_pData = pData;
	m_nSize = nSize;
}

// get an element at an index
const double& DArray::GetAt(int nIndex) const {
	//TODO
	/*if (nIndex >= 0 && nIndex < m_nSize) {
		return m_pData[nIndex];
	}
	else {
		return false;
	}*/
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
	
}

// set the value of an element 
void DArray::SetAt(int nIndex, double dValue) {
	//TODO
	assert(nIndex >= 0 && nIndex < m_nSize);
	m_pData[nIndex] = dValue;
}

// overload operator '[]'
const double& DArray::operator[](int nIndex) const {
	//TODO
	assert(nIndex >= 0 && nIndex < m_nSize);
	return m_pData[nIndex];
}

// add a new element at the end of the array
void DArray::PushBack(double dValue) {
	//TODO
	/*m_pData[m_nSize] = dValue;
	m_nSize++;*/
	double* pPointer = new double[static_cast<size_t>(m_nSize) + 1];

	for (int i = 0; i < m_nSize; i++)
		pPointer[i] = m_pData[i];

	pPointer[m_nSize] = dValue;

	delete[] m_pData;
	m_pData = pPointer;
	m_nSize++;
}

// delete an element at some index
void DArray::DeleteAt(int nIndex) {
	//TODO
	assert(nIndex >= 0 && nIndex < m_nSize);
	double* pPointer = new double[static_cast<size_t>(m_nSize) - 1];
	for (int i = 0; i < nIndex; i++)
	{
		pPointer[i] = m_pData[i];
	}
	for (int i = nIndex; i < m_nSize - 1; i++)
	{
		pPointer[i] = m_pData[i + 1];
	}
	delete[] m_pData;
	m_pData = pPointer;
	m_nSize--;
}

// insert a new element at some index
void DArray::InsertAt(int nIndex, double dValue) {
	//TODO
	assert(nIndex >= 0 && nIndex <= m_nSize); // nIndex == m_nSize is legal
	double* pPointer = new double[static_cast<size_t>(m_nSize) + 1];
	for (int i = 0; i < nIndex; i++)
	{
		pPointer[i] = m_pData[i];
	}
	pPointer[nIndex] = dValue;
	for (int i = nIndex + 1; i < m_nSize + 1; i++)
	{
		pPointer[i] = m_pData[i - 1];
	}
	delete[] m_pData;
	m_pData = pPointer;
	m_nSize++;
}

// overload operator '='
DArray& DArray::operator = (const DArray& arr) {
	//TODO
	delete[] m_pData;
	m_nSize = arr.m_nSize;
	m_pData = new double[m_nSize];
	for (int i = 0; i < m_nSize; i++)
		m_pData[i] = arr[i];
	return *this;
}
