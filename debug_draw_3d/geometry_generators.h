#pragma once

#include <Godot.hpp>

#include <vector>

using namespace godot;

class GeometryGenerator {
public:

#pragma region Predefined Geometry Parts

	const static Vector3 CenteredCubeVertices[8];
	const static Vector3 CubeVertices[8];
	const static int CubeIndices[24];
	const static int CubeWithDiagonalsIndices[36];
	const static Vector3 ArrowheadVertices[6];
	const static int ArrowheadIndices[16];
	const static Vector3 CenteredSquareVertices[4];
	const static int SquareIndices[6];
	const static Vector3 PositionVertices[6];
	const static int PositionIndices[6];

#pragma endregion

	static std::vector<Vector3> CreateCameraFrustumLines(Plane frustum[6]);
	static std::vector<Vector3> CreateCubeLines(Vector3 position, Quat rotation, Vector3 size, bool centeredBox = true, bool withDiagonals = false);
	static std::vector<Vector3> CreateSphereLines(int lats, int lons, float radius, Vector3 position);
	static std::vector<Vector3> CreateCylinderLines(int edges, float radius, float height, Vector3 position, int drawEdgeEachNStep = 1);
	static std::vector<Vector3> CreateLinesFromPath(PoolVector3Array path);
};