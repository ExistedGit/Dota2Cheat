#pragma once
#include <map>
#include <mutex>
#include <iostream>

// Threadsafe technology patented by Wolf49406 and extended by yours truly
template <class K, class V, class Compare = std::less<K>, class Allocator = std::allocator<std::pair<const K, V> > >
class qwemap {
private:
	std::map<K, V, Compare, Allocator> _map;
	std::mutex _m;

public:
	auto begin() {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map.begin();
	}
	auto end() {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map.end();
	}
	void set(const K& key, const V& value) {
		std::lock_guard<std::mutex> lk(this->_m);
		this->_map[key] = value;
	}

	V& operator[](const K& key) {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map[key];
	}

	V& get(const K& key) {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map[key];
	}

	bool empty() {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map.empty();
	}

	void clear() {
		std::lock_guard<std::mutex> lk(this->_m);
		this->_map.clear();
		return;
	}
	bool contains(const K& key) {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map.contains(key);
	}
	bool count(const K& key) {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map.count(key);
	}

	size_t size() {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map.size();
	}

	void erase(const K& key) {
		std::lock_guard<std::mutex> lk(this->_m);
		this->_map.erase(key);
	}
	auto erase(std::map<K, V, Compare, Allocator>::iterator it) {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map.erase(it);

	}

	std::map<K, V> getCopyOfOriginal() {
		std::lock_guard<std::mutex> lk(this->_m);
		return this->_map;
	}
};