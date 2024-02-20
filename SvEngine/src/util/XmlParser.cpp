#include "pch.h"
#include "Util/XmlParser.hpp"
#include "Util/File.hpp"

XmlNode::XmlNode(NodeType* node) : m_node(node)
{
}

XmlNode XmlNode::FindChild(StringView key)
{
	return m_node->first_node(key.data());;
}

Vector<XmlNode> XmlNode::FindChildren(StringView key)
{
	Vector<XmlNode> nodes;
	NodeType* node = m_node->first_node(key.data());
	while (node)
	{
		nodes.emplace_back(node);
		node = node->next_sibling(key.data());
	}
	return nodes;
}


bool XmlParser::ParseFromFile(StringView path, XmlNode& root)
{
	File xmlFile(path);
	auto bytes = xmlFile.Read();
	m_data = action::ToUnicodeString(std::string { bytes.begin(), bytes.end() });

	if (m_data.empty())
		return false;

	m_doc = std::make_shared<DocumentType>();
	m_doc->parse<0>(&m_data[0]);
	root = XmlNode(m_doc->first_node());
}