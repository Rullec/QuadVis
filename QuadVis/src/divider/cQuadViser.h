#pragma once
#include <string>
#include <util/cGeoUtil.hpp>
#include <geometry/cBaseMesh.h>
#include <vector>


class cQuadViser {
public:
	cQuadViser(const std::string & conf);
	void Init();
	void GetFaces(std::vector<tPolygon> & faces) const;

private:
	tVector mCoef;

	// divide info
	const double mCubeWidth = 1.4;
	const double mStep = 0.01;

	// buffer and cnting
	std::vector<tPolygon> mFaces;

	// intersect tool
	void mFindIntersect(double *x, double * y);
};