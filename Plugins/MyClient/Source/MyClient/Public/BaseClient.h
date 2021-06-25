// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Net_Core.h"
#include "Net_Squeue.h"
#include "Net_Message.h"
#include "Net_Connection.h"
#include "UObject/NoExportTypes.h"
#include "BaseClient.generated.h"

/**
 * 
 */
UCLASS()
class MYCLIENT_API UBaseClient : public UObject {
	GENERATED_BODY()

public:
	UBaseClient();
	 ~UBaseClient();
	
	 bool connect(const std::string& host, const uint16_t port);
	 void disconnect();
	 bool isConnected();
	 void send(const net::Message<MessageType>& msg);

	 // login.
	 bool ack();
	 bool login(int32_t uid);
	 bool logout(int32_t uid);

	 // item.
	 bool syncPlayerItem();
	 void passItemInfo(FName itemcode, int32_t count = 1);

	 FString retrieveMessage();

	 net::Squeue<net::Owned_message<MessageType>>& incoming();

	 char* uid = nullptr;

protected:
	asio::io_context m_context;
	std::thread thrContext;
	std::unique_ptr<net::Connection<MessageType>> m_connection;

private:
	net::Squeue<net::Owned_message<MessageType>> m_qMessagesIn;

	
};
