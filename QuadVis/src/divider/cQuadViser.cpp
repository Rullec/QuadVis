#include "cQuadViser.h"
#include <util/json/reader.h>
#include <iostream>
#include <fstream>
#include <geometry/cBaseMesh.h>

cQuadViser::cQuadViser(const std::string & path)
{
	// load conf
	Json::Value root, viser, coeff;
	std::ifstream fin(path);
	Json::Reader reader;
	reader.parse(fin, root);

	viser = root["QuadViser"];
	coeff = viser["Equation"];
	for (int i = 0; i < coeff.size(); i++)
	{
		mCoef[i] = coeff[i].asDouble();
	}

	// init buffer
	mFaces.reserve(10000);
	mFaces.clear();
}

void cQuadViser::Init()
{
	// 接下来将在原点为心，宽度为5的立方体范围内进行划分
	double x_bod[2], y_bod[2], z_bod[2];
	for(double x = -mCubeWidth / 2; x < mCubeWidth / 2; x+=mStep)
	for (double y = -mCubeWidth / 2; y < mCubeWidth / 2; y += mStep)
	{
		// check the intersection in this cube:
		// x: [x, x+step]; y: [y, y+step]; z:[z, z+step]
		x_bod[0] = x, x_bod[1] = x + mStep;
		y_bod[0] = y, y_bod[1] = y + mStep;
		mFindIntersect(x_bod, y_bod);
	}
}

void cQuadViser::mFindIntersect(double *x_bod, double * y_bod)
{
	double a = mCoef[0], b = mCoef[1], c = mCoef[2], d = mCoef[3];
	// 找8条边的解:
	double x, y, z_2, z_minus, z_positive;
	tPolygon cur_face;
	for (int i = 0; i < 2; i++)
	for (int j = 0; j < 2; j++)
	{
		x = x_bod[i], y = y_bod[j];
		z_2 = (-d - a * std::pow(x, 2) - b * std::pow(y, 2)) / c;
		if (z_2 < 1e-10) continue;
		else if (z_2 < 1e-5)
		{
			// only one point
			cur_face.mVertexLst.push_back(tVector(x, y, std::sqrt(z_2), 1));
		}
		else
		{
			z_minus = -sqrt(z_2);
			z_positive = -z_minus;
			cur_face.mVertexLst.push_back(tVector(x, y, z_minus, 1));
			cur_face.mVertexLst.push_back(tVector(x, y, z_positive, 1));
		}
	}
	if(cur_face.mVertexLst.size() > 0) mFaces.push_back(cur_face);
}

void cQuadViser::GetFaces(std::vector<tPolygon> & faces) const
{
	faces = mFaces;
}