// /////////////////////////////////////////////////////////////////
// @file AbstractWidget.cpp
// @author PJ O Halloran
// @date 20/10/2010
//
// Base level class for my OpenGL based UI library.
//
// /////////////////////////////////////////////////////////////////

// External Headers
#include <boost/lexical_cast.hpp>


// Project Headers
#include "AbstractWidget.h"

#include "GameMain.h"
#include "TextResource.h"
#include "LuaStateManager.h"

////////////////////////// TEMPORARY TEMPORARY TEMPORARY - On SnowLeopard this is suppored, but GLEW doens't hook up properly
////////////////////////// Fixed probably in 10.6.3
//#ifdef __APPLE__
//#define glGenVertexArrays glGenVertexArraysAPPLE
//#define glDeleteVertexArrays  glDeleteVertexArraysAPPLE
//#define glBindVertexArray	glBindVertexArrayAPPLE
//#endif

// /////////////////////////////////////////////////////////////////
//
//
// /////////////////////////////////////////////////////////////////
namespace GameHalloran
{

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::SetupQuad()
	{
		static bool vaoError = false, vboError = false;
		if(vaoError || vboError)
		{
			// GL failed to generate a VAO or VBO for us so theres a serious rendering problem that needs debugging right away to get this app to run!
            GF_LOG_TRACE_FAT("AbstractWidget::SetupQuad()", "Failed to generate a VAO or VBO previously");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::FAT, std::string("AbstractWidget::SetupQuad()"), std::string("Failed to generate a VAO or VBO previously"));
			return;
		}
		if(glIsVertexArray(m_vaoId) == GL_FALSE)
		{
            GF_LOG_TRACE_DEB("AbstractWidget::SetupQuad()", "No VOA exists yet so will generate one now");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::DEB, std::string("AbstractWidget::SetupQuad()"), std::string("No VOA exists yet so will generate one now"));
			glGenVertexArrays(1, &m_vaoId);
			if(m_vaoId == 0)
			{
                GF_LOG_TRACE_ERR("AbstractWidget::SetupQuad()", "Failed to generate the VAO");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::SetupQuad()"), std::string("Failed to generate the VAO"));
				vaoError = true;
				return;
			}
		}
		if(glIsBuffer(m_vboId) == GL_FALSE)
		{
            GF_LOG_TRACE_DEB("AbstractWidget::SetupQuad()", "No VBO exists yet so will generate one now");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::DEB, std::string("AbstractWidget::SetupQuad()"), std::string("No VBO exists yet so will generate one now"));
			glGenBuffers(1, &m_vboId);
			if(m_vboId == 0)
			{
                GF_LOG_TRACE_ERR("AbstractWidget::SetupQuad()", "Failed to generate the VBO");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::SetupQuad()"), std::string("Failed to generate the VBO"));
				vboError = true;
				return;
			}
		}

		// Bind to the VBO so we can copy data to the area of GPU memory.
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);

		// Geometry setup here for a quad
		GLfloat positions[] = {m_position.GetX(), m_position.GetY(), 0.0f, 1.0f, /* Top Left */
									m_position.GetX(), m_position.GetY() - m_height, 0.0f, 1.0f, /* Bottom Left */
									m_position.GetX() + m_width, m_position.GetY() - m_height, 0.0f, 1.0f, /* Bottom Right */
									m_position.GetX() + m_width, m_position.GetY(), 0.0f, 1.0f /* Top Right */
								};
		//GLfloat texCoords[] = {1.0f, 0.0f, /* Top Left */
		//							1.0f, 1.0f, /* Bottom Left */
		//							0.0f, 1.0f, /* Bottom Right */
		//							0.0f, 0.0f /* Top Right */
		//							};
		GLfloat texCoords[] = {0.0f, 1.0f, /* Top Left */
									0.0f, 0.0f, /* Bottom Left */
									1.0f, 0.0f, /* Bottom Right */
									1.0f, 1.0f /* Top Right */
									};

		// Allocate memory and a type for the currently bound VBO.
		glBufferData(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(texCoords), NULL, GL_STATIC_DRAW);

		// Copy the quad data into the VBO.
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(positions), positions);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(positions), sizeof(texCoords), texCoords);

		// Bind to the widgets VAO to save the vertex attributes state to speed up rendering.
		glBindVertexArray(m_vaoId);

		// Now set the Vertex Shader Attributes information.
		// 1) Set offset of geometry vertices for the currently bound VBO.
		GLuint index = 0;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)0);
		// 2) Set offset of the geometry tex coords for the currently bound VBO (index == 3 to be compatible with GLT Batch and GLT shaders format).
		index = 3;
		glEnableVertexAttribArray(index);
		glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid *)sizeof(positions));

		// Unbind after geometry is assembled and sent to the GPU.
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::Init(const std::string &textureNameRef) throw (GameException &)
	{
		m_applyTexture = !textureNameRef.empty();
		if(!m_applyTexture && !m_flatShaderProg)
		{
			throw GameException(std::string("Flat Shader object passed to widget is NULL"));
		}
		else if(m_applyTexture && !m_texShaderProg)
		{
			throw GameException(std::string("Texture Shader object passed to widget is NULL"));
		}

		// Get the shader uniform locations here for use later...
		if(m_flatShaderProg)
		{
			m_projLoc = m_flatShaderProg->GetUniformLocation("mvpMatrix");
			if(m_projLoc == -1)
			{
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to find the mvpMatrix position");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::Init()"), std::string("Failed to find the mvpMatrix position"));
			}
			m_colorLoc = m_flatShaderProg->GetUniformLocation("colorVec");
			if(m_colorLoc == -1)
			{
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to find the colorVec position");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::Init()"), std::string("Failed to find the colorVec position"));
			}
		}
		if(m_texShaderProg)
		{
			m_projLoc = m_texShaderProg->GetUniformLocation("projMatrix");
			if(m_projLoc == -1)
			{
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to find the projMatrix position");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::Init()"), std::string("Failed to find the projMatrix position"));
			}
			m_samplerLoc = m_texShaderProg->GetUniformLocation("colorMap");
			if(m_samplerLoc == -1)
			{
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to find the colorMap position");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::Init()"), std::string("Failed to find the colorMap position"));
			}
			m_alphaLoc = m_texShaderProg->GetUniformLocation("uiAlpha");
			if(m_alphaLoc == -1)
			{
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to find the uiAlpha position");
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::Init()"), std::string("Failed to find the uiAlpha position"));
			}
		}

		m_bb.SetMin(Point3(m_position.GetX(), m_position.GetY() - m_height, 0.0f));
		m_bb.SetMax(Point3(m_position.GetX() + m_width, m_position.GetY(), 0.0f));

		if(m_applyTexture)
		{
			boost::optional<TexHandle> th = g_appPtr->GetTextureManagerPtr()->Load2D(textureNameRef, GL_CLAMP_TO_EDGE);
			if(th.is_initialized())
			{
				m_tHandle = *th;
				m_currentTextureHandle = m_tHandle;
			}
			else
			{
#if DEBUG
				std::string idStr;
				try { idStr = boost::lexical_cast<std::string, ScreenElementId>(m_id); } catch(...) { }
                GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to generate the texture for the widget " + idStr);
#endif
//				SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::Init()"), std::string("Failed to generate the texture for the widget ") + idStr);
			}
		}

		glGenVertexArrays(1, &m_vaoId);
		if(m_vaoId == 0)
		{
#if DEBUG
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ScreenElementId>(m_id); } catch(...) { }
            GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to generate the VAO for the widget " + idStr);
#endif
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::Init()"), std::string("Failed to generate the VAO for the widget ") + idStr);
		}
		glGenBuffers(1, &m_vboId);
		if(m_vboId == 0)
		{
#if DEBUG
			std::string idStr;
			try { idStr = boost::lexical_cast<std::string, ScreenElementId>(m_id); } catch(...) { }
            GF_LOG_TRACE_ERR("AbstractWidget::Init()", "Failed to generate the VBO for the widget " + idStr);
#endif
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::Init()"), std::string("Failed to generate the VBO for the widget ") + idStr);
		}

		SetupQuad();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::SetLuaPosition(const LuaPlus::LuaObject &table)
	{
		if(SetPoint3FromLua(table, m_position))
		{
			LuaPlus::LuaObject posType = table.GetByName("Type");
			if(posType.IsString())
			{
#ifdef DEBUG
				if((strcmp(posType.GetString(), "relative") != 0) && (strcmp(posType.GetString(), "absolute") != 0))
				{
                    GF_LOG_TRACE_ERR("AbstractWidget::SetLuaPosition()", std::string("The Type parameter from Position is invalid: ") + posType.GetString());
//					SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::SetLuaPosition()"), std::string("The Type parameter from Position is invalid: ") + std::string(posType.GetString()));
				}
#endif

				F32 x(m_position.GetX()), y(m_position.GetY()), z(m_position.GetZ());
				if(strcmp(posType.GetString(), "relative") == 0)
				{
					const F32 sw = static_cast<F32>(g_appPtr->GetWindowManager()->GetWidth());
					const F32 sh = static_cast<F32>(g_appPtr->GetWindowManager()->GetHeight());
					Clamp<F32>(x, 0.0f, 1.0f);
					Clamp<F32>(y, 0.0f, 1.0f);
					Clamp<F32>(z, 0.0f, 1.0f);
					x = InterpolateFloat(x, 0.0f, sw);
					y = InterpolateFloat(y, 0.0f, sh);
					z = InterpolateFloat(z, -1.0f, 1.0f);
					m_position.Set(x, y, z);
				}
			}

			// TODO: Change this Z behaviour if required, its easier for now to fix it!
			// At the moment all our UI elements are fixed at 0.0f on the xy plane.
			m_position.SetZ(0.0f);
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::SetLuaDimensions(const LuaPlus::LuaObject &table)
	{
		if(table.IsTable())
		{
			LuaPlus::LuaObject dimType = table.GetByName("Type");
			LuaPlus::LuaObject dimW = table.GetByName("Width");
			LuaPlus::LuaObject dimH = table.GetByName("Height");
			LuaPlus::LuaObject dimD = table.GetByName("Depth");

			F32 w(0.0f), h(0.0f), d(0.0f);
			if(dimW.IsNumber())
			{
				w = static_cast<F32>(dimW.GetNumber());
			}
			if(dimH.IsNumber())
			{
				h = static_cast<F32>(dimH.GetNumber());
			}
			if(dimD.IsNumber())
			{
				d = static_cast<F32>(dimD.GetNumber());
			}

			if(dimType.IsString())
			{
#ifdef DEBUG
				if((strcmp(dimType.GetString(), "relative") != 0) && (strcmp(dimType.GetString(), "absolute") != 0))
				{
                    GF_LOG_TRACE_ERR("AbstractWidget::SetLuaDimensions()", std::string("The Type parameter from Dimensions is invalid: ") + dimType.GetString());
//					SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::SetLuaDimensions()"), std::string("The Type parameter from Dimensions is invalid: ") + std::string(dimType.GetString()));
				}
#endif

				if(strcmp(dimType.GetString(), "relative") == 0)
				{
					const F32 sw = static_cast<F32>(g_appPtr->GetWindowManager()->GetWidth());
					const F32 sh = static_cast<F32>(g_appPtr->GetWindowManager()->GetHeight());
					Clamp<F32>(w, 0.0f, 1.0f);
					Clamp<F32>(h, 0.0f, 1.0f);
					Clamp<F32>(d, 0.0f, 1.0f);
					w = InterpolateFloat(w, 0.0f, sw);
					h = InterpolateFloat(h, 0.0f, sh);
					d = InterpolateFloat(d, -1.0f, 1.0f);
				}
			}

			// TODO: Change this Z behaviour if required, its easier for now to fix it!
			// At the moment all our UI elements are fixed at a depth of 0.0f.
			d = 0.0f;
			m_width = w;
			m_height = h;
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	AbstractWidget::AbstractWidget(const Point3 &posRef,\
									const Vector4 &colorRef,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									const F32 width,\
									const F32 height,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									const std::string &textureNameRef,\
									const bool visible,\
									const ScreenElementId id) throw (GameException &)\
		: m_position(posRef), m_visible(visible), m_color(colorRef), m_id(id), m_width(width), m_height(height), m_applyTexture(false),\
			m_vaoId(0), m_vboId(0), m_tHandle(0), m_mvpStackManagerPtr(mvpStackManPtr), m_texShaderProg(shaderTexObj), m_flatShaderProg(shaderFlatObj),\
				m_projLoc(-1), m_colorLoc(-1), m_samplerLoc(-1), m_alphaLoc(-1), m_projMatrix(), m_bb(), m_currentTextureHandle(0)
	{
		if(m_width < 0.0f)
		{
			m_width = -m_width;
		}
		if(m_height < 0.0f)
		{
			m_height = -m_height;
		}

		Init(textureNameRef);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	AbstractWidget::AbstractWidget(const LuaPlus::LuaObject &widgetScriptData,\
									boost::shared_ptr<ModelViewProjStackManager> mvpStackManPtr,\
									const boost::shared_ptr<GLSLShader> shaderFlatObj,\
									const boost::shared_ptr<GLSLShader> shaderTexObj,\
									const ScreenElementId id) throw (GameException &)\
		: m_position(), m_visible(true), m_color(), m_id(id), m_width(0.0f), m_height(0.0f), m_applyTexture(false),	m_vaoId(0), m_vboId(0), m_tHandle(0),\
			m_mvpStackManagerPtr(mvpStackManPtr), m_texShaderProg(shaderTexObj), m_flatShaderProg(shaderFlatObj), m_projLoc(-1), m_colorLoc(-1), m_samplerLoc(-1),\
				m_alphaLoc(-1), m_projMatrix(), m_bb(), m_currentTextureHandle(0)
	{
		if(!widgetScriptData.IsTable())
		{
			throw GameException(std::string("Lua data is of an invalid type"));
		}

		std::string texName;
		SetLuaPosition(widgetScriptData["Position"]);
		SetColorFromLua(widgetScriptData["Color"], m_color);
		SetLuaDimensions(widgetScriptData["Dimension"]);
		SetStringFromLua(widgetScriptData["TextureName"], texName);
		SetBoolFromLua(widgetScriptData["Visible"], m_visible);

		Init(texName);
	}

	// /////////////////////////////////////////////////////////////////
	// 
	// /////////////////////////////////////////////////////////////////
	AbstractWidget::~AbstractWidget()
	{
		try
		{
			if(glIsBuffer(m_vboId) == GL_TRUE)
			{
				glDeleteBuffers(1, &m_vboId);
			}
			if(glIsVertexArray(m_vaoId) == GL_TRUE)
			{
				glDeleteVertexArrays(1, &m_vaoId);
			}
		}
		catch(...)
		{
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool AbstractWidget::VOnRestore()
	{
		SetupQuad();
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::VOnLostDevice()
	{
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::VOnResolutionChange(const I32 oldWidth, const I32 oldHeight, const I32 newWidth, const I32 newHeight)
	{
		Point3 oldWidgetPos(m_position);
		F32 oldWidgetWidth(m_width), oldWidgetHeight(m_height);

		F32 oldFactorX(oldWidgetPos.GetX() / F32(oldWidth));
		F32 oldFactorY(oldWidgetPos.GetY() / F32(oldHeight));
		// Z not needed...

		F32 oldFactorWidth(oldWidgetWidth / F32(oldWidth));
		F32 oldFactorHeight(oldWidgetHeight / F32(oldHeight));

		F32 newX = InterpolateFloat(oldFactorX, 0.0, newWidth);
		F32 newY = InterpolateFloat(oldFactorY, 0.0, newHeight);
		Point3 newPosition(newX, newY, oldWidgetPos.GetZ());
		//GameHalloran::ConvertWindowCoordinates(newPosition);
		F32 newW = InterpolateFloat(oldFactorWidth, 0.0, newWidth);
		F32 newH = InterpolateFloat(oldFactorHeight, 0.0, newHeight);

		SetPositionHelper(newPosition);
		SetWidthHelper(newW);
		SetHeightHelper(newH);
		SetupQuad();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool AbstractWidget::PreRenderTexturedWidget()
	{
		if(!m_texShaderProg->Activate())
		{
            GF_LOG_TRACE_ERR("AbstractWidget::PreRenderTexturedWidget()", "Failed to activate the shader");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::PreRenderTexturedWidget()"), std::string("Failed to activate the shader"));
			return (false);
		}

		glUniform1i(m_samplerLoc, 0);
		glUniform1f(m_alphaLoc, m_color.GetW());
		glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, m_projMatrix.GetComponentsConst());

		g_appPtr->GetTextureManagerPtr()->Bind(m_currentTextureHandle, GL_TEXTURE_2D);

		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool AbstractWidget::PreRenderFlatWidget(boost::optional<Vector4> opColor)
	{
		if(!m_flatShaderProg->Activate())
		{
            GF_LOG_TRACE_ERR("AbstractWidget::PreRenderFlatWidget()", "Failed to activate the shader");
//			SafeGameLogAndPrefix(g_appPtr->GetLoggerPtr(), GameLog::ERR, std::string("AbstractWidget::PreRenderFlatWidget()"), std::string("Failed to activate the shader"));
			return (false);
		}

		glUniformMatrix4fv(m_projLoc, 1, GL_FALSE, m_projMatrix.GetComponentsConst());
		if(!opColor.is_initialized())
		{
			glUniform4fv(m_colorLoc, 1, m_color.GetComponentsConst());
		}
		else
		{
			glUniform4fv(m_colorLoc, 1, (*opColor).GetComponentsConst());
		}
		return (true);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	bool AbstractWidget::VOnRender(const F64 time, const F32 elapsedTime)
	{
		bool result = true;

		if(m_visible)
		{
			// Get the current proj matrix only.
			m_mvpStackManagerPtr->GetModelViewProjectionMatrix(m_projMatrix);

			if(m_applyTexture)
			{
				result = PreRenderTexturedWidget();
			}
			else
			{
				result = PreRenderFlatWidget();
			}

			// Bind to the VAO used to assemble the widgets geometry.
			glBindVertexArray(m_vaoId);
			// Send the geometry to the vertex shader.
			glDrawArrays(GL_TRIANGLE_FAN, 0, NUM_VERTICES);
			// Unbind after render
			glBindVertexArray(0);
		}

		return (result);
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::SetPositionHelper(const Point3 &posRef)
	{
		m_position = posRef;
		m_bb.SetMin(Point3(m_position.GetX(), m_position.GetY() - m_height, 0.0f));
		m_bb.SetMax(Point3(m_position.GetX() + m_width, m_position.GetY(), 0.0f));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::VSetPosition(const Point3 &posRef)
	{
		SetPositionHelper(posRef);
		SetupQuad();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::VSetPositionType(const Point3 &posRef, const bool normalMode)
	{
		if(normalMode)
		{
			VSetPosition(posRef);
		}
		else
		{
			F32 x(posRef.GetX()), y(posRef.GetY()), z(posRef.GetZ());
			const F32 sw = static_cast<F32>(g_appPtr->GetWindowManager()->GetWidth());
			const F32 sh = static_cast<F32>(g_appPtr->GetWindowManager()->GetHeight());
			Clamp<F32>(x, 0.0f, 1.0f);
			Clamp<F32>(y, 0.0f, 1.0f);
			Clamp<F32>(z, 0.0f, 1.0f);
			x = InterpolateFloat(x, 0.0f, sw);
			y = InterpolateFloat(y, 0.0f, sh);
			z = InterpolateFloat(z, -1.0f, 1.0f);
			VSetPosition(Point3(x, y, z));
		}
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::SetWidthHelper(const F32 width)
	{
		if(width < 0.0f)
		{
			m_width = -width;
		}
		else
		{
			m_width = width;
		}

		m_bb.SetMin(Point3(m_position.GetX(), m_position.GetY() - m_height, 0.0f));
		m_bb.SetMax(Point3(m_position.GetX() + m_width, m_position.GetY(), 0.0f));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::VSetWidth(const F32 width)
	{
		SetWidthHelper(width);
		SetupQuad();
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::SetHeightHelper(const F32 height)
	{
		if(height < 0.0f)
		{
			m_height = -height;
		}
		else
		{
			m_height = height;
		}

		m_bb.SetMin(Point3(m_position.GetX(), m_position.GetY() - m_height, 0.0f));
		m_bb.SetMax(Point3(m_position.GetX() + m_width, m_position.GetY(), 0.0f));
	}

	// /////////////////////////////////////////////////////////////////
	//
	// /////////////////////////////////////////////////////////////////
	void AbstractWidget::VSetHeight(const F32 height)
	{
		SetHeightHelper(height);
		SetupQuad();
	}

}
