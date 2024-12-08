#pragma once
#ifndef __LINKQUEUE_H__
#define __LINKQUEUE_H__

/*
重载new和delete实现一个简单的对象池
假设LinkQue的pop和push操作频繁，导致new、delete频繁

对象池可以提前构造  OBJECT_POOL_SIZE * sizeof(LinkQue) 大小的内存
再将这些LinkQue的*nxt连接起来，需要new一个对象的时候就返回当前未使用的objectPool，释放的时候回收即可

*/

/*
实现一个链队列
LinkQue

调用100,000次pop\push
不使用对象池,28ms
使用对象池,6ms
差不多5倍优化
*/
template <typename T>
class LinkQue {
public:
	LinkQue()
		:_head(new QueueItem()),
		_rear(_head)
	{}
	void push(const T& val) {
		//使用对象池管理的话，内存开辟不需要在这实现
		QueueItem* newItem = new QueueItem(val);
		_rear->_nxt = newItem;
		_rear = newItem;
	}
	void pop() {
		if (empty()) return;
		QueueItem* first = _head->_nxt;
		_head->_nxt = first->_nxt;
		if (!first->_nxt)
			_head = _rear;
		delete first;
	}
	T first() const { return _head->_nxt->data; }
	bool empty() const { return _head == _rear; }
private:
	struct QueueItem {
		QueueItem(T data = T()) //使用默认构造，相当于用每个类型的默认值赋值了
			:_data(data),
			_nxt(nullptr)
		{}
		T _data;
		QueueItem* _nxt;
		/*
			对象池
		*/
		static QueueItem* _objectPool;
		static const int OBJECT_POOL_SIZE = 100000;
		void* operator new(size_t size) {
			QueueItem* pool = nullptr;
			if (!_objectPool) {
				_objectPool = (QueueItem*)new char[OBJECT_POOL_SIZE * sizeof(QueueItem)];
				pool = _objectPool;
				for (int i = 0; i < OBJECT_POOL_SIZE - 1; ++i) {
					pool->_nxt = pool + 1;
					++pool;
				}
				pool->_nxt = nullptr;
			}
			pool = _objectPool;
			_objectPool = _objectPool->_nxt;
			return pool;
		}
		void operator delete(void* ptr) {
			QueueItem* p = (QueueItem*)ptr;
			p->_nxt = _objectPool;
			_objectPool = p;
		}
	};
	QueueItem* _head;	//指向头节点
	QueueItem* _rear;	//指向队尾
};

template <typename T>
typename LinkQue<T>::QueueItem* LinkQue<T>::QueueItem::_objectPool = nullptr;

#endif // !__LINKQUEUE_H__
