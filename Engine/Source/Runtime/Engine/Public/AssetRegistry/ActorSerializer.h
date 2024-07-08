// Copyright 2024 Insight Interactive. All Rights Reserved.
#pragma once


class AActor;

class FActorSerializer
{
public:
	static void DeserializeActorOverrides( AActor& OutActor, const JsonUtility::ReadContext& InstanceOverrides );
	static void DeserializeActor( AActor& OutActor, const JsonUtility::ReadContext& Value );
	static void SerializeActor( AActor& Actor, JsonUtility::WriteContext& Output );

};

