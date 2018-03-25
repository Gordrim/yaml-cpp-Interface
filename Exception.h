#pragma once
#include <exception>
#include <string>
#include "yaml-cpp/exceptions.h"

namespace YAML
{
	namespace Interface
	{
		
		//!Base class of Interface's exceptions 
		struct Exception : YAML::Exception
		{
			//! Constructor
			/*!
			\param mark Mark object from yaml-cpp, contain the position of the error in the file.
			\param message the message to display.
			\param source the error's source.
			*/
			Exception(const Mark &mark, const std::string& message, const std::string& source = "") : YAML::Exception( mark, message+((source=="") ? "" : ", "+source)), source(source), interfaceErrorMessage(message){};

			//!Path that represent the error's source in the tree of nodes
			std::string source; 
			std::string interfaceErrorMessage;
		};

		//!Exception thrown if the type of a node is not valide
		struct InvalidNodeType : Exception
		{
			//! Constructor
			/*!
			\param mark Mark object from yaml-cpp, contain the position of the error in the file.
			\param requiredType string that contain the name of the required node's type
			\param foundType string that contain the name of the actual found node's type
			\param source the error's source.
			*/
			InvalidNodeType(const Mark &mark, const std::string requiredType, const std::string foundType, const std::string& source = "") :
				Exception(mark, "Invalid Node Type: require " + requiredType + ", found "+ foundType, source),
				requiredType(requiredType)
			{}

			std::string foundType; //!<string that contain the name of the required node's type
			std::string requiredType; //!<string that contain the name of the actual found node's type
		};

		//!Exception thrown if an required element of the file is missing
		struct MissingElement : Exception
		{
			//! Constructor
			/*!
			\param mark Mark object from yaml-cpp, contain the position of the error in the file.
			\param requiredKey string that contain the name of missing key
			\param requiredType string that contain the name of the missing element's type
			\param source the error's source.
			*/
			MissingElement(const Mark &mark, const std::string& requiredKey, const std::string& requiredType, const std::string& source = "") :
				Exception(mark, "Missing Element: " + requiredKey + " a " + requiredType, source),
				requiredKey(requiredKey), requiredType(requiredType)
			{}

			std::string requiredKey;//!<string that contain the name of missing key
			std::string requiredType;//!<string that contain the name of the missing element's type
		};

		//!Exception thrown if the type of an element is incorrect
		struct BadType : Exception
		{
			//! Constructor
			/*!
			\param mark Mark object from yaml-cpp, contain the position of the error in the file.
			\param value string that contain the value found
			\param requiredType string that contain the type wanted
			\param source the error's source.
			*/
			BadType(const Mark &mark, const std::string value, std::string requiredType, const std::string& source = "") :
				Exception(mark, "Bad Type: " + value + " is not a " + requiredType, source),
				value(value), requiredType(requiredType)
			{}

			std::string value;//!<string that contain the value found
			std::string requiredType;//!<string that contain the type wanted
		};

		//!Exception thrown if the key with the value specified were not found in a specified file
		struct NotFoundInFile : Exception
		{
			//! Constructor
			/*!
			\param mark Mark object from yaml-cpp, contain the position of the error in the file.
			\param fileName string that contain the name/path of the file
			\param keyName string that contain the key sougth
			\param value string that contain the value sougth
			\param source the error's source.
			*/
			NotFoundInFile(const Mark &mark, const std::string& fileName, const std::string& keyName, const std::string& value, const std::string& source = "") :
				Exception(mark, "No " + keyName + " equal to \"" + value + "\" found in " + fileName, source),
				fileName(fileName), keyName(keyName), value(value)
			{}

			std::string fileName;//!<string that contain the name/path of the file
			std::string keyName;//!<string that contain the key sougth
			std::string value;//!<string that contain the value sougth
		};
	}
	
}