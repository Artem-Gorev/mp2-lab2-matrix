// ННГУ, ИИТММ, Курс "Алгоритмы и структуры данных"
//
// Copyright (c) Сысоев А.В.
//
//

#ifndef __TDynamicMatrix_H__
#define __TDynamicMatrix_H__

#include <iostream>

using namespace std;

const int MAX_VECTOR_SIZE = 100000000;
const int MAX_MATRIX_SIZE = 10000;

// Динамический вектор - 
// шаблонный вектор на динамической памяти
template<typename T>
class TDynamicVector
{
protected:
  size_t sz;
  T* pMem;
public:
  TDynamicVector(size_t size = 1) : sz(size)
  {
    if (sz == 0 || sz > MAX_VECTOR_SIZE)
      throw out_of_range("Длинна вектора не должна быть равна 0 или быть больше MAX_VECTOR_SIZE");
    pMem = new T[sz]();
  }
  TDynamicVector(T* arr, size_t size) : sz(size) {
    if (arr == nullptr)
      throw invalid_argument("Указатель на массив не должен быть равен nullptr");
    pMem = new T[size];
    for (int i = 0; i < size; i++) {
      pMem[i] = arr[i];
    }
  }
  TDynamicVector(const TDynamicVector& v): sz(v.sz), pMem(new T[v.sz]())
  {
    for (size_t i = 0; i < v.sz; i++) {
      this->pMem[i] = v.pMem[i];
    }
  }
  TDynamicVector(TDynamicVector&& v) noexcept : sz(v.sz), pMem(v.pMem)
  {
    v.pMem = nullptr;
    v.sz = 0;
  }
  ~TDynamicVector()
  {
    delete[] pMem;
  }
  TDynamicVector& operator=(const TDynamicVector& v)
  {
    if (this == &v) {
      return *this;
    }
    this->sz = v.sz;
    if (this->pMem != nullptr) {
      delete[] pMem;
    }
    pMem = new T[sz];
    for (size_t i = 0; i < sz; i++) {
      this->pMem[i] = v.pMem[i];
    }
    return *this;
  }
  TDynamicVector& operator=(TDynamicVector&& v) noexcept
  {
    if (this != &v) {
      delete[] pMem;
      sz = v.sz;
      pMem = v.pMem;
      v.sz = 0;
      v.pMem = nullptr;
    }
    return *this;
  }

  size_t size() const noexcept { return sz; }

  // индексация
  T& operator[](size_t ind)
  {
    return pMem[ind];
  }
  const T& operator[](size_t ind) const
  {
    return pMem[ind];
  }
  // индексация с контролем
  T& at(size_t ind)
  {
    if (ind >= sz)
      throw out_of_range("Индекс вне диапазона");
    return pMem[ind];
  }
  const T& at(size_t ind) const
  {
    if (ind >= sz)
      throw out_of_range("Индекс вне диапазона");
    return pMem[ind];
  }

  // сравнение
  bool operator==(const TDynamicVector& v) const noexcept
  {
    if (this == &v) return true;
    if (sz != v.sz) return false;

    for (size_t i = 0; i < sz; i++)
      if (pMem[i] != v.pMem[i])
        return false;
    return true;
  }
  bool operator!=(const TDynamicVector& v) const noexcept
  {
    return !(*this == v);
  }

  // скалярные операции
  TDynamicVector operator+(T val)const
  {
    TDynamicVector temp(sz);
    for (size_t i = 0; i < sz; i++) {
      temp.pMem[i] = pMem[i] + val;
    }
    return temp;
  }
  TDynamicVector operator-(T val)const
  {
    TDynamicVector temp(sz);
    for (size_t i = 0; i < sz; i++) {
      temp.pMem[i] = pMem[i] - val;
    }
    return temp;
  }
  TDynamicVector operator*( T val) const
  {
    TDynamicVector temp(sz);
    for (size_t i = 0; i < sz; i++) {
      temp.pMem[i] = this->pMem[i] * val;
    }
    return temp;
  }

  // векторные операции
  TDynamicVector operator+(const TDynamicVector& v)const
  {
    if (this->sz != v.sz) {
      throw invalid_argument("Складываются вектора разного размера");
    }
    TDynamicVector temp(sz);
    for (int i = 0; i < sz; i++) {
      temp.pMem[i] = pMem[i] + v.pMem[i];
    }
    return temp;

  }
  TDynamicVector operator-(const TDynamicVector& v)const
  {
    if (this->sz != v.sz) {
      throw invalid_argument("Вычитаются вектора разного размера");
    }
    TDynamicVector temp(sz);
    for (size_t i = 0; i < sz; i++) {
      temp.pMem[i] =pMem[i] - v.pMem[i];
    }
    return temp;
  }

  T operator*(const TDynamicVector& v)const{
    if (this->sz != v.sz) {
      throw invalid_argument("Умножаются вектора разного размера");
    }
    T result = T();
    for (size_t i = 0; i < sz; i++) {
      result += this->pMem[i] * v.pMem[i];
    }
    return result;
  }

  friend void swap(TDynamicVector& lhs, TDynamicVector& rhs) noexcept
  {
    std::swap(lhs.sz, rhs.sz);
    std::swap(lhs.pMem, rhs.pMem);
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      istr >> v.pMem[i]; // требуется оператор>> для типа T
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicVector& v)
  {
    for (size_t i = 0; i < v.sz; i++)
      ostr << v.pMem[i] << ' '; // требуется оператор<< для типа T
    return ostr;
  }
};


// Динамическая матрица - 
// шаблонная матрица на динамической памяти
template<typename T>
class TDynamicMatrix : public TDynamicVector<TDynamicVector<T>>
{
  using TDynamicVector<TDynamicVector<T>>::pMem;
  using TDynamicVector<TDynamicVector<T>>::sz;
public:
  TDynamicMatrix(size_t s = 1) : TDynamicVector<TDynamicVector<T>>(s)
  {
    if (s == 0 || sz > MAX_MATRIX_SIZE)
      throw out_of_range("Размер матрицы должен быть не равен 0 и не больше MAX_MATRIX_SIZE");
    for (size_t i = 0; i < sz; i++)
      pMem[i] = TDynamicVector<T>(sz);
  }
  using TDynamicVector<TDynamicVector<T>>::operator[];

  // сравнение
  bool operator==(const TDynamicMatrix& m) const noexcept
  {
    if ( this == &m) return true;
    if (sz != m.sz) return false;
    for (size_t i = 0; i < sz; ++i) {
      if (pMem[i] != m.pMem[i])
        return false;
    }
    return true;
  }

  bool operator!=(const TDynamicMatrix& m) const noexcept {
    return !(*this==m);
  }


  // матрично-скалярные операции
  TDynamicMatrix operator*(const T& val)const{
    TDynamicMatrix temp(sz);
    for (size_t i = 0; i < sz; ++i) {
      temp.pMem[i] = pMem[i] * val;
    }
    return temp;
  }

  // матрично-векторные операции
  TDynamicVector<T> operator*(const TDynamicVector<T>& v) const
  {
    TDynamicVector<T> temp(sz);
    for (int i = 0; i < sz; i++)
      temp[i] = pMem[i] * v;
    return temp;
  }

  // матрично-матричные операции
  TDynamicMatrix operator+(const TDynamicMatrix& m) const
  {
    if (sz != m.sz) {
      throw invalid_argument("Матрицы разных размеров складывать нельзя");
    }
    TDynamicMatrix temp(sz);
    for (size_t i = 0; i < sz; i++) {
      temp.pMem[i] = pMem[i] + m.pMem[i];
    }
    return temp;
  }
  TDynamicMatrix operator-(const TDynamicMatrix& m) const
  {
    if (this->sz != m.sz) {
      throw invalid_argument("Матрицы разных размеров вычитать нельзя");
    }
    TDynamicMatrix temp(sz);
    for (size_t i = 0; i < sz; i++) {
      temp.pMem[i] = pMem[i] - m.pMem[i];
    }
    return temp;
  }

  TDynamicMatrix Transpose()const{
    TDynamicMatrix temp(sz);
    for (size_t i = 0; i < sz; i++) {
      for (size_t j = 0; j < sz; j++) {
        temp.pMem[i][j] = this->pMem[j][i];
      }
    }
    return temp;
  }
  TDynamicMatrix operator*(const TDynamicMatrix& m) const{
    if (this->sz != m.sz) {
      throw std::invalid_argument("Матрицы разных размеров умножать нельзя");
    }
    TDynamicMatrix temp(sz);
    TDynamicMatrix mT=m.Transpose();
    for (size_t i = 0; i<sz; i++) {
      for (size_t j = 0; j<sz; j++) {
        temp[i][j]=this->pMem[i]*mT.pMem[j];
      }
    }
    return temp;
  }

  // ввод/вывод
  friend istream& operator>>(istream& istr, TDynamicMatrix& v)
  {

    for (size_t i = 0; i < v.sz; i++) {
      istr >> v.pMem[i]; // требуется оператор>> для типа T
    }
    return istr;
  }
  friend ostream& operator<<(ostream& ostr, const TDynamicMatrix& v)
  {
    for (size_t i = 0; i < v.sz; i++) {
      ostr << v.pMem[i]<<endl;
    }
    return ostr;
  }
};

#endif
