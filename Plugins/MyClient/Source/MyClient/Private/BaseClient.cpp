// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClient.h"

UBaseClient::UBaseClient() {}

UBaseClient::~UBaseClient() {
	std::free(uid);
}

bool UBaseClient::connect(const std::string& host, const uint16_t port) {
	asio::ip::tcp::resolver resolver(m_context);
	asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

	m_connection = std::make_unique<net::Connection<MessageType>>(
		net::Connection<MessageType>::owner::client,
		m_context,
		asio::ip::tcp::socket(m_context), m_qMessagesIn);

	m_connection->connectToServer(endpoints);

	thrContext = std::thread([this]() { m_context.run(); });
	return true;
}

void UBaseClient::disconnect() {
	if (isConnected()) {
		m_connection->disconnect();
	}

	m_context.stop();
	if (thrContext.joinable())
		thrContext.join();
	m_connection.release();
}

bool UBaseClient::isConnected() {
	if (m_connection)
		return m_connection->isConnected();
	return false;
}

void UBaseClient::send(const net::Message<MessageType>& msg) {
	if (isConnected())
		m_connection->send(msg);
}

bool UBaseClient::ack() {
	if (isConnected()) {
		return true;
	}
	return false;
}

bool UBaseClient::login(int32_t i_uid) {
	net::Message<MessageType> msg;
	msg.Header.id = MessageType::LOGIN;
	
	FString c_uid = FString::FromInt(i_uid);

	while (c_uid.Len() != 4) c_uid = "0" + c_uid;
	
	if (uid != nullptr) std::free(uid);
	uid = (char*)std::malloc(5);
	
	char* t_uid = (ANSICHAR*)StringCast<ANSICHAR>(*c_uid).Get();
	std::memcpy(uid, t_uid, std::strlen(t_uid));
	uid[4] = '\0';

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
	//	FString::Printf(TEXT("c_uid size: %d"), c_uid.Len()));

	msg.gets(uid, std::strlen(uid));
	send(msg);



	return true;
}

bool UBaseClient::syncPlayerItem() {
	if (uid == nullptr) return false;

	net::Message<MessageType> msg;
	msg.Header.id = MessageType::ITEM;

	//const char* c_str = "10003";
	char c_data[6]{};
	std::memcpy(c_data, uid, std::strlen(uid));
	c_data[4] = '3';
	c_data[5] = '\0';

	msg.gets(c_data, std::strlen(c_data));
	send(msg);

	return true;
}

void UBaseClient::passItemInfo(FName itemcode, int32_t count) {
	net::Message<MessageType> msg;
	msg.Header.id = MessageType::ITEM;

	char c_str[30]{};
	std::memcpy(c_str, uid, std::strlen(uid));

	FString s_code = itemcode.ToString();
	FString s_count = FString::FromInt(count);
	while (s_code.Len() != 4) s_code = "0" + s_code;
	while (s_count.Len() != 4) s_count = "0" + s_count;
	char* ansi_code = (ANSICHAR*)StringCast<ANSICHAR>(*s_code).Get();
	char* ansi_count = (ANSICHAR*)StringCast<ANSICHAR>(*s_count).Get();

	c_str[4] = '1';
	c_str[13] = '\0';
	std::memcpy(c_str, uid, 4);
	std::memcpy(c_str + 5, ansi_code, std::strlen(ansi_code));
	std::memcpy(c_str + 9, ansi_count, std::strlen(ansi_count));

	msg.gets(c_str, std::strlen(c_str));
	send(msg);
}

FString UBaseClient::retrieveMessage() {
	FString m_data;

	if (!incoming().empty()) {
		auto m_msg = incoming().pop_front().msg;
		while (!m_msg.Body.empty()) {
			uint8_t v = m_msg.Body.back();
			m_data.AppendChar(char(v));
			m_msg.Body.pop_back();
		}
	}

	return m_data.Reverse();
}

net::Squeue<net::Owned_message<MessageType>>& UBaseClient::incoming() {
	return m_qMessagesIn;
}

