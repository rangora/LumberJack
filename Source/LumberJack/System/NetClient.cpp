// Fill out your copyright notice in the Description page of Project Settings.


#include "NetClient.h"


UNetClient::UNetClient() {}
UNetClient::~UNetClient() {}

void UNetClient::Test() {
	net::Message<MessageType> msg;
	msg.Header.id = MessageType::ACK;

	uint8_t num = 10;
	msg << num;
	send(msg);
}
