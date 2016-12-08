#include "Engine/UI/UISystem.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Renderer/AABB2.hpp"
#include "Engine/Fonts/BitmapFont.hpp"
#include "Engine/Input/InputOutputUtils.hpp"
#include "Widgets/LabelWidget.hpp"
#include "Widgets/ButtonWidget.hpp"

UISystem* UISystem::instance = nullptr;

//-----------------------------------------------------------------------------------
UISystem::UISystem()
{
    LoadAndParseUIXML();
}

//-----------------------------------------------------------------------------------
UISystem::~UISystem()
{

}

//-----------------------------------------------------------------------------------
void UISystem::Update(float deltaSeconds)
{
    for (WidgetBase* widget : m_childWidgets)
    {
        widget->Update(deltaSeconds);
    }
}

//-----------------------------------------------------------------------------------
void UISystem::Render() const
{
    Renderer::instance->BeginOrtho(Vector2::ZERO, Vector2(1600, 900));
    {
        for (WidgetBase* widget : m_childWidgets)
        {
            widget->Render();
        }
    }
    Renderer::instance->EndOrtho();
}

//-----------------------------------------------------------------------------------
void UISystem::LoadAndParseUIXML()
{
    XMLNode root = XMLUtils::OpenXMLDocument("Data/UI/Widget.xml");
    std::vector<XMLNode> children = XMLUtils::GetChildren(root);
    for (XMLNode& node : children)
    {
        if (!node.isEmpty())
        {
            m_childWidgets.push_back(CreateWidget(node));
        }
    }
}

//-----------------------------------------------------------------------------------
WidgetBase* UISystem::CreateWidget(XMLNode& node)
{
    std::string nodeName = node.getName();
    WidgetBase* widget = CreateWidget(nodeName);
    widget->BuildFromXMLNode(node);
    return widget;
}

//-----------------------------------------------------------------------------------
WidgetBase* UISystem::CreateWidget(const std::string& name)
{
    if (name == "Label")
    {
        return static_cast<WidgetBase*>(new LabelWidget());
    }
    else if (name == "Button")
    {
        return static_cast<WidgetBase*>(new ButtonWidget());
    }
    return nullptr;
}

