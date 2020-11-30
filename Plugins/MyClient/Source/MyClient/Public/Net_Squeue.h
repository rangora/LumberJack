// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Net_Core.h"


namespace net {
	template<typename T>
	class Squeue {
	public:
		Squeue() = default;
		Squeue(const Squeue<T>&) = delete;
		virtual ~Squeue() { clear(); }

		const T& front() {
			std::lock_guard<std::mutex> lock(muxQueue);
			return deqQueue.front();
		}

		const T& back() {
			std::lock_guard lock(muxQueue);
			return deqQueue.back();
		}

		void push_back(const T& item) {
			std::lock_guard<std::mutex> lock(muxQueue);
			deqQueue.emplace_back(std::move(item));
			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		void push_front(const T& item) {
			std::lock_guard<std::mutex> lock(muxQueue);
			deqQueue.emplace_front(std::move(item));
			std::unique_lock<std::mutex> ul(muxBlocking);
			cvBlocking.notify_one();
		}

		size_t count() {
			std::lock_guard<std::mutex> lock(muxQueue);
			return deqQueue.size();
		}

		void clear() {
			std::lock_guard<std::mutex> lock(muxQueue);
			deqQueue.clear();
		}

		T pop_front() {
			std::lock_guard<std::mutex> lock(muxQueue);
			auto t = std::move(deqQueue.front());
			deqQueue.pop_front();
			return t;
		}

		T pop_back() {
			std::lock_guard<std::mutex> lock(muxQueue);
			auto t = std::move(deqQueue.back());
			deqQueue.pop_back();
			return t;
		}

		bool empty() {
			std::lock_guard<std::mutex> lock(muxQueue);
			return deqQueue.empty();
		}

		void wait() {
			while (empty()) {
				std::unique_lock<std::mutex> ul(muxBlocking);
				cvBlocking.wait(ul);
			}
		}


	protected:
		std::mutex muxQueue;
		std::deque<T> deqQueue;
		std::condition_variable cvBlocking;
		std::mutex muxBlocking;
	};
}