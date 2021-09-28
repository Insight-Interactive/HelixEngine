/*
	File: JsonUtility.h
	Author: Garrett Courtney

	Description:
	Light wrapper around rapidjson api for accessing properties.
*/
#pragma once

#include "FileSystem.h"
#include "Transform.h"
#include <rapidjson/document.h>


namespace JsonUtility
{
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
		Reads and returns a integer property from a json value. Returns true if succeeded false if not.
	*/
	bool GetInteger( const rapidjson::Value& Value, const char* PropertyName, int32& OutInteger );

	/*
		Reads and returns a float property from a json value. Returns true if succeeded false if not.
	*/
	bool GetFloat( const rapidjson::Value& Value, const char* PropertyName, float& OutFloat );

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
		Reads and returns a transform property from a json value. Returns true if succeeded false if not.
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
}

#include "JsonUtility.inl"
