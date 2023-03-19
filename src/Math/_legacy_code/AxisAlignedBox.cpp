float32 AxisAlignedBox::SquaredDistanceTo(const vec4f32 &refPoint) const
{
	CVector3 maxDist;
	
	if(this->Contains(refPoint))
		return 0;

	if(refPoint.x < this->min.x)
		maxDist.x = this->min.x - refPoint.x;
	else if(refPoint.x > this->max.x)
		maxDist.x = refPoint.x - this->max.x;

	if(refPoint.y < this->min.y)
		maxDist.y = this->min.y - refPoint.y;
	else if(refPoint.y > this->max.y)
		maxDist.y = refPoint.y - this->max.y;

	if(refPoint.z < this->min.z)
		maxDist.z = this->min.z - refPoint.z;
	else if(refPoint.z > this->max.z)
		maxDist.z = refPoint.z - this->max.z;

	return maxDist.SquaredLength();
}
