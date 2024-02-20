#pragma once

#include <rapidxml.hpp>

using DocumentType = rapidxml::xml_document<WCHAR>;
using AttributeType = rapidxml::xml_attribute<WCHAR>;

class XmlNode
{
	using NodeType = rapidxml::xml_node<WCHAR>;
public:
	XmlNode(NodeType* node = nullptr);

	__forceinline bool IsValid() { return m_node != nullptr; }

	XmlNode FindChild(StringView key);
	Vector<XmlNode> FindChildren(StringView key);
private:
	NodeType* m_node;
};

class XmlParser
{
public:
	bool ParseFromFile(StringView path, XmlNode& root);
private:
	std::shared_ptr<DocumentType> m_doc;
	String m_data;
};

