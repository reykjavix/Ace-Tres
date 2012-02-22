#include "MyMesh.h"

MyMesh::MyMesh() {
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_zPos = 0.0f;
	m_fPitch = 0.0f;
	m_fYaw = 0.0f;
	m_fRoll = 0.0f;
	m_mesh = NULL;
}

MyMesh::MyMesh(MODEL * mesh) {
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_zPos = 0.0f;
	m_fPitch = 0.0f;
	m_fYaw = 0.0f;
	m_fRoll = 0.0f;
	m_mesh = mesh;
}

void MyMesh::setMesh(MODEL * mesh) {
	m_mesh = mesh;
}

void MyMesh::setPosition(float x, float y, float z) {
	m_xPos = x;
	m_yPos = y;
	m_zPos = z;
}

void MyMesh::setPosition(D3DXVECTOR3 vec) {
	m_xPos = vec.x;
	m_yPos = vec.y;
	m_zPos = vec.z;
}

D3DXVECTOR3 MyMesh::getPosition() {
	return D3DXVECTOR3(m_xPos, m_yPos, m_zPos);
}

void MyMesh::setRotation(float pitch, float yaw, float roll) {
	m_fPitch = pitch;
	m_fYaw = yaw;
	m_fRoll = roll;
}

void MyMesh::setRotation(D3DXVECTOR3 rot) {
	m_fPitch = rot.x;
	m_fYaw = rot.y;
	m_fRoll = rot.z;
}

D3DXVECTOR3 MyMesh::getRotation() {
	return D3DXVECTOR3(m_fPitch, m_fYaw, m_fRoll);
}

MODEL* MyMesh::getModel()
{
	return m_mesh;
}

void MyMesh::Draw() {
	if (m_mesh == NULL) return;

	D3DXMATRIX pMatWorld;
	D3DXMATRIX MatTemp;  // Temp matrix for rotations.
    D3DXMATRIX MatRot;   // Final rotation matrix, applied to 
                         // pMatWorld.
 
    // Using the left-to-right order of matrix concatenation,
    // apply the translation to the object's world position
    // before applying the rotations.
    D3DXMatrixTranslation(&pMatWorld, m_xPos, m_yPos, m_zPos);
    D3DXMatrixIdentity(&MatRot);

    // Now, apply the orientation variables to the world matrix
    if(m_fPitch || m_fYaw || m_fRoll) {
        // Produce and combine the rotation matrices.
        D3DXMatrixRotationX(&MatTemp, m_fPitch);         // Pitch
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationY(&MatTemp, m_fYaw);           // Yaw
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
        D3DXMatrixRotationZ(&MatTemp, m_fRoll);          // Roll
        D3DXMatrixMultiply(&MatRot, &MatRot, &MatTemp);
 
        // Apply the rotation matrices to complete the world matrix.
        D3DXMatrixMultiply(&pMatWorld, &MatRot, &pMatWorld);
	}

	d3ddev->SetTransform(D3DTS_WORLD, &pMatWorld);
	DrawModel(m_mesh);
}