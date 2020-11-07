// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClient.h"
#include "NetClient.generated.h"

/**
 * 
 */
UCLASS()
class LUMBERJACK_API UNetClient : public UBaseClient {
	GENERATED_BODY()
	
public:
	UNetClient();
	~UNetClient();

	void Test();
};
