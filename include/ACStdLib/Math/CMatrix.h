/*
 * Copyright (c) 2017 Amir Czwink (amir130@hotmail.de)
 *
 * This file is part of ACStdLib.
 *
 * ACStdLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ACStdLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ACStdLib.  If not, see <http://www.gnu.org/licenses/>.
 */
#pragma once
//Local
#include "../Debug.h"
#include "../Definitions.h"

namespace ACStdLib
{
    namespace Math
    {
        template<typename ScalarType>
        class CMatrix
        {
        private:
            //Members
            uint32 nColumns;
            uint32 nRows;
            ScalarType *pValues; //row-major

        public:
            //Constructors
            inline CMatrix(uint32 nRows, uint32 nColumns)
            {
                this->nColumns = nColumns;
                this->nRows = nRows;
                this->pValues = new ScalarType[nRows * nColumns];
            }

            inline CMatrix(const CMatrix &refRight) //Copy constructor
            {
                this->pValues = nullptr;

                *this = refRight;
            }

            inline CMatrix(CMatrix &&refRight) //Move constructor
            {
                this->pValues = nullptr;

                *this = (CMatrix &&)refRight;
            }

            //Destructor
            inline ~CMatrix()
            {
                if(this->pValues)
                    delete[] this->pValues;
            }

            //Inline operators
            inline ScalarType &operator()(uint32 row, uint32 col)
            {
                ASSERT(col < this->nColumns, "Column out of bounds");
                ASSERT(row < this->nRows, "Row out of bounds");

                return this->pValues[row * this->nColumns + col];
            }

            inline const ScalarType &operator()(uint32 row, uint32 col) const
            {
                ASSERT(col < this->nColumns, "Column out of bounds");
                ASSERT(row < this->nRows, "Row out of bounds");

                return this->pValues[row * this->nColumns + col];
            }

            inline CMatrix operator*=(const CMatrix &refRight)
            {
                *this = (*this * refRight);

                return *this;
            }

            //Operators
            CMatrix &operator=(const CMatrix &refRight) //Copy assign
            {
                if(this->pValues)
                    delete[] this->pValues;

                this->nColumns = refRight.nColumns;
                this->nRows = refRight.nRows;
                this->pValues = new ScalarType[nRows * nColumns];

                MemCopy(this->pValues, refRight.pValues, nRows * nColumns * sizeof(ScalarType));

                return *this;
            }

            CMatrix &operator=(CMatrix &&refRight) //Move assign
            {
                if(this->pValues)
                    delete[] this->pValues;

                this->nColumns = refRight.nColumns;
                this->nRows = refRight.nRows;
                this->pValues = refRight.pValues;

                refRight.pValues = nullptr;

                return *this;
            }

            CMatrix operator*(const CMatrix &refRight) const
            {
                uint32 col, row, k;
                ScalarType sum;

                ASSERT(this->nColumns == refRight.nRows, "Matrix-multiplication requires that left matrices' number of columns are equal to right matrices number of rows'");

                CMatrix result(this->nRows, refRight.nColumns);

                for(row = 0; row < this->nRows; row++)
                {
                    for(col = 0; col < this->nColumns; col++)
                    {
                        sum = 0;
                        for(k = 0; k < refRight.nRows; k++)
                            sum += (*this)(row, k) * refRight(k, col);

                        result(row, col) = sum;
                    }
                }

                return result;
            }
        };

        //Functions
        template<typename ScalarType>
        CMatrix<ScalarType> Identity(uint32 n)
        {
            CMatrix<ScalarType> result(n, n);

            for(uint32 i = 0; i < n; i++)
                result(i, i) = 1;

            return result;
        }
    }
}