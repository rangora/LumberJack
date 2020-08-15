// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine.h"
#include "MainGameState.h"


AMainGameState::AMainGameState() {
	static ConstructorHelpers::FObjectFinder<UDataTable> BaseItemDB_Class(
		TEXT("/Game/DataTable/BaseItemTable.BaseItemTable"));

	if (BaseItemDB_Class.Succeeded())
		BaseItemDB = BaseItemDB_Class.Object;
}