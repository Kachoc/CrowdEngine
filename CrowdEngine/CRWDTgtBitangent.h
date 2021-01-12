#ifndef CRWD_TGT_BITANGENT_H_INCLUDED
#define CRWD_TGT_BITANGENT_H_INCLUDED

namespace Crowd
{

void CalculateTBNCalculateTBN(bool ConserveNormal, const Vector3 &P0, const Vector3 &P1, const Vector3 &P2, const Vector2 UV0, const Vector2 &UV1, const Vector2 &UV2, Vector3 &Normal, Vector3 &Tangent, float Handedness);

} // namespace Crowd

//#include "CRWDTgtBitangent.inl"

#endif // CRWD_TGT_BITANGENT_H_INCLUDED