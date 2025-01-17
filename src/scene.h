#pragma once

#include "maths.h"
#include "mesh.h"
#include "bvh.h"
#include "skylight.h"

#include <vector>

struct Camera
{
	Vec3 position;
	Quat rotation;

	float fov;

	// todo: lens options

	// todo: shutter options

};



struct Material
{
	Material() 
	{	
		color = SrgbToLinear(Color(0.82f, 0.67f, 0.16f));
		emission = Color(0.0f);
		metallic = 0.0;
		subsurface = 0.0f;
		specular = 0.5f;
		roughness = 0.5f;
		specularTint = 0.0f;
		anisotropic = 0.0f;
		sheen = 0.0f;
		sheenTint = 0.5f;
		clearcoat = 0.0f;
		clearcoatGloss = 1.0f;
	}

	Color emission;
	Color color;
	float metallic;
	float subsurface;
	float specular;
	float roughness;
	float specularTint;
	float anisotropic;
	float sheen;
	float sheenTint;
	float clearcoat;
	float clearcoatGloss;
};

enum GeometryType
{
	eSphere,
	ePlane,
	eMesh
};

struct SphereGeometry
{
	float radius;
};

struct PlaneGeometry
{
	float plane[4];
};

struct MeshGeometry
{
	const Vec3* positions;
	const Vec3* normals;
	const int* indices;
	const BVHNode* nodes;

	int numVertices;
	int numIndices;
	int numNodes;

	int id;
};


struct Primitive
{
	Primitive() : lightSamples(0) {}

	// begin end transforms for the primitive
	Transform startTransform;	
	Transform endTransform;
	
	Material material;
	GeometryType type;

	union
	{
		SphereGeometry sphere;
		PlaneGeometry plane;
		MeshGeometry mesh;
	};

	// if > 0 then primitive will be explicitly sampled
	int lightSamples;
};

struct Sky
{
	Color horizon;
	Color zenith;

	CUDA_CALLABLE Color Eval(const Vec3& dir) const
	{
		return Lerp(horizon, zenith, sqrtf(Abs(dir.y)));
	}

	// map
};

struct Scene
{
	// contiguous buffer for the data
	typedef std::vector<Primitive> PrimitiveArray;
	PrimitiveArray primitives;
	
	Sky sky;
	Camera camera;
	
	void AddPrimitive(const Primitive& p)
	{
		primitives.push_back(p);
	}	
};


