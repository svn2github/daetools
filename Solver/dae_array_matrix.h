#ifndef DAE_ARRAY_MATRIX_H
#define DAE_ARRAY_MATRIX_H

#include <stdio.h>
#include <typeinfo>
#include <iostream>
#include <fstream>
#include "../Core/helpers.h"
#include "solver_class_factory.h"
#include <boost/numeric/ublas/matrix_sparse.hpp>
#include <boost/numeric/ublas/io.hpp>
using namespace boost::numeric;

#ifdef HAS_GNU_GSL
#include <gsl/gsl_linalg.h>
#endif

#ifdef HAS_TRILINOS
#include <Amesos_ConfigDefs.h>
#include <Epetra_RowMatrix.h>
#include <Epetra_MultiVector.h>
#include <Epetra_SerialDenseVector.h>
#include <Epetra_SerialDenseMatrix.h>
#include <Epetra_CrsMatrix.h>
#endif

namespace dae
{
namespace solver
{
/*********************************************************************************************
	daeDenseArray
**********************************************************************************************/
class DAE_SOLVER_API daeDenseArray : public daeArray<real_t>
{
public:
	daeDenseArray(void)
	{
		N = 0;
		data = NULL;
	}
	virtual ~daeDenseArray(void)
	{
	}

public:
	virtual real_t GetItem(size_t i) const
	{
		if(!data || i >= N) 
			daeDeclareAndThrowException(exOutOfBounds);
		return data[i];
	}

	virtual void SetItem(size_t i, real_t value)
	{
		if(!data || i >= N) 
			daeDeclareAndThrowException(exOutOfBounds);
		data[i] = value;
	}

	virtual size_t GetSize(void) const
	{
		return N;
	}

	void InitArray(size_t n, real_t* pData)
	{
		N = n;
		data = pData;
	}
	
	void Print(bool bStructureOnly = false)
	{
		for(size_t i = 0; i < N; i++)
		{
			if(i != 0)
				std::cout << ", ";
			if(bStructureOnly)
				std::cout << (GetItem(i) == 0 ? "-" : "X");
			else
				std::cout << dae::toStringFormatted(GetItem(i), 12, 5);
		}
		std::cout << std::endl;
		std::cout.flush();
	}

protected:
	size_t 	N;
	real_t* data;
};

/*********************************************************************************************
	daeDenseMatrix
**********************************************************************************************/
class DAE_SOLVER_API daeDenseMatrix : public daeMatrix<real_t>
{
public:
	daeDenseMatrix(void)
	{
		Nrow        = 0;
		Ncol        = 0;
		data_access = eColumnWise;
		data        = NULL;
	}
	virtual ~daeDenseMatrix(void)
	{
	}

public:
// Nrow is the number of rows
// Ncol is the number of columns
// GetItem/GetItem access is always (row, column)
// That internally translates to :
//      [row][col] if eRowWise
//      [col][row] if eColumnWise
	virtual real_t GetItem(size_t row, size_t col) const
	{
		if(!data) 
			daeDeclareAndThrowException(exInvalidPointer);
		if(row >= Nrow || col >= Ncol) 
			daeDeclareAndThrowException(exOutOfBounds);

		if(data_access == eRowWise)
			return data[row][col];
		else
			return data[col][row];
	}

	virtual void SetItem(size_t row, size_t col, real_t value)
	{
		if(!data) 
			daeDeclareAndThrowException(exInvalidPointer);
		if(row >= Nrow || col >= Ncol) 
			daeDeclareAndThrowException(exOutOfBounds);

		if(data_access == eRowWise)
			data[row][col] = value;
		else
			data[col][row] = value;
	}

	virtual size_t GetNrows(void) const
	{
		return Nrow;
	}

	virtual size_t GetNcols(void) const
	{
		return Ncol;
	}

	void InitMatrix(size_t nrows, size_t ncols, real_t** pData, daeeMatrixAccess access)
	{
		Nrow        = nrows;
		Ncol        = ncols;
		data_access = access;
		data        = pData;
	}
	
	void Print(bool bStructureOnly = false)
	{
		for(size_t row = 0; row < Nrow; row++)
		{
			for(size_t col = 0; col < Ncol; col++)
			{
				if(col != 0)
					std::cout << " ";
				if(bStructureOnly)
					std::cout << (GetItem(row, col) == 0 ? "-" : "X");
				else
					std::cout << dae::toStringFormatted(GetItem(row, col), 12, 5);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout.flush();
	}
	
	void ClearMatrix(void)
	{
		if(!data) 
			daeDeclareAndThrowException(exInvalidPointer);
		
		if(data_access == eRowWise)
		{
			for(size_t row = 0; row < Nrow; row++)
				for(size_t col = 0; col < Ncol; col++)
					data[row][col] = 0;
		}
		else
		{
			for(size_t row = 0; row < Nrow; row++)
				for(size_t col = 0; col < Ncol; col++)
					data[col][row] = 0;
		}
	}

public:
	size_t				Nrow;
	size_t				Ncol;
	real_t**			data;
	daeeMatrixAccess	data_access;
};

inline bool CompareMatrices(daeMatrix<real_t>& left, daeMatrix<real_t>& right)
{
	size_t lNrow = left.GetNrows();
	size_t lNcol = left.GetNcols();
	size_t rNrow = right.GetNrows();
	size_t rNcol = right.GetNcols();
	
	if(lNrow != rNrow)
		return false;
	if(lNcol != rNcol)
		return false;
	
	for(size_t i = 0; i < lNrow; i++)
		for(size_t k = 0; k < lNcol; k++)
			if(left.GetItem(i, k) != right.GetItem(i, k))
				return false;

	return true;
}

/*********************************************************************************************
	daeLapackMatrix
**********************************************************************************************/
class DAE_SOLVER_API daeLapackMatrix : public daeMatrix<real_t>
{
public:
	daeLapackMatrix(void)
	{
		Nrow        = 0;
		Ncol        = 0;
		data_access = eColumnWise;
		data        = NULL;
	}
	virtual ~daeLapackMatrix(void)
	{
	}

public:
// Nrow is the number of rows
// Ncol is the number of columns
// GetItem/GetItem access is always (row, column)
// That internally translates to :
//      [row][col] if eRowWise
//      [col][row] if eColumnWise
	virtual real_t GetItem(size_t row, size_t col) const
	{
		if(!data) 
			daeDeclareAndThrowException(exInvalidPointer);
		if(row >= Nrow || col >= Ncol) 
			daeDeclareAndThrowException(exOutOfBounds);

		if(data_access == eRowWise)
			return data[row*Ncol + col];
		else
			return data[col*Nrow + row];
	}

	virtual void SetItem(size_t row, size_t col, real_t value)
	{
		if(!data) 
			daeDeclareAndThrowException(exInvalidPointer);
		if(row >= Nrow || col >= Ncol) 
			daeDeclareAndThrowException(exOutOfBounds);

		if(data_access == eRowWise)
			data[row*Ncol + col] = value;
		else
			data[col*Nrow + row] = value;
	}

	virtual size_t GetNrows(void) const
	{
		return Nrow;
	}

	virtual size_t GetNcols(void) const
	{
		return Ncol;
	}

	void InitMatrix(size_t nrows, size_t ncols, real_t* pData, daeeMatrixAccess access)
	{
		Nrow        = nrows;
		Ncol        = ncols;
		data_access = access;
		data        = pData;
	}
	
	void Print(bool bStructureOnly = false)
	{
		for(size_t i = 0; i < Nrow; i++)
		{
			for(size_t k = 0; k < Ncol; k++)
			{
				if(k != 0)
					std::cout << " ";
				if(bStructureOnly)
					std::cout << (GetItem(i, k) == 0 ? "-" : "X");
				else
					std::cout << dae::toStringFormatted(GetItem(i, k), 12, 5);
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout.flush();
	}
	
	void ClearMatrix(void)
	{
		::memset(data, 0, Nrow*Ncol*sizeof(real_t));
	}
	
public:
	size_t				Nrow;
	size_t				Ncol;
	real_t*				data;
	daeeMatrixAccess	data_access;
};		

/*********************************************************************************************
	daeCSRMatrix
**********************************************************************************************/
template<typename FLOAT, typename INT>
class daeCSRMatrix : public daeSparseMatrix<FLOAT>
{
public:
	daeCSRMatrix(void)
	{
		N        = 0;
		NNZ      = 0;
		A        = NULL;
		IA       = NULL;
		JA       = NULL;
		indexing = CSR_C_STYLE;
	}
	
	daeCSRMatrix(int n, int nnz, bool ind)
	{
		N        = 0;
		NNZ      = 0;
		A        = NULL;
		IA       = NULL;
		JA       = NULL;
		indexing = ind;
	
		Reset(n, nnz, ind);
	}
	
	~daeCSRMatrix(void)
	{
		Free();
	}
	
public:
	bool GetIndexing(void)
	{
		return indexing;
	}
	
	void SetIndexing(bool ind)
	{
		indexing = ind;
	}
	
	void Reset(int n, int nnz, bool ind)
	{
		N        = n;
		NNZ      = nnz;
		indexing = ind;
		
	// If the pointers are NULL then it will allocate as in malloc
	// Otherwise it will realloc the memory to the new size
		A  = (FLOAT*)realloc(A,   NNZ  * sizeof(FLOAT));
		IA = (INT*)  realloc(IA, (N+1) * sizeof(INT));
		JA = (INT*)  realloc(JA,  NNZ  * sizeof(INT));
		
		if(!A || !IA || !JA)
		{
			Free();
			daeDeclareException(exMiscellanous);
			e << "Unable to allocate memory for daeCSRMatrix";
			throw e;
		}

		memset(A,  0,  NNZ  * sizeof(FLOAT));
		memset(IA, 0, (N+1) * sizeof(INT));
		memset(JA, 0,  NNZ  * sizeof(INT));
		
	// The last item in IA is the number of NNZ	
		if(indexing == CSR_C_STYLE)
			IA[N] = NNZ;
		else
			IA[N] = NNZ + 1;
	}
	
	void ClearValues(void)
	{
		memset(A,  0,  NNZ  * sizeof(FLOAT));
	}

	void Free(void)
	{
		if(A)
			free(A);
		if(IA)
			free(IA);
		if(JA)
			free(JA);
		
		N   = 0;
		NNZ = 0;
		A   = NULL;
		IA  = NULL;
		JA  = NULL;
	}
	
	real_t GetItem(size_t i, size_t j) const
	{
		int index = CalcIndex(i, j);
		if(index < 0)
			return 0.0;
		else
			return A[index];
	}
	
	void SetItem(size_t i, size_t j, real_t val)
	{
		temp = CalcIndex(i, j);
		if(temp < 0)
		{
			daeDeclareException(exMiscellanous);
			e << "Invalid element in CRS matrix: (" << i << ", " << j << ")";
			throw e;
		}
		A[temp] = val;
	}
	
	size_t GetNrows(void) const
	{
		return N;
	}
	
	size_t GetNcols(void) const
	{
		return N;
	}
		
	int CalcIndex(size_t i, size_t j) const
	{
		if(i >= N || j >= N) 
			daeDeclareException(exOutOfBounds);
	
		if(indexing == CSR_C_STYLE)
		{
			for(int k = IA[i]; k < IA[i+1]; k++)
				if(j == JA[k])
					return k;
		}
		else
		{
			j = j + 1; // FORTRAN arrays use 1 based indexing
			for(int k = IA[i]-1; k < IA[i+1]-1; k++)
				if(j == JA[k])
					return k;
		}
		return -1;
	}

	real_t CalculateRatio() const
	{
		real_t sparse = 2 * NNZ + N + 1;
		real_t dense  = N * N;
		
		return sparse/dense; 
	}
	
	void Print(bool bStructureOnly = false, bool bPrintValues = false) const
	{
		int n, i, k;
		real_t value;
		
		std::cout << "N     = " << N   << std::endl;
		std::cout << "NNZ   = " << NNZ << std::endl;
		std::cout << "Ratio = " << CalculateRatio() << std::endl;
		std::cout << "IA:" << std::endl;
		for(i = 0; i < N+1; i++)
		{
			if(k != 0)
				std::cout << ", ";
			std::cout << IA[i];
		}
		std::cout << std::endl;
	
		std::cout << "JA:" << std::endl;
		for(i = 0; i < NNZ; i++)
		{				
			if(k != 0)
				std::cout << ", ";
			std::cout << JA[i];
		}
		std::cout << std::endl;
	
		if(bPrintValues)
		{
			std::cout << "A:" << std::endl;
			for(i = 0; i < NNZ; i++)
			{
				if(k != 0)
					std::cout << ", ";
				std::cout << A[i];
			}
			std::cout << std::endl;

			for(i = 0; i < N; i++)
			{
				for(k = 0; k < N; k++)
				{				
					n = CalcIndex(i, k);
					if(n < 0)
						value = 0.0;
					else
						value = A[n];				
					
					if(k != 0)
						std::cout << " ";
					if(bStructureOnly)
						std::cout << (n < 0 ? "-" : "X");
					else
						std::cout << dae::toStringFormatted(value, 12, 5);				
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;	
		}
		std::cout.flush();
	}
	
	void AddRow(const std::map<size_t, size_t>& mapIndexes)
	{
		std::map<size_t, size_t>::const_iterator iter;
		for(iter = mapIndexes.begin(); iter != mapIndexes.end(); iter++)
			SetColumnIndex(iter->second);
		NextRow();
	}

	void ResetCounters(void)
	{
	// Reset the row counter	
		rowCounter = 0;

	// Reset column indexes counter
		counter = 0;

	// Indexes in the 1st row always start with the 1st element
		if(indexing == CSR_C_STYLE)
			IA[0] = 0; 
		else
			IA[0] = 1;
	}
	
	void NextRow(void)
	{
		rowCounter++;
		if(indexing == CSR_C_STYLE)
			IA[rowCounter] = counter;
		else
			IA[rowCounter] = counter + 1;
	}
	
	void SetColumnIndex(size_t col)
	{
		if(indexing == CSR_C_STYLE)
			JA[counter] = col;
		else
			JA[counter] = col + 1;
			
		counter++;
	}

	void Sort(void)
	{
		int i, b, e;

		for(i = 0; i < N; i++)
		{
			if(indexing == CSR_C_STYLE)
			{
				b = IA[i];
				e = IA[i+1];
			}
			else
			{
				b = IA[i]   - 1;
				e = IA[i+1] - 1;
			}
			std::sort(&JA[b], &JA[e]);
		}
	}

	void SaveMatrixAsXPM(const std::string& strFilename)
	{
		size_t i, j;
		std::ofstream of(strFilename.c_str(), std::ios_base::out);
		if(!of.is_open())
			return;
		
		char* row  = new char[N+1];
		row[N]  = '\0';
	
		of << "/* XPM */" << std::endl;
		of << "static char *dummy[]={" << std::endl;
		of << "\"" << N << " " << N << " " << 2 << " " << 1 << "\"," << std::endl;
		of << "\"- c #ffffff\"," << std::endl;
		of << "\"X c #000000\"," << std::endl;
		
		for(i = 0; i < N; i++)
		{
			memset(row, '-', N);
			for(j = 0; j < N; j++)
			{				
				if(CalcIndex(i, j) >= 0)
					row[j] = 'X';			
			}
			of << "\"" << row << (i == N-1 ? "\"" : "\",") << std::endl;
		}
		of << "};" << std::endl;
		of.close();
		delete[] row;
	}
	
public:
	int     NNZ;      // no of non-zero elements
	int     N;        // matrix size
	FLOAT*  A;        // values
	INT*    IA;       // row indexes data
	INT*    JA;       // column indexes
	bool    indexing; // C style arrays start from 0, FORTRAN from 1
	
protected:
	int    temp;
	size_t rowCounter;
	size_t counter;
};

}
}

#endif
