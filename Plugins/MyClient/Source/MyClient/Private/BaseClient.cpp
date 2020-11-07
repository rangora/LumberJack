// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClient.h"

UBaseClient::UBaseClient() {}

UBaseClient::~UBaseClient() {}

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

bool UBaseClient::login(int32_t uid) {
	net::Message<MessageType> msg;
	msg.Header.id = MessageType::LOGIN;

	const char* c_str = "1000";
	msg.gets(c_str, std::strlen(c_str));
	send(msg);

	return true;
}

bool UBaseClient::syncPlayerItem() {
	net::Message<MessageType> msg;
	msg.Header.id = MessageType::ITEM;

	const char* c_str = "3";
	msg.gets(c_str, std::strlen(c_str));
	send(msg);

	return true;
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

