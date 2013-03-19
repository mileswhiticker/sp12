#include "Application.hpp"

#include <OGRE\OgreSceneManager.h>
#include <OGRE\OgreManualObject.h>
#include <OGRE\OgreMath.h>
#include <OGRE\OgreRenderOperation.h>
#include <OGRE\OgreHardwareVertexBuffer.h>
#include <OGRE\OgreHardwareBufferManager.h>
#include <OGRE\OgreMeshManager.h>
#include <OGRE\OgreSubMesh.h>
#include <OGRE\OgreEntity.h>
#include <OGRE\OgreSubEntity.h>

void Application::createSphere(const std::string& strName, const float r, const int nRings, const int nSegments, bool a_inverted)
{
	Ogre::MeshPtr pSphere = Ogre::MeshManager::getSingleton().createManual(strName + "_mesh", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	Ogre::SubMesh *pSphereVertex = pSphere->createSubMesh();
 
	pSphere->sharedVertexData = new Ogre::VertexData();
	Ogre::VertexData* vertexData = pSphere->sharedVertexData;
 
	// define the vertex format
	Ogre::VertexDeclaration* vertexDecl = vertexData->vertexDeclaration;
	size_t currOffset = 0;
	// positions
	vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_POSITION);
	currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	// normals
	vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT3, Ogre::VES_NORMAL);
	currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT3);
	// two dimensional texture coordinates
	vertexDecl->addElement(0, currOffset, Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES, 0);
	currOffset += Ogre::VertexElement::getTypeSize(Ogre::VET_FLOAT2);
 
	// allocate the vertex buffer
	vertexData->vertexCount = (nRings + 1) * (nSegments+1);
	Ogre::HardwareVertexBufferSharedPtr vBuf = Ogre::HardwareBufferManager::getSingleton().createVertexBuffer(vertexDecl->getVertexSize(0), vertexData->vertexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::VertexBufferBinding* binding = vertexData->vertexBufferBinding;
	binding->setBinding(0, vBuf);
	float* pVertex = static_cast<float*>(vBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
 
	// allocate index buffer
	pSphereVertex->indexData->indexCount = 6 * nRings * (nSegments + 1);
	pSphereVertex->indexData->indexBuffer = Ogre::HardwareBufferManager::getSingleton().createIndexBuffer(Ogre::HardwareIndexBuffer::IT_16BIT, pSphereVertex->indexData->indexCount, Ogre::HardwareBuffer::HBU_STATIC_WRITE_ONLY, false);
	Ogre::HardwareIndexBufferSharedPtr iBuf = pSphereVertex->indexData->indexBuffer;
	unsigned short* pIndices = static_cast<unsigned short*>(iBuf->lock(Ogre::HardwareBuffer::HBL_DISCARD));
 
	float fDeltaRingAngle = (Ogre::Math::PI / nRings);
	float fDeltaSegAngle = (2 * Ogre::Math::PI / nSegments);
	unsigned short wVerticeIndex = 0 ;
 
	// Generate the group of rings for the sphere
	for( int ring = 0; ring <= nRings; ring++ )
	{
		float r0 = r * sinf (ring * fDeltaRingAngle);
		float y0 = r * cosf (ring * fDeltaRingAngle);
 
		// Generate the group of segments for the current ring
		int multi = 1;
		if(a_inverted)
			multi = -1;
		for(int seg = 0; seg <= nSegments; seg++)
		{
			float x0 = r0 * sinf(multi * seg * fDeltaSegAngle);
			float z0 = r0 * cosf(multi * seg * fDeltaSegAngle);
 
			// Add one vertex to the strip which makes up the sphere
			*pVertex++ = x0;
			*pVertex++ = y0;
			*pVertex++ = z0;
 
			Ogre::Vector3 vNormal = Ogre::Vector3(x0, y0, z0).normalisedCopy();
			*pVertex++ = vNormal.x;
			*pVertex++ = vNormal.y;
			*pVertex++ = vNormal.z;
 
			*pVertex++ = (float) seg / (float) nSegments;
			*pVertex++ = (float) ring / (float) nRings;
 
			if (ring != nRings)
			{
				// each vertex (except the last) has six indices pointing to it
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex;               
				*pIndices++ = wVerticeIndex + nSegments;
				*pIndices++ = wVerticeIndex + nSegments + 1;
				*pIndices++ = wVerticeIndex + 1;
				*pIndices++ = wVerticeIndex;
				wVerticeIndex ++;
			}
		}; // end for seg
	} // end for ring
 
	// Unlock
	vBuf->unlock();
	iBuf->unlock();
	// Generate face list
	pSphereVertex->useSharedVertices = true;
 
	// the original code was missing this line:
	pSphere->_setBounds( Ogre::AxisAlignedBox( Ogre::Vector3(-r, -r, -r), Ogre::Vector3(r, r, r) ), false );
	pSphere->_setBoundingSphereRadius(r);
	// this line makes clear the mesh is loaded (avoids memory leaks)
	pSphere->load();
}
