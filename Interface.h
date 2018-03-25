#pragma once
//Bibliotèque dynamique
#define YAML_CPP_DLL

#include <string>
#include <exception>
#include <fstream>
#include "yaml-cpp/yaml.h"
#include "Exception.h"

namespace YAML
{
	namespace Interface
	{
		//! Function that check if the required element is present in the file
		/*!
		\param node Node from yaml-cpp that need to contain the element
		\param key string key of the element required
		\param type string that contain the type of the element for the error message, if the element is missing
		\param source the error's source.
		*/
		inline void checkRequiredElement(const YAML::Node& node, const std::string& key, const std::string type, const std::string& source = "")
		{
			if (!node.IsMap() || !node[key])
			{

				throw MissingElement(node.Mark(), key, type, source);
			}
		}

		//! Function that check if the required element is present in the file
		/*!
		\param node Node from yaml-cpp that need to contain the element
		\param key string key of the element required
		\param type string that contain the type of the element for the error message, if the element is missing
		\param source the error's source.
		*/
		inline void checkNodeType(const YAML::Node& node, YAML::NodeType::value nodeType, std::string source = "")
		{
			if (node.Type() != nodeType)
			{
				std::string requiredType("");
				switch (nodeType)
				{
				case YAML::NodeType::Map:
					requiredType = "Map";
					break;
				case YAML::NodeType::Sequence:
					requiredType = "Sequence";
					break;
				case YAML::NodeType::Scalar:
					requiredType = "Scalar";
					break;
				case YAML::NodeType::Null:
					requiredType = "Null";
					break;
				default: requiredType = "Undefined";
				}
				std::string foundType("");
				switch (node.Type())
				{
				case YAML::NodeType::Map:
					foundType = "Map";
					break;
				case YAML::NodeType::Sequence:
					foundType = "Sequence";
					break;
				case YAML::NodeType::Scalar:
					foundType = "Scalar";
					break;
				case YAML::NodeType::Null:
					foundType = "Null";
					break;
				default: foundType = "Undefined";
				}

				throw InvalidNodeType(node.Mark(), requiredType, foundType, source);
			}
		}

		inline std::vector<YAML::Node>::iterator seekInFile(std::vector<YAML::Node>& file, const std::string& fileName,
			const std::string& keyName, const std::string& value, const std::string& source = "")
		{
			auto fileIterator = file.begin();
			while (fileIterator != file.end())
			{
				try
				{
					if (fileIterator->IsMap() && fileIterator->operator[](keyName) && fileIterator->operator[](keyName).as<std::string>() == value) break;
				}
				catch (YAML::BadConversion) {}
				++fileIterator;
			}
			if (fileIterator == file.end())
			{
				throw NotFoundInFile(Mark(), fileName, keyName, value, source);
			}
			return fileIterator;
		}

		inline std::vector<YAML::Node>::iterator seekInFile(const std::string& fileName,
			const std::string& keyName, const std::string& value, const std::string& source = "")
		{
			std::vector<YAML::Node> file = LoadAllFromFile(fileName);
			return seekInFile(file, fileName, keyName, value, source);
		}

		template<class T>
		inline void checkElementType(const YAML::Node& node, const std::string& type, const std::string& source = "")
		{
			try { node.as<T>(); }
			catch (std::runtime_error e)
			{
				std::ostringstream stream;
				stream << node;
				throw BadType(node.Mark(), stream.str(), type, source);
			}
		}
	}
}
