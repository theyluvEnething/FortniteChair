#include "sdk.h"
#include "../driver/driver.h"
#include "../../util/util.h"
#include "../data/offsets.h"
#include "../cheat.h"

Vector3 SDK::GetBoneWithRotation(uintptr_t mesh, int id)
{
	/*uintptr_t BoneA = driver::read<uintptr_t>(mesh + offset::BONE_ARRAY);
	if (BoneA == NULL)
	{
		BoneA = driver::read<uintptr_t>(mesh + offset::BONE_ARRAY + 0x10);
	}*/
	uint64_t BoneA = driver::read<uint64_t>(mesh + 0x5B0 + (driver::read<int>(mesh + 0x5F8) * 0x10)); // updated
	FTransform Bone = driver::read<FTransform>(BoneA + (id * offset::bonec));
	FTransform Comp = driver::read<FTransform>(mesh + offset::COMPONENT_TO_WORLD);
	D3DMATRIX matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), Comp.ToMatrixWithScale());
	return Vector3(matrix._41, matrix._42, matrix._43);
}

Vector3 SDK::GetBoneWithRotation(FTransform Comp, uintptr_t BoneA, int BoneId)
{
	FTransform Bone = driver::read<FTransform>(BoneA + (BoneId * offset::bonec));
	D3DMATRIX matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), Comp.ToMatrixWithScale());
	return Vector3(matrix._41, matrix._42, matrix._43);
}

struct CamewaDescwipsion
{
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
	char Useless[0x18];
};

Camera SDK::GetViewAngles()
{
	Camera view_point{};
	uintptr_t location_pointer = driver::read<uintptr_t>(cache::UWorld + 0x110);
	uintptr_t rotation_pointer = driver::read<uintptr_t>(cache::UWorld + 0x120);
	FQuat fnrot{};
	fnrot.x = driver::read<double>(rotation_pointer);
	fnrot.y = driver::read<double>(rotation_pointer + 0x20);
	fnrot.z = driver::read<double>(rotation_pointer + 0x1D0);
	view_point.Angle = Vector3(fnrot.x, fnrot.y, fnrot.z);
	view_point.Location = driver::read<Vector3>(location_pointer);
	view_point.Rotation.x = asin(fnrot.z) * (180.0 / M_PI);
	view_point.Rotation.y = ((atan2(fnrot.x * -1, fnrot.y) * (180.0 / M_PI)) * -1) * -1;
	view_point.FieldOfView = driver::read<float>(cache::UPlayerController + 0x394) * 90.f;
	return view_point;
}

Vector2 SDK::ProjectWorldToScreen(Vector3 WorldLocation)
{
	//get ur players newest view angles
	/*if (Debug::PrintLocations) {
		Util::Print3D("Location: ", vCamera.Location);
		Util::Print3D("Rotation: ", vCamera.Rotation);
		Util::Print2D("Fov: ", Vector2(vCamera.FieldOfView, 69));
	}*/

	D3DMATRIX tempMatrix = Matrix(cache::Camera.Rotation);

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = WorldLocation - cache::Camera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector2((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(cache::Camera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(cache::Camera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z);
}
