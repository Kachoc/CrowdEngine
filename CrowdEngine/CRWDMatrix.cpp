#include "CRWDMatrix.h"
#include "D3DEntry.h"

namespace Crowd
{

Matrix &Matrix::SMultiply(Matrix &out, const Matrix &m1, const Matrix &m2)
{
	if(m1._34 < 0)
	{
		if(m2._34 < 0)
		{
			return *(Matrix*) D3DXMatrixMultiply((D3DXMATRIX*) &out, (D3DXMATRIX*) &(-m1), (D3DXMATRIX*) &(-m2));
		}
		else
		{
			out = -(*(Matrix*) D3DXMatrixMultiply((D3DXMATRIX*) &out, (D3DXMATRIX*) &(-m1), (D3DXMATRIX*) &m2));
			return out;
		}
	}
	else if(m2._34 < 0)
	{
		out = -(*(Matrix*) D3DXMatrixMultiply((D3DXMATRIX*) &out, (D3DXMATRIX*) &m1, (D3DXMATRIX*) &(-m2)));
		return out;
	}
	else
	{
		return *(Matrix*) D3DXMatrixMultiply((D3DXMATRIX*) &out, (D3DXMATRIX*) &m1, (D3DXMATRIX*) &m2);
	}
}

} // namespace Crowd