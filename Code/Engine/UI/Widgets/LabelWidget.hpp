#pragma once
#include "Engine/UI/WidgetBase.hpp"

class LabelWidget : public WidgetBase
{
public:
    //CONSTRUCTORS/////////////////////////////////////////////////////////////////////
    LabelWidget();
    virtual ~LabelWidget();

    //FUNCTIONS/////////////////////////////////////////////////////////////////////
    virtual void Update(float deltaSeconds) override;
    virtual void Render() const override;
    virtual void BuildFromXMLNode(XMLNode& node) override;
    virtual void RecalculateBounds() override;
};