#ifndef MP2_LAB2_MATRIX_RAZREG_MATRIX_H
#define MP2_LAB2_MATRIX_RAZREG_MATRIX_H
#include "tmatrix.h"
template<typename T>
class Razreg_M{
private:
    size_t sz_m;
    TDynamicVector<T> pMem;
    TDynamicVector<size_t> rows_Ind;
    TDynamicVector<size_t> cols_Ind;
public:
    Razreg_M(size_t size_Matrix, size_t size_Razreg=1):sz_m(size_Matrix){
        pMem=TDynamicVector<T>(size_Razreg);
        rows_Ind=TDynamicVector<size_t>(size_Razreg);
        cols_Ind=TDynamicVector<size_t>(size_Razreg);
    }
    Razreg_M(const TDynamicMatrix<T>& Matrix):sz_m(Matrix.size()){
        size_t non_zero_elem = 0;
        for(size_t i = 0; i < sz_m; i++) {
            for(size_t j = 0; j < sz_m; j++) {
                if (Matrix[i][j]!=T()) {
                    non_zero_elem++;
                }
            }
        }
        pMem=TDynamicVector<T>(non_zero_elem);
        rows_Ind=TDynamicVector<size_t>(non_zero_elem);
        cols_Ind=TDynamicVector<size_t>(non_zero_elem);
        size_t ind = 0;
        for (size_t i =0; i < sz_m; i++) {
            for(size_t j = 0; j < sz_m; j++) {
                T val = Matrix[i][j];
                if (val != T()) {
                    pMem[ind] = val;
                    rows_Ind[ind] = i;
                    cols_Ind[ind] = j;
                    ind++;
                }
            }
        }
    }


    //Только чтения и записи существующих элементов
    T& at1(size_t rows, size_t cols){
        if (rows>=sz_m || cols>=sz_m) {
            throw std::out_of_range("Выход за пределы");
        }
        for(size_t i = 0; i <pMem.size(); i++) {
            if (rows_Ind[i]==rows && cols_Ind[i]==cols) {
                return pMem[i];
            }
        }
        throw std::out_of_range("Элемент не найден");
    }

    const T& at1(size_t rows, size_t cols)const{
        if (rows>=sz_m || cols>=sz_m) {
            throw std::out_of_range("Выход за пределы");
        }
        for(size_t i = 0; i <pMem.size(); i++) {
            if (rows_Ind[i]==rows && cols_Ind[i]==cols) {
                return pMem[i];
            }
        }
        throw std::out_of_range("Элемент не найден");
    }
    // Индексация когда необходимо вычисления с 0 (Только для чтения)
    T at2(size_t rows, size_t cols){
        if (rows>=sz_m || cols>=sz_m) {
            throw std::out_of_range("Выход за пределы");
        }
        for(size_t i = 0; i <pMem.size(); i++) {
            if (rows_Ind[i]==rows && cols_Ind[i]==cols) {
                return pMem[i];
            }
        }
        return T();
    }
    T at2(size_t rows, size_t cols)const{
        if (rows>=sz_m || cols>=sz_m) {
            throw std::out_of_range("Выход за пределы");
        }
        for(size_t i = 0; i <pMem.size(); i++) {
            if (rows_Ind[i]==rows && cols_Ind[i]==cols) {
                return pMem[i];
            }
        }
        return T();
    }
    TDynamicMatrix<T> Convert_Matrix()const{
        TDynamicMatrix<T> temp(sz_m);
        for(size_t i = 0; i < pMem.size(); i++) {
            size_t row = rows_Ind[i];
            size_t col = cols_Ind[i];
            temp[row][col] = pMem[i];
        }
        return temp;
    }
    Razreg_M operator*(const Razreg_M& R_M)const{
        if (this->sz_m!=R_M.sz_m) {
            throw std::invalid_argument("Нельзя умножать матрицы разного размера");
        }
        TDynamicMatrix<T>temp1=Convert_Matrix();
        TDynamicMatrix<T> temp2 = R_M.Convert_Matrix();
        TDynamicMatrix<T> result = temp1*temp2;
        return Razreg_M(result);
    }
    friend ostream& operator<<(ostream& ostr, const Razreg_M& R_M){
        for(size_t i = 0; i < R_M.pMem.size(); i++) {
            ostr << "[" << R_M.rows_Ind[i] << "," << R_M.cols_Ind[i]
                 << "] = " << R_M.pMem[i] << std::endl;
        }
        return ostr;
    }
};

#endif //MP2_LAB2_MATRIX_RAZREG_MATRIX_H