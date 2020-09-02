#pragma once

#include <Arduino.h>

template <typename T, size_t BUFFER_SIZE>
class CircularBuffer {
   public:
    CircularBuffer() : _head{0}, _tail{0}, _full{false} {}

    ~CircularBuffer() {}

    void reset() {
        _head = _tail = _full = 0;
    }

    bool empty() const {
        return _head == _tail && !_full;
    }

    bool full() const {
        return _full;
    }

    size_t size() const {
        size_t res = 0;
        if (!_full) {
            if (_head < _tail)
                res = BUFFER_SIZE + _head - _tail;
            else
                res = _head - _tail;
        } else {
            res = BUFFER_SIZE;
        }
        return res;
    }

    void push(const T &item) {
        if (_full) {
            _tail++;
            if (_tail == BUFFER_SIZE)
                _tail = 0;
        }
        _pool[_head++] = item;
        if (_head == BUFFER_SIZE)
            _head = 0;
        if (_head == _tail)
            _full = true;
    }

    bool pop(T &item) {
        bool res = false;
        if (!empty()) {
            item = _pool[_tail++];
            if (_tail == BUFFER_SIZE) _tail = 0;
            _full = false;
            res = true;
        }
        return res;
    }

    bool pop_back(T &item) {
        bool res = false;
        if (!empty()) {
            item = _pool[--_head];
            _full = false;
            res = true;
        }
        return res;
    }

    bool peek(T &item) const {
        bool res = false;
        if (!empty()) {
            item = _pool[_tail];

            res = true;
        }
        return res;
    }

   private:
    T _pool[BUFFER_SIZE];
    size_t _head;
    size_t _tail;
    bool _full;
};