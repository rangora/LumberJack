// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "Engine.h"
#include "NetClient.h"


AMainGameState::AMainGameState() 
	:uid(1000) {
	static ConstructorHelpers::FObjectFinder<UDataTable> BaseItemDB_Class(
		TEXT("/Game/DataTable/BaseItemTable.BaseItemTable"));

	if (BaseItemDB_Class.Succeeded())
		BaseItemDB = BaseItemDB_Class.Object;
}

void AMainGameState::PostInitializeComponents() {
	Super::PostInitializeComponents();

	Client = NewObject<UNetClient>();
	if (Client != nullptr) {
		Client->connect("127.0.0.1", 999);
		Client->login(uid);
		FPlatformProcess::Sleep(1.f);

		FString m_data = Client->retrieveMessage();
		
		if (!m_data.IsEmpty()) {
			if (m_data[0] - '0' == LoginOperation::ACCEPT) {
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(
					TEXT("Login Accepted!!")));
			}
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(
			TEXT("m_data: %s"), *m_data));

		/*Client->ack();
		FPlatformProcess::Sleep(1.f);
		FString m_data = Client->retrieveMessage();


		if (!m_data.IsEmpty()) {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(
				TEXT("m_data: %s"), *m_data));
		}*/
	}
}
