#ifndef MP2_LAB2_MATRIX_LENT_MATRIX_H
#define MP2_LAB2_MATRIX_LENT_MATRIX_H
#include "tmatrix.h"
template <typename T>
class Lent_M{
private:
    size_t sz_m;
    size_t upper_diag; //Количество диагоналей
    size_t lower_diag;
    TDynamicVector<TDynamicVector<T>> pMem;
public:
    Lent_M(size_t size_M, size_t lower, size_t upper): sz_m(size_M), upper_diag(upper), lower_diag(lower), pMem(lower + upper + 1){
        for (size_t i = 0; i < lower + upper + 1; i++) {
            pMem[i] = TDynamicVector<T>(sz_m);
        }
    }
    Lent_M(const TDynamicMatrix<T>& Matrix):sz_m(Matrix.size()){
        upper_diag = 0, lower_diag = 0;
        for (size_t k = 1; k < sz_m; k++) {
            bool found = false;
            for (size_t i = 0, j = k; j < sz_m; i++, j++) {
                if (Matrix[i][j] != 0) {
                    found = true;
                    break;
                }
            }
            if (found) upper_diag = k;
            else break;
        }
        for (size_t k = 1; k < sz_m; k++) {
            bool found = false;
            for (size_t j = 0, i = k; i < sz_m; i++, j++) {
                if (Matrix[i][j] != 0) {
                    found = true;
                    break;
                }
            }
            if (found) lower_diag = k;
            else break;
        }
        pMem=TDynamicVector<TDynamicVector<T>>(lower_diag + upper_diag+1);
        for (size_t i = 0; i < lower_diag + upper_diag + 1; i++) {
            pMem[i] = TDynamicVector<T>(sz_m);
        }

        for (size_t k = 1; k <= lower_diag; k++) {
            size_t lower_ind = k-1;
            for (size_t i = k, j = 0; i < sz_m; i++, j++) {
                pMem[lower_ind][j] = Matrix[i][j];
            }
        }
        size_t main_diag_ind=lower_diag;
        for (size_t i = 0, j = 0; i < sz_m; i++, j++ ) {
            pMem[main_diag_ind][j] = Matrix[i][j];
        }

        for (size_t k = 1; k<= upper_diag; k++) {
            size_t upper_ind = k + main_diag_ind;
            for (size_t j=k, i = 0; j < sz_m; j++, i++) {
                pMem[upper_ind][i] = Matrix[i][j];
            }
        }
    }
    // Индексация когда необходимо вычисления с 0 (Только для чтения)
    T at(size_t i, size_t j){
        if (i >= sz_m || j >= sz_m) {
            throw std::out_of_range("Выход за пределы");
        }

        if (j > i) {
            if (j - i > upper_diag) return T();
            size_t diag_index = lower_diag + (j - i);
            return pMem[diag_index][i];
        }
        else {
            if (i - j > lower_diag) return T();
            size_t diag_index = lower_diag - (i - j);
            return pMem[diag_index][i];
        }
    }
    T at(size_t i, size_t j)const {
        if (i >= sz_m || j >= sz_m) {
            throw std::out_of_range("Выход за пределы");
        }

        if (j > i) {
            if (j - i > upper_diag) return T();
            size_t diag_index = lower_diag + (j - i);
            return pMem[diag_index][i];
        }
        else {
            if (i - j > lower_diag) return T();
            size_t diag_index = lower_diag - (i - j);
            return pMem[diag_index][i];
        }
    }
    TDynamicMatrix<T> Convert_Matrix()const {
        TDynamicMatrix<T> temp(sz_m);
        for (size_t i = 0; i < sz_m; i++) {
            for (size_t j = 0; j < sz_m; j++) {
                temp[i][j] = this->at(i,j);
            }
        }
        return temp;
    }
    Lent_M operator*(const Lent_M& L_M)const{
        if (this->sz_m!=L_M.sz_m) {
            throw std::invalid_argument("Нельзя умножать матрицы разного размера");
        }
        TDynamicMatrix<T> temp1=Convert_Matrix();
        TDynamicMatrix<T> temp2 = L_M.Convert_Matrix();
        TDynamicMatrix<T> result = temp1*temp2;
        return Lent_M(result);

    }
    friend ostream& operator<<(ostream& ostr, const Lent_M& L_M) {
        for (size_t i = 0; i < L_M.sz_m; i++) {
            for (size_t j = 0; j <L_M.sz_m; j++ ) {
                ostr << L_M.at(i,j) << " ";
            }
            ostr << endl;
        }
        return ostr;
    }

};
#endif //MP2_LAB2_MATRIX_LENT_MATRIX_H