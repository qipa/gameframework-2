//========================================================================
// Part of the GameCode3 Application
//
// GameCode3 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 3rd Edition" by Mike McShaffry, published by
// Charles River Media. ISBN-10: 1-58450-680-6   ISBN-13: 978-1-58450-680-5
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the author a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1584506806?ie=UTF8&tag=gamecodecompl-20&linkCode=as2&camp=1789&creative=390957&creativeASIN=1584506806
//
// There's a companion web site at:
// http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code:
// http://gamecode3.googlecode.com/svn/trunk/
//
// (c) Copyright 2009 Michael L. McShaffry
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License v2
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================

// /////////////////////////////////////////////////////////////////
// @file SceneNode.cpp
// @author Mike McShaffry & PJ O Halloran
// @date 04/10/2010
//
// Contains the SceneNode classes implementation.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <string>


// Project Headers
#include "SceneNode.h"

#include "GameMain.h"

#include "SceneGraphManager.h"


// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	SceneNode::SceneNode(boost::optional<ActorId> actorId, const std::string &name, const RenderPass renderPass, const Material &material, const Matrix4 &toWorld)\
		: m_parentPtr(NULL), m_props(), m_useCustomShader(false), m_children(), m_shaderPtr()
	{
		m_props.SetActorId(actorId);
		m_props.SetName(name);
		m_props.SetRenderPass(renderPass);
		m_props.SetMaterial(material);
		VSetTransform(toWorld);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	SceneNode::SceneNode(boost::optional<ActorId> actorId, const std::string &name, const RenderPass renderPass, const Material &material, const Matrix4 &toWorld, const Matrix4 &fromWorld)\
		: m_parentPtr(NULL), m_props(), m_useCustomShader(false), m_children(), m_shaderPtr()
	{
		m_props.SetActorId(actorId);
		m_props.SetName(name);
		m_props.SetRenderPass(renderPass);
		m_props.SetMaterial(material);
		VSetTransform(toWorld, fromWorld);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	SceneNode::~SceneNode()
	{
		// All children are shared ptrs so theres no need to explicitly clear the list.
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VPreRender(SceneGraphManager *scenePtr)
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VPreRender()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
									GameLog::ERR,\
									std::string("SceneNode::VPreRender()"),\
									std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		// Check if we need to get the shader (On the first time through or else we changed to a different shader recently).
		if(m_useCustomShader)
		{
			if(!m_shaderPtr)
			{
				m_shaderPtr = scenePtr->GetShader(m_props.GetShaderName());
				if(!m_shaderPtr)
				{
                    GF_LOG_TRACE_ERR("SceneNode::VPreRender()", std::string("Failed to get the custom shader from the SGM: ") + m_props.GetShaderName());
//					SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
											GameLog::ERR,\
											std::string("SceneNode::VPreRender()"),\
											std::string("Failed to get the custom shader from the SGM: ") + m_props.GetShaderName());
					return (false);
				}
			}

			// Activate the custom GLSL program we will be using to render the node.
			if(!m_shaderPtr->Activate())
			{
                GF_LOG_TRACE_ERR("SceneNode::VPreRender()", std::string("Failed to activate the custom shader: ") + m_props.GetShaderName());
//                SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(),\
                                        GameLog::ERR,\
                                        std::string("SceneNode::VPreRender()"),\
                                        std::string("Failed to activate the custom shader: ") + m_props.GetShaderName());
                return (false);
			}

			// Uniforms for the custom shader should be set in derived classes.
		}

		// Save the transformation state of the modelview matrix stack before we render 
		//  and set the new state to be the old matrix * this nodes matrix.
		scenePtr->GetStackManager()->GetModelViewMatrixStack()->PushMatrix();
		scenePtr->GetStackManager()->GetModelViewMatrixStack()->MultiplyMatrix(m_props.GetToWorld());
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VRender(SceneGraphManager *scenePtr)
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VRender()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VRender()"), std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		// Base implementation, TO be implemented by derived classes as only they will know exactly what to render.
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VPostRender(SceneGraphManager *scenePtr)
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VPostRender()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VPostRender()"), std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		// Restore the ModelView stacks state after render.
		scenePtr->GetStackManager()->GetModelViewMatrixStack()->PopMatrix();
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void SceneNode::VSetTransform(const Matrix4 &toWorld)
	{
		m_props.SetToWorld(toWorld);
		Matrix4 inv;
		if(!toWorld.Inversed(inv))
		{
            GF_LOG_TRACE_INF("SceneNode::VSetTransform()", "Failed to calculate an inverse for the nodes new toWorld matrix");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::INF, std::string("SceneNode::VSetTransform()"), std::string("Failed to calculate an inverse for the nodes new toWorld matrix"));
			inv.LoadIdentity();
		}

		m_props.SetFromWorld(inv);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	void SceneNode::VSetTransform(const Matrix4 &toWorld, const Matrix4 &fromWorld)
	{
		m_props.SetToWorld(toWorld);
		m_props.SetFromWorld(fromWorld);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VAddChild(boost::shared_ptr<ISceneNode> childNodePtr)
	{
		if(!childNodePtr)
		{
			return (false);
		}

		m_children.push_back(childNodePtr); 

		// Set the childs parent pointer to point to this SceneNode.
		childNodePtr->VSetParentPtr(this);

		// Calculate the new radius of the sphere enclosing this node and all children!
		Matrix4 childToWorld = childNodePtr->VGet()->GetToWorld();
		F32 childRadius = childNodePtr->VGet()->GetRadius();
		
		Point3 childPos, myPos;
		childToWorld.GetPosition(childPos);
		m_props.GetToWorld().GetPosition(myPos);
		
		// Vector to the child node.
		Vector3 dirToChild = childPos - myPos;

		F32 newRadius = dirToChild.Magnitude() + childRadius;
		if (newRadius > m_props.GetRadius())
		{
			m_props.SetRadius(newRadius);
		}

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VRemoveChild(const ActorId id)
	{
		bool found = false;
		for(SceneNodeList::iterator i = m_children.begin(); ((!found) && (i != m_children.end())); )
		{
			const SceneNodeProperties * const propsPtr = (*i)->VGet();
			if((propsPtr->GetActorId().is_initialized()) && (*propsPtr->GetActorId() == id))
			{
				i = m_children.erase(i);
				found = true;
			}
			else
			{
				++i;
			}
		}
		return (found);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SceneNode::RenderSceneNode(SceneGraphManager *scenePtr, ISceneNode *snPtr)
	{
		if(!snPtr || !scenePtr)
		{
			return;
		}

		F32 alpha = snPtr->VGet()->GetAlpha();
		if(FloatCmp(alpha, g_OPAQUE))
		{
			if(snPtr->VPreRender(scenePtr))
			{
				snPtr->VRender(scenePtr);
			}
			snPtr->VPostRender(scenePtr);
		}
		else if (!FloatCmp(alpha, g_TRANSPARENT))
		{
			// The object isn't totally transparent...
			Matrix4 mat;
			F32 z;

			// Get the top most MV transformation matrix.
			scenePtr->GetStackManager()->GetModelViewMatrixStack()->GetMatrix(mat);
			
			// Get our nodes position.
			Vector4 worldPos4;
			mat.GetPosition(worldPos4);
			
			// Get the camera matrix.
			Matrix4 toWorldCamera(scenePtr->GetCamera()->VGet()->GetToWorld());

			// Transform the nodes position into eye space.
			Vector4 eyePos4 = toWorldCamera * worldPos4;

			// TODO: Should we take into account the W component?? (remove this check when we resolve this...)
			if(!FloatCmp(eyePos4.GetW(), 1.0f))
			{
				// Log trace here to help answer our problem...
                GF_LOG_TRACE_DEB("SceneNode::VRenderChildren()", "The W component is non zero, recheck your work!!!!");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::DEB, std::string("SceneNode::VRenderChildren()"), std::string("The W component is non zero, recheck your work!!!!"));
			}
			z = eyePos4.GetZ();

			boost::shared_ptr<AlphaSceneNode> asn(GCC_NEW AlphaSceneNode(boost::shared_ptr<ISceneNode>(snPtr), mat, z));
			if(!asn)
			{
                GF_LOG_TRACE_ERR("SceneNode::VRenderChildren()", "Failed to allocate memory for an alpha scene node");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VRenderChildren()"), std::string("Failed to allocate memory for an alpha scene node"));
			}
			else
			{
				scenePtr->AddAlphaSceneNode(asn);
			}
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VRenderChildren(SceneGraphManager *scenePtr)
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VRenderChildren()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VRenderChildren()"), std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		bool result = true;
		for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i)
		{
			// Don't render this node if you can't see it
			if ((*i)->VIsVisible(scenePtr))
			{
				RenderSceneNode(scenePtr, (*i).get());
			}

			(*i)->VRenderChildren(scenePtr);
		}
		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VOnRestore(SceneGraphManager *scenePtr)
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VOnRestore()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VOnRestore()"), std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		bool result = true;
		for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i)
		{
			if(!(*i)->VOnRestore(scenePtr))
			{
				result = false;
			}
		}
		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VOnLostDevice(SceneGraphManager *scenePtr)
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VOnLostDevice()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VOnLostDevice()"), std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		bool result = true;
		for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i)
		{
			if(!(*i)->VOnLostDevice(scenePtr))
			{
				result = false;
			}
		}
		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VIsVisible(SceneGraphManager *scenePtr) const
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VIsVisible()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VIsVisible()"), std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		Matrix4 cameraTransform(scenePtr->GetCamera()->VGet()->GetToWorld());
		Matrix4 cameraInvTransform(scenePtr->GetCamera()->VGet()->GetFromWorld());
		
		// Position of the node in world space.
		Vector4 posWorld4;
		m_props.GetToWorld().GetPosition(posWorld4);

		// Transform the nodes position into camera space.
		Vector4 posCamEyeSpace4 = cameraTransform * posWorld4;

		// Convert position to Point3 by way of Vector3 first.
		Vector3 posCamEyeSpace3(posCamEyeSpace4);
		Point3 eyePt(posCamEyeSpace3);

		// Check if the node is inside the camera nodes Frustrum!
		return (scenePtr->GetCamera()->GetFrustum()->Inside(eyePt, m_props.GetRadius()));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VPick(SceneGraphManager *scenePtr, const RayCast &ray)
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VPick()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VPick()"), std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		if(m_children.empty())
		{
			return (false);
		}

		bool result = true;
		for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i)
		{
			if(!(*i)->VPick(scenePtr, ray))
			{
				result = false;
			}
		}
		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool SceneNode::VOnUpdate(SceneGraphManager *scenePtr, const F32 elapsedTime)
	{
		if(!scenePtr)
		{
			// Log error
            GF_LOG_TRACE_ERR("SceneNode::VOnUpdate()", "Failed to pass a valid SceneGraphManager pointer");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("SceneNode::VOnUpdate()"), std::string("Failed to pass a valid SceneGraphManager pointer"));
			return (false);
		}

		bool result = true;
		for(SceneNodeList::iterator i = m_children.begin(), end = m_children.end(); i != end; ++i)
		{
			if(!(*i)->VOnUpdate(scenePtr, elapsedTime))
			{
				result = false;
			}
		}
		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	Point3 SceneNode::GetPosition() const
	{
		Point3 pos;
		m_props.GetToWorld().GetPosition(pos);
		return (pos);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SceneNode::SetPosition(const Point3 &pos)
	{
		Matrix4 toWorld(m_props.GetToWorld());
		toWorld[Matrix4::M30] = pos.GetX();
		toWorld[Matrix4::M31] = pos.GetY();
		toWorld[Matrix4::M32] = pos.GetZ();
		toWorld[Matrix4::M33] = 1.0f;

		VSetTransform(toWorld);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void SceneNode::SetShaderName(const std::string &shaderNameRef)
	{
		// Mark the node as using a custom GLSL shader from now on.
		m_useCustomShader = true;
		m_props.SetShaderName(shaderNameRef);
		// Clear the current GLSL shader program so that on the next pre render call a new shader will be retrieved from the SGM
		m_shaderPtr.reset();
	};

}
