// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net_Core.h"
#include "Net_Squeue.h"
#include "Net_Message.h"


namespace net {
	template<typename T>
	class Connection : public std::enable_shared_from_this<Connection<T>> {
	public:
		enum class owner {
			server,
			client
		};

		Connection(owner parent,
			asio::io_context& asioContext,
			asio::ip::tcp::socket socket,
			Squeue<Owned_message<T>>& qIn)
			: m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn) {
			m_nOwnerType = parent;
		}

		virtual ~Connection() {};

		uint32_t getID() const {
			return id;
		}

		void connectToClient(uint32_t uid = 0) {
			if (m_nOwnerType == owner::server) {
				if (m_socket.is_open()) {
					id = uid;
					readHeader();
				}
			}
		}

		void connectToServer(const asio::ip::tcp::resolver::results_type& endpoints) {
			// Only clients can connect to server.
			if (m_nOwnerType == owner::client) {
				// Request asio attempts to connect to an endpoint.
				asio::async_connect(m_socket, endpoints,
					[this](std::error_code ec, asio::ip::tcp::endpoint endpoint) {
						if (!ec) {
							readHeader();
						}

					});
			}
		}

		void disconnect() {
			if (isConnected()) {
				asio::post(m_asioContext, [this]() {m_socket.close(); });
			}
		}

		bool isConnected() const {
			return m_socket.is_open();
		}

		void send(const Message<T>& msg) {
			asio::post(m_asioContext,
				[this, msg]() {
					bool bWriteingMessage = !m_qMessagesOut.empty();
					m_qMessagesOut.push_back(msg);
					if (!bWriteingMessage) {
						writeHeader();
					}
				});
		}

	private:
		// ASYNC - Prime context ready to read a message header.
		void readHeader() {
			asio::async_read(m_socket, asio::buffer(&m_msgTemporaryIn.Header, sizeof(Message_header<T>)),
				[this](std::error_code ec, std::size_t length) {
					if (!ec) {
						if (m_msgTemporaryIn.Header.size > 0) {
							m_msgTemporaryIn.Body.resize(m_msgTemporaryIn.Header.size);
							readBody();
						}
						else {
							addToIncomingMessageQueue();
						}
					}
					else {
						m_socket.close();
					}

				});
		}

		void readBody() {
			asio::async_read(m_socket, asio::buffer(m_msgTemporaryIn.Body.data(),
				m_msgTemporaryIn.Body.size()),
				[this](std::error_code ec, std::size_t length) {
					if (!ec) {
						addToIncomingMessageQueue();
					}
					else {
						m_socket.close();
					}
				});

		}

		void writeHeader() {
			asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().Header, sizeof(Message_header<T>)),
				[this](std::error_code ec, std::size_t length) {
					if (!ec) {
						if (m_qMessagesOut.front().Body.size() > 0) {
							writeBody();
						}
						else {
							m_qMessagesOut.pop_front();

							if (!m_qMessagesOut.empty()) {
								writeHeader();
							}
						}
					}
					else {
						if (m_nOwnerType == owner::server) {
							std::cout << "[" << id << "] read header fail.\n";
							m_socket.close();
						}
					}
				});
		}

		void writeBody() {
			asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().Body.data(), m_qMessagesOut.front().Body.size()),
				[this](std::error_code ec, std::size_t length) {
					if (!ec) {
						m_qMessagesOut.pop_front();

						if (!m_qMessagesOut.empty()) {
							writeHeader();
						}
					}
					else {
						m_socket.close();
					}
				});
		}

		void addToIncomingMessageQueue() {
			if (m_nOwnerType == owner::server) {
				m_qMessagesIn.push_back({ this->shared_from_this(), m_msgTemporaryIn });
			}
			else
				m_qMessagesIn.push_back({ nullptr, m_msgTemporaryIn });
			readHeader();
		}

	protected:
		asio::ip::tcp::socket m_socket;
		asio::io_context& m_asioContext;

		Squeue<Message<T>> m_qMessagesOut;
		Squeue<Owned_message<T>>& m_qMessagesIn;
		Message<T> m_msgTemporaryIn;

		owner m_nOwnerType = owner::server;
		uint32_t id = 0;
	};
}