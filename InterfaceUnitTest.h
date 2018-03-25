#pragma once

#include "Interface.h"
#include "gtest/gtest.h"

TEST(YamlInterfaceTest, Exception)
{
	YAML::Interface::Exception exceptionWithoutSource (YAML::Mark(), "message");
	YAML::Interface::Exception exceptionWithSource(YAML::Mark(), "message", "source");
	EXPECT_EQ(std::string(exceptionWithoutSource.what()), "yaml-cpp: error at line 1, column 1: message");
	EXPECT_EQ(std::string(exceptionWithoutSource.interfaceErrorMessage), "message");
	EXPECT_EQ(std::string(exceptionWithoutSource.source), "");

	EXPECT_EQ(std::string(exceptionWithSource.what()), "yaml-cpp: error at line 1, column 1: message, source");
	EXPECT_EQ(std::string(exceptionWithSource.interfaceErrorMessage), "message");
	EXPECT_EQ(std::string(exceptionWithSource.source), "source");
}

TEST(YamlInterfaceTest, CheckRequiredElement)
{
	YAML::Node node(YAML::Load("name: test\n"));
	EXPECT_NO_THROW(YAML::Interface::checkRequiredElement(node, "name", "string"));
	EXPECT_THROW(YAML::Interface::checkRequiredElement(node, "unpresent element", ""), YAML::Interface::MissingElement);
	try{ YAML::Interface::checkRequiredElement(node, "unpresent element", "type", "source");}
	catch (YAML::Interface::MissingElement e)
	{
		EXPECT_EQ(std::string(e.what()), "yaml-cpp: error at line 1, column 1: Missing Element: unpresent element a type, source");
		EXPECT_EQ(e.source, "source");
		EXPECT_EQ(e.requiredKey, "unpresent element");
	}
}
TEST(YamlInterfaceTest, CheckNodeType)
{
	YAML::Node node(YAML::Load("map: a"));
	EXPECT_NO_THROW(YAML::Interface::checkNodeType(node, YAML::NodeType::Map));
	EXPECT_THROW(YAML::Interface::checkNodeType(node, YAML::NodeType::Scalar), YAML::Interface::InvalidNodeType);
	try { YAML::Interface::checkNodeType(node, YAML::NodeType::Scalar, "source"); }
	catch (YAML::Interface::InvalidNodeType e)
	{
		EXPECT_EQ(std::string(e.what()), "yaml-cpp: error at line 1, column 1: Invalid Node Type: require Scalar, found Map, source");
		EXPECT_EQ(e.source, "source");
		EXPECT_EQ(e.requiredType , "Scalar");
	}
}

TEST(YamlInterfaceTest, SeekInFile)
{
	std::vector<YAML::Node> nodeTab = { YAML::Load("name: test1"), YAML::Load("name: test2") };
	EXPECT_NO_THROW(YAML::Interface::seekInFile(nodeTab, "fileName", "name", "test1"));
	EXPECT_NO_THROW(YAML::Interface::seekInFile(nodeTab, "fileName", "name", "test2"));
	EXPECT_THROW(YAML::Interface::seekInFile(nodeTab, "fileName", "name", "not in file"), YAML::Interface::NotFoundInFile);
	try { YAML::Interface::seekInFile(nodeTab, "fileName", "name", "not in file", "source"); }
	catch (YAML::Interface::NotFoundInFile e)
	{
		EXPECT_EQ(std::string(e.what()), "yaml-cpp: error at line 1, column 1: No name equal to \"not in file\" found in fileName, source");
		EXPECT_EQ(e.source, "source");
		EXPECT_EQ(e.keyName, "name");
		EXPECT_EQ(e.value, "not in file");
		EXPECT_EQ(e.fileName, "fileName");
	}

	EXPECT_THROW(YAML::Interface::seekInFile("fileName", "name", "not in file"), YAML::BadFile);
}

TEST(YamlInterfaceTest, checkElementType)
{
	YAML::Node node(YAML::Load("int: 1\nushort: a\nstring: [0, 1]"));
	EXPECT_NO_THROW(YAML::Interface::checkElementType<int>(node["int"], "int", ""));
	EXPECT_THROW(YAML::Interface::checkElementType<short>(node["ushort"], "ushort", ""), YAML::Interface::BadType);
	EXPECT_THROW(YAML::Interface::checkElementType<std::string>(node["string"], "string", ""), YAML::Interface::BadType);
	try { YAML::Interface::checkElementType<short>(node["ushort"], "ushort", ""); }
	catch (YAML::Interface::BadType e)
	{
		EXPECT_EQ(std::string(e.what()), "yaml-cpp: error at line 2, column 9: Bad Type: a is not a ushort");
		EXPECT_EQ(e.source, "");
		EXPECT_EQ(e.value, "a");
		EXPECT_EQ(e.requiredType, "ushort");
	}
	try { YAML::Interface::checkElementType<std::string>(node["string"], "string", "a source"); }
	catch (YAML::Interface::BadType e)
	{
		EXPECT_EQ(std::string(e.what()), "yaml-cpp: error at line 3, column 9: Bad Type: [0, 1] is not a string, a source");
		EXPECT_EQ(e.source, "a source");
		EXPECT_EQ(e.value, "[0, 1]");
		EXPECT_EQ(e.requiredType, "string");
	}
}