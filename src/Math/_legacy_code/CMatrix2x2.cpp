//Operators
CMatrix2x2 CMatrix2x2::operator*(const CMatrix2x2 &refRight) const
{
	float64 current;
	CMatrix2x2 result;

	for(uint8 row = 0; row < 2; row++)
	{
		for(uint8 col = 0; col < 2; col++)
		{
			current = 0;
			for(uint8 k = 0; k < 2; k++)
			{
				current += (*this)(k, row) * refRight(col, k);
			}

			result(col, row) = (float32)current;
		}
	}

	return result;
}

CMatrix2x2 CMatrix2x2::Scale(float32 scaleX, float32 scaleY)
{
	CMatrix2x2 scale;

	scale(0, 0) = scaleX;
	scale(1, 1) = scaleY;

	return scale;
}
