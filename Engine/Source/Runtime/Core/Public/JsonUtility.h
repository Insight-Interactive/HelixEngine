// Copyright 2024 Insight Interactive. All Rights Reserved.
/*
	File: JsonUtility.h
	Author: Garrett Courtney

	Description:
	Light wrapper around rapidjson api for accessing properties.
*/
#pragma once

#include "GUID.h"
#include "FileSystem.h"
#include "Transform.h"
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


namespace JsonUtility
{
	typedef rapidjson::PrettyWriter<rapidjson::StringBuffer> WriteContext;
	typedef rapidjson::Value ReadContext;

	/*
		Populates a json document with a pre-opened file. Returns true if succeeded false if not.
		@param File - Preloaded file ontainting the json source.
	*/
	bool LoadDocument( FileRef File, rapidjson::Document& OutDocument );

	/*
		Loads a json document and populates the out document. Its recomented 'LoadDocument( FileRef File, rapidjson::Document& OutDocument )'
		be used instead to reuse file descriptors. Returns true if succeeded false if not.
	*/
	bool LoadDocument( const char* Filename, rapidjson::Document& OutDocument );

	/*
		Writes json data to a specified file. Returns true if succeeded, false if not.
	*/
	bool WriteDocument( const Char* DestinationFile, rapidjson::StringBuffer& Buffer );

	/*
		Returns true if a property exists, false if not.
	*/
	bool PropertyExists( const rapidjson::Value& Value, const char* PropertyName );

	/*
		Reads and returns a integer property from a json value. Returns true if succeeded false if not.
	*/
	bool GetInteger( const rapidjson::Value& Value, const char* PropertyName, int32& OutInteger );

	/*
		Reads and returns a float property from a json value. Returns true if succeeded false if not.
	*/
	bool GetFloat( const rapidjson::Value& Value, const char* PropertyName, float& OutFloat );

	/*
		Reads and returns a double property from a json value. Returns true if succeeded false if not.
	*/
	bool GetDouble( const rapidjson::Value& Value, const char* PropertyName, double& OutDouble );

	/*
		Reads and returns a guid property from a json value. Returns true if succeeded false if not.
	*/
	bool GetGuid( const rapidjson::Value& Value, const char* PropertyName, FGUID& OutGuid );

	/*
		Reads and returns a string property from a json value. It's recomended you use 'GetString( const rapidjson::Value& Value, const char* PropertyName, char* OutStringBuffer, uint32 BufferSize )'
		as it does not allocate heap memory. Returns true if succeeded false if not.
	*/
	bool GetString( const rapidjson::Value& Value, const char* PropertyName, String& OutString );

	/*
		Reads and returns a string property from a json value. Returns true if succeeded false if not.
	*/
	bool GetString( const rapidjson::Value& Value, const char* PropertyName, char* OutStringBuffer, uint32 BufferSize );

	/*
		Reads and returns a boolean property from a json value. Returns true if succeeded false if not.
	*/
	bool GetBoolean( const rapidjson::Value& Value, const char* PropertyName, bool& OutBool );
	
	/*
		Reads and returns a FTransform property from a json value. Returns true if succeeded false if not.
		Json Transform properties must be a laid out as follows to be read correctly:
		<PropertyName>: [
			{
				"PositionX": 0.0,
				"PositionY": 0.0,
				"PositionZ": 0.0,
				"RotationX": 0.0,
				"RotationY": 0.0,
				"RotationZ": 0.0,
				"ScaleX": 0.0,
				"ScaleY": 0.0,
				"ScaleZ": 0.0
			}
		]
	*/
	bool GetTransform( const rapidjson::Value& OwningObject, const char* PropertyName, FTransform& OutTransform );

	/*
		Reads the FTransform components of a json structure without assuming an array.
	*/
	bool GetTransform( const rapidjson::Value& OwningObject, FTransform& OutTransform );

	/*
		Writes a Transform in the proper format to be read with JSonUtility::GetTransform()
	*/
	void WriteTransform( WriteContext& Output, const char* PropertyName, const FTransform& Transform );

	/*
		Writes just the transforms values with no object blocks in Json.
	*/
	void WriteTransformValues( WriteContext& Output, const FTransform& Transform );
}

#include "JsonUtility.inl"
