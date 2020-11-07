// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#pragma warning(disable:4459)

#include "Net_Core.h"


namespace net {
	template <typename T>
	struct Message_header {
		T id{};
		uint32_t size = 0;
	};

	template <typename T>
	struct Message {
		Message_header<T> Header;
		std::vector<uint8_t> Body;

		size_t size() const {
			return Body.size();
		}

		//friend std::ostream& operator<<(ostream& os, const Message<T>& msg) {
		//	os << "ID: " << int(msg.Header.id) << "Size: " << msg.Header.size;
		//	return os;
		//}

		template <typename DataType>
		friend Message<T>& operator<<(Message<T>& msg, const DataType& data) {
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complecated.");

			size_t i = msg.Body.size();
			msg.Body.resize(msg.Body.size() + sizeof(DataType));
			// memcpy(dest,src,num of byte).
			std::memcpy(msg.Body.data() + i, &data, sizeof(DataType));
			msg.Header.size = msg.size();

			return msg;
		}

		template<typename DataType>
		friend Message<T>& operator>>(Message<T>& msg, DataType& data) {
			static_assert(std::is_standard_layout<DataType>::value, "Data is too complecated.");

			size_t i = msg.Body.size() - sizeof(DataType);
			
			std::memcpy(&data, msg.Body.data() + i, sizeof(DataType));

			msg.Body.resize(i);
			msg.Header.size = msg.size();

			return msg;
		}

		void gets(const char* str, int32_t len) {
			size_t i = Body.size();

			Body.resize(Body.size() + len);
			std::memcpy(Body.data() + i, str, len);
			Header.size = Body.size(); 
		}
	};

	template<typename T>
	class Connection;

	template<typename T>
	struct Owned_message {
		std::shared_ptr<Connection<T>> Remote = nullptr;
		Message<T> msg;

		//friend std::ostream& operator<<(std::ostream& os, const Owned_message& msg) {
		//	os << msg.msg;
		//	return os;
		//}
	};
}
