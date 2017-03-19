#pragma once
#include "Engine\Renderer\2D\Renderable2D.hpp"
#include "Engine\Math\Transform2D.hpp"
#include "Engine\Renderer\RGBA.hpp"
#include "Engine\Renderer\AABB2.hpp"

class Material;

//-----------------------------------------------------------------------------------
class BarGraphRenderable2D : public Renderable2D
{
public:
    //CONSTRUCTORS/////////////////////////////////////////////////////////////////////
    BarGraphRenderable2D(const AABB2& bounds, RGBA filledColor, RGBA unfilledColor = RGBA::GRAY, int orderingLayer = 0, bool isEnabled = true);
    virtual ~BarGraphRenderable2D();

    //FUNCTIONS/////////////////////////////////////////////////////////////////////
    virtual void Update(float deltaSeconds);
    virtual AABB2 GetBounds();
    virtual void Render(BufferedMeshRenderer& renderer);

    //MEMBER VARIABLES/////////////////////////////////////////////////////////////////////
    Material* m_material;
    AABB2 m_bounds;
    Transform2D m_transform;
    RGBA m_fillColor;
    RGBA m_unfilledColor;
    float m_percentageFilled;
    bool m_isVertical = false;
};