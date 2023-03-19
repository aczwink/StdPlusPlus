CMatrix4x4 CMatrix4x4::LookAtRH(const CVector3 &refCamPos, const CVector3 &refCamTarget, const CVector3 &refCamUpVector)
{
	CVector3 camDirection, right, yAxis;
	CMatrix4x4 lookAt;

	camDirection = (refCamPos - refCamTarget).Normalize();
	right = refCamUpVector.Cross(camDirection).Normalize();
	yAxis = camDirection.Cross(right);

	lookAt[0] = CVector4(right, -Dot(right, refCamPos));
	lookAt[1] = CVector4(yAxis, -Dot(yAxis, refCamPos));
	lookAt[2] = CVector4(camDirection, -Dot(camDirection, refCamPos));
	lookAt(3, 3) = 1;

	return lookAt.Transpose(); //TODO: OPTIMIZE THIS
}
