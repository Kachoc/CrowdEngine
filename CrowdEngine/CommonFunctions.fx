float CalculateNorm(float3 v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

float ComputeNorm(float3 v)
{
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

float CalculateSquareNorm(float3 v)
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

float ComputeSquareNorm(float3 v)
{
	return v.x*v.x + v.y*v.y + v.z*v.z;
}

float ComputeSpecCoef(float dotEyeReflection, float dotNormalLighRay, float specCoef)
// scalarProduct : résultat du produit scalaire entre les vecteurs direction du reflet spéculaire et pixel->oeil
// specularIntensity : caractéristique du matériau
{
	float specDot = max(dotEyeReflection, 0.0f);
	float specInt = pow(1.0f-specCoef*specCoef, 0.3f);
	return pow(specDot, specCoef*255.0f)*specInt*max(-dotNormalLighRay, 0);
}