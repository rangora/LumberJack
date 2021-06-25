// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <queue>

//#ifdef _WIN32
//#define _WIN32_WINNT 0x0A00
//#endif

#define ASIO_STANDALONE
#include "asio.hpp"
#include <asio/error.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>


enum class MessageType : uint8_t {
	ACK,
	ITEM,
	LOGIN,
	LOGOUT,
};

enum ItemOperation {
	ITEMOUT=0,
	ITEMIN=1,
	ITEMSYNC=3
};

enum LoginOperation {
	DECLINE=0,
	ACCEPT=1,
};

