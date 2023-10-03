#pragma once

#include <stdio.h>
#include <initializer_list>
#include <iostream>

template <typename _T>
class Matrix
{
private:
	// Data Storage
	_T** data;

	// Length
	int rows;
	int columns;

	bool isFree;

	_T** BuildPtr(int rowCount, int columnCount)
	{
		// Allocate new Data pointer array
		_T** nData = new _T * [rowCount];

		for (int i = 0; i < rowCount; i++)
			nData[i] = new _T[columnCount];

		return nData;
	}

	void DestroyPtr(_T** ptr, int rowCount)
	{
		for (int i = 0; i < rowCount; i++)
			delete[] ptr[i];

		delete[] ptr;
	}

public:
	Matrix()
		: rows(0), columns(0), isFree(false)
	{

	}

	Matrix(int rowCount, int columnCount, _T defaultValue = 0)
		: rows(rowCount), columns(columnCount), isFree(false)
	{
		data = (BuildPtr(rows, columns));

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < columns; c++)
				data[r][c] = (_T)defaultValue;
	}

	Matrix(std::initializer_list<std::initializer_list<int>> mx)
		: rows(mx.size()), columns(mx.begin()->size()), isFree(false)
	{
		data = (BuildPtr(rows, columns));

		int r = 0;
		for (const auto& row : mx)
		{
			if (this->columns != row.size())
				throw 1;

			int c = 0;
			for (const auto& data : row)
			{
				this->data[r][c] = data;
				c++;
			}
			r++;
		}
	}

	// Copy Constructor
	Matrix(const Matrix<_T>& mx)
		: rows(mx.GetRows()), columns(mx.GetColumns()), isFree(false)
	{
		data = BuildPtr(rows, columns);

		for (int r = 0; r < rows; r++)
			for (int c = 0; c < columns; c++)
				this->data[r][c] = mx[r][c];
	}

	~Matrix()
	{
		// Clear memory if ptr is not free
		if (!isFree)
			DestroyPtr(data, rows);
	}

	// Memory management
	Matrix<_T>& Free()
	{
		this->isFree = true;

		return *this;
	}

	void Delete()
	{
		isFree = true;
		DestroyPtr(data, rows);
	}

	// DEBUG FUNCTIONS
	void Display()
	{
		printf("\n");
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
				std::cout << this->data[r][c] << "\t";
			printf("\n");
		}
		printf("\n");
	}
	// END DEBUG FUNCTIONS

	// General Functions

	static Matrix<double> Identity(int rowCount, int columnCount)
	{
		if (rowCount != columnCount)
			throw 1;

		Matrix<double> result(rowCount, columnCount);

		for (int r = 0; r < rowCount; r++)
		{
			for (int c = 0; c < columnCount; c++)
			{
				if (r == c) result[r][c] = 1;
				else result[r][c] = 0;
			}
		}

		return result;
	}

	_T At(int row, int column)
	{
		return this->data[row][column];
	}

	void Resize(int rowCount, int columnCount)
	{
		// Allocate new Data pointer array
		_T** nData = BuildPtr(rowCount, columnCount);

		// Store old data
		for (int r = 0; r < ((this->rows > rowCount) ? rowCount : this->rows); r++)
			for (int c = 0; c < ((this->columns > columnCount) ? columnCount : this->columns); c++)
				nData[r][c] = this->data[r][c];

		// delete data ptr to re-allocate it
		DestroyPtr(this->data, this->rows);

		// Re-allocate ptr with new size
		this->data = BuildPtr(rowCount, columnCount);

		// Fill with zeros
		for (int r = 0; r < rowCount; r++)
			for (int c = 0; c < columnCount; c++)
				this->data[r][c] = 0;

		// Restore fitting results from prev mx
		for (int r = 0; r < ((this->rows > rowCount) ? rowCount : this->rows); r++)
			for (int c = 0; c < ((this->columns > columnCount) ? columnCount : this->columns); c++)
				this->data[r][c] = nData[r][c];

		// Delete tmp ptr and data
		DestroyPtr(nData, rowCount);

		// Set new row sizes
		this->rows = rowCount;
		this->columns = columnCount;
	}

	int GetRows() const
	{
		return rows;
	}

	int GetColumns() const
	{
		return columns;
	}

	void Augment(Matrix<_T>& mx)
	{
		if (rows != mx.GetRows())
			throw 1;

		int tRows = this->rows;
		int tCols = this->columns;

		this->Resize(rows, columns + mx.GetColumns());

		for (int r = 0; r < mx.GetRows(); r++)
			for (int c = tCols; c < columns; c++)
				data[r][c] = mx.At(r, c - tCols);
	}

	Matrix<_T> Transpose()
	{
		// Allocate new Data pointer array
		// N x M mx now becomes M x N mx
		_T** nData = BuildPtr(columns, rows);

		// Store old data in new matrix
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < columns; c++)
				nData[c][r] = data[r][c];

		// New matrix
		Matrix<_T> mx(this->columns, this->rows);

		// Update new matrix
		for (int r = 0; r < this->rows; r++)
			for (int c = 0; c < this->columns; c++)
				mx[c][r] = nData[c][r];

		// Destroy tmp ptr
		DestroyPtr(nData, columns);

		return mx;
	}

	Matrix<_T> SubMatrix(int startRow, int endRow, int startCol, int endCol)
	{
		if (startRow < 0 && endRow < 0)
			throw 1;

		if (startRow > endRow || startCol > endCol)
			throw 1;

		if (startRow >= this->rows && endRow >= this->rows)
			throw 1;

		if (startCol >= this->columns && endCol >= this->columns)
			throw 1;

		Matrix<_T> mx(endRow - startRow, endCol - startCol);

		for (int r = startRow; r < endRow; r++)
			for (int c = startCol; c < endCol; c++)
				mx[r - startRow][c - startCol] = data[r][c];

		return mx;
	}

	// Arithmatic Functions (primitive types only)

	// Row Reduced Echelon Form
	Matrix<_T> RREF()
	{
		// Result mx
		Matrix<double> mx(rows, columns);

		// Copy this->data into mx.data
		for (int r = 0; r < rows; r++)
			for (int c = 0; c < columns; c++)
				mx[r][c] = this->data[r][c];

		// Pivot row and pivot column
		int pr = 0;
		int pc = 0;

		while (pr < rows && pc < columns)
		{
			// Get pivot column
			if (mx[pr][pc] == 0)
			{
				int swapPiv = -1;
				for (int r = pr; r < rows; r++)
					if (mx[r][pc] != 0)
						swapPiv = r;

				// Row Swap
				if (swapPiv != -1)
				{
					Matrix<double> vecBuffer(1, this->columns);

					// Store Rpr in tmp vector
					for (int c = 0; c < this->columns; c++)
						vecBuffer[0][c] = mx[pr][c];

					// Swap Rpr with RswapPiv
					for (int c = 0; c < this->columns; c++)
						mx[pr][c] = mx[swapPiv][c];

					// Place buffer content into RswapPiv
					for (int c = 0; c < this->columns; c++)
						mx[swapPiv][c] = vecBuffer[0][c];
				}
				else
					pc++;
			}
			else
			{
				// For every element in the pivot column, find the ratio and subtract it
				// Rn = Rn - ratio * Rp
				// Where rn is current row and Rp is pivot row
				for (int r = 0; r < rows; r++)
				{
					// skip the pivot row
					if (r == pr) continue;

					// Find required ratio to make a pivot
					double ratio = mx[r][pc] / mx[pr][pc];

					// Get every column element of Rn
					for (int c = 0; c < columns; c++)
					{
						// Rn = Rn - ratio * Rp
						mx[r][c] = mx[r][c] - mx[pr][c] * ratio;
					}
				}

				int div = mx[pr][pc];

				// If pivot does not = 1 then divide row by pivot
				if (div != 1)
					for (int c = pc; c < columns; c++)
						mx[pr][c] /= div;

				// Increment pivot
				pr++;
				pc++;
			}
		}

		return mx;
	}

	Matrix<double> Inverse()
	{
		if (this->columns != this->rows)
			throw 1;

		if (this->RREF() != Matrix::Identity(this->rows, this->columns))
			throw 1;

		Matrix<double> mx = *this;
		Matrix<double> Ix = *this;
		Matrix<double> result(Ix.GetRows(), Ix.GetColumns());

		for (int r = 0; r < Ix.GetRows(); r++)
		{
			for (int c = 0; c < Ix.GetColumns(); c++)
			{
				if (r == c)
					Ix[r][c] = 1;
				else
					Ix[r][c] = 0;
			}
		}

		mx.Augment(Ix);

		result = mx.RREF().SubMatrix(0, mx.GetRows(), Ix.GetColumns(), mx.GetColumns());

		return result;
	}

	// Operators

	_T* operator[](int index) const
	{
		return data[index];
	}

	Matrix<_T> operator+(Matrix<_T>& mx)
	{
		if (this->rows != mx.GetRows() || this->columns != mx.GetColumns())
			throw 1;

		Matrix<_T> result(this->rows, this->columns);

		for (int r = 0; r < this->rows; r++)
			for (int c = 0; c < this->columns; c++)
				result[r][c] = this->data[r][c] + mx[r][c];

		return result;
	}

	Matrix<_T> operator-(Matrix<_T>& mx)
	{
		if (this->rows != mx.GetRows() || this->columns != mx.GetColumns())
			throw 1;

		Matrix<_T> result(this->rows, this->columns);

		for (int r = 0; r < this->rows; r++)
			for (int c = 0; c < this->columns; c++)
				result[r][c] = this->data[r][c] - mx[r][c];

		return result;
	}

	// Dot product
	Matrix<_T> operator*(Matrix<_T>& mx)
	{
		if (this->columns != mx.GetRows())
			throw 1;

		int rowCount = this->rows;
		int columnCount = mx.columns;
		Matrix<_T> result(rowCount, columnCount);

		for (int i = 0; i < mx.GetColumns(); i++)
			for (int r = 0; r < this->rows; r++)
				for (int c = 0; c < this->columns; c++)
					result[r][i] += this->data[r][c] * mx[c][i];

		return result;
	}

	Matrix<_T>& operator=(const Matrix<_T>& mx)
	{
		if (this->rows != mx.GetRows() || this->columns != mx.GetColumns())
			throw 1;

		for (int r = 0; r < this->rows; r++)
			for (int c = 0; c < this->columns; c++)
				this->data[r][c] = mx[r][c];

		this->isFree = false;

		return *this;
	}

	bool operator==(const Matrix<_T>& mx)
	{
		if (this->rows != mx.GetRows() || this->columns != mx.GetColumns())
			return false;

		for (int r = 0; r < this->rows; r++)
			for (int c = 0; c < this->columns; c++)
				if (this->data[r][c] != mx[r][c])
					return false;

		return true;
	}

	bool operator!=(const Matrix<_T>& mx)
	{
		return !(*this == mx);
	}

	void operator+=(const Matrix<_T>& mx)
	{
		*this = *this + mx;
	}

	void operator-=(const Matrix<_T>& mx)
	{
		*this = *this - mx;
	}

	void operator*=(const Matrix<_T>& mx)
	{
		*this = *this * mx;
	}
};