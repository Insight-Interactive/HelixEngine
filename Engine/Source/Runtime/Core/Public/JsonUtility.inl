
inline bool JsonUtility::LoadDocument( FileRef File, rapidjson::Document& OutDocument )
{
	if (File->IsOpen())
	{
		OutDocument.Parse( (const Char*)File->Data() );
		return OutDocument.IsObject();
	}
	return false;
}

inline bool JsonUtility::LoadDocument( const char* Filename, rapidjson::Document& OutDocument )
{
	HE_ASSERT( Filename != NULL );
	
	FileRef JsonSource( Filename, FUM_Read, CM_Text );
	return LoadDocument( JsonSource, OutDocument );
}

inline bool JsonUtility::WriteDocument( const Char* DestinationFile, rapidjson::StringBuffer& Buffer )
{
	FileRef OutFile( DestinationFile, FUM_Write, CM_Text );
	if (OutFile->IsOpen())
	{
		if (!OutFile->WriteData( (void*)Buffer.GetString(), Buffer.GetSize(), 1 ))
		{
			HE_ASSERT( false );
			return false;
		}
	}
	return true;
}

inline bool PropertyExists( const rapidjson::Value& Value, const char* PropertyName )
{
	auto Iter = Value.FindMember( PropertyName );
	return Iter != Value.MemberEnd();
}

inline bool JsonUtility::GetInteger( const rapidjson::Value& Value, const char* PropertyName, int32& OutInteger )
{
	HE_ASSERT( PropertyName != NULL );

	auto Iter = Value.FindMember( PropertyName );
	if (Iter == Value.MemberEnd())
	{
		return false;
	}

	auto& Property = Iter->value;
	if (Property.IsInt() == false)
	{
		return false;
	}

	OutInteger = Property.GetInt();

	return true;
}

inline bool JsonUtility::GetFloat( const rapidjson::Value& Value, const char* PropertyName, float& OutFloat )
{
	HE_ASSERT( PropertyName != NULL );
	
	auto Iter = Value.FindMember( PropertyName );
	if (Iter == Value.MemberEnd())
	{
		return false;
	}

	auto& Property = Iter->value;
	if (Property.IsDouble() == false)
	{
		return false;
	}

	OutFloat = Property.GetFloat();

	return true;
}

inline bool GetDouble( const rapidjson::Value& Value, const char* PropertyName, double& OutDouble )
{
	HE_ASSERT( PropertyName != NULL );

	auto Iter = Value.FindMember( PropertyName );
	if (Iter == Value.MemberEnd())
	{
		return false;
	}

	auto& Property = Iter->value;

	OutDouble = Property.GetDouble();

	return true;
}

inline bool JsonUtility::GetGuid( const rapidjson::Value& Value, const char* PropertyName, FGUID& OutGuid )
{
	HE_ASSERT( PropertyName != NULL );

	auto Iter = Value.FindMember( PropertyName );
	if (Iter == Value.MemberEnd())
	{
		return false;
	}

	auto& Property = Iter->value;
	if (Property.IsString() == false)
	{
		return false;
	}

	FGUID::CreateFromString( Property.GetString(), OutGuid );

	return true;
}

inline bool JsonUtility::GetString( const rapidjson::Value& Value, const char* PropertyName, String& OutString )
{
	HE_ASSERT( PropertyName != NULL );

	auto Iter = Value.FindMember( PropertyName );
	if (Iter == Value.MemberEnd())
	{
		return false;
	}

	auto& Property = Iter->value;
	if (Property.IsString() == false)
	{
		return false;
	}

	OutString = Property.GetString();

	return true;
}

inline bool JsonUtility::GetString( const rapidjson::Value& Value, const char* PropertyName, char* OutStringBuffer, uint32 BufferSize )
{
	HE_ASSERT( PropertyName != NULL );

	auto Iter = Value.FindMember( PropertyName );
	if (Iter == Value.MemberEnd())
	{
		return false;
	}

	auto& Property = Iter->value;
	if (Property.IsString() == false)
	{
		return false;
	}
	
	ZeroMemory( OutStringBuffer, BufferSize );
	CopyMemory( OutStringBuffer, Property.GetString(), BufferSize * sizeof(char) );

	return true;
}

inline bool JsonUtility::GetBoolean( const rapidjson::Value& Value, const char* PropertyName, bool& OutBoolean )
{
	HE_ASSERT( PropertyName != NULL );

	auto Iter = Value.FindMember( PropertyName );
	if (Iter == Value.MemberEnd())
	{
		return false;
	}

	auto& Property = Iter->value;
	if (Property.IsBool() == false)
	{
		return false;
	}

	OutBoolean = Property.GetBool();

	return true;
}

inline bool JsonUtility::GetTransform( const rapidjson::Value& OwningObject, const char* PropertyName, FTransform& OutTransform )
{
	auto Iter = OwningObject.FindMember( PropertyName );
	if (Iter == OwningObject.MemberEnd())
		return false;

	const rapidjson::Value& TransformJson = OwningObject[PropertyName][0];

	return GetTransform(TransformJson, OutTransform);
}

inline bool JsonUtility::GetTransform( const rapidjson::Value& TransformJson, FTransform& OutTransform )
{
	FVector3 Position, Scale;
	FAngles EulerRotation;
	JsonUtility::GetFloat( TransformJson, "PositionX", Position.x );
	JsonUtility::GetFloat( TransformJson, "PositionY", Position.y );
	JsonUtility::GetFloat( TransformJson, "PositionZ", Position.z );

	JsonUtility::GetFloat( TransformJson, "RotationX", EulerRotation.pitch );
	JsonUtility::GetFloat( TransformJson, "RotationY", EulerRotation.yaw );
	JsonUtility::GetFloat( TransformJson, "RotationZ", EulerRotation.roll );

	JsonUtility::GetFloat( TransformJson, "ScaleX", Scale.x );
	JsonUtility::GetFloat( TransformJson, "ScaleY", Scale.y );
	JsonUtility::GetFloat( TransformJson, "ScaleZ", Scale.z );

	OutTransform.SetPosition( Position );
	OutTransform.SetRotation( EulerRotation );
	OutTransform.SetScale( Scale );

	return true;
}

inline void JsonUtility::WriteTransform( JsonUtility::WriteContext& Output, const char* PropertyName, const FTransform& Transform )
{
	Output.Key( PropertyName );
	Output.StartArray();
	{
		Output.StartObject();

		WriteTransformValues( Output, Transform );

		Output.EndObject();
	}
	Output.EndArray();
}

inline void JsonUtility::WriteTransformValues( WriteContext& Output, const FTransform& Transform )
{
	FVector3 Pos = Transform.GetPosition();
	FAngles Rot = Transform.GetEulerRotation();
	FVector3 Sca = Transform.GetScale();

	Output.Key( "PositionX" );
	Output.Double( Pos.x );
	Output.Key( "PositionY" );
	Output.Double( Pos.y );
	Output.Key( "PositionZ" );
	Output.Double( Pos.z );

	Output.Key( "RotationX" );
	Output.Double( Rot.pitch );
	Output.Key( "RotationY" );
	Output.Double( Rot.yaw );
	Output.Key( "RotationZ" );
	Output.Double( Rot.roll );

	Output.Key( "ScaleX" );
	Output.Double( Sca.x );
	Output.Key( "ScaleY" );
	Output.Double( Sca.y );
	Output.Key( "ScaleZ" );
	Output.Double( Sca.z );
}
