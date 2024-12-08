#pragma once
#ifndef __LINKQUEUE_H__
#define __LINKQUEUE_H__

/*
����new��deleteʵ��һ���򵥵Ķ����
����LinkQue��pop��push����Ƶ��������new��deleteƵ��

����ؿ�����ǰ����  OBJECT_POOL_SIZE * sizeof(LinkQue) ��С���ڴ�
�ٽ���ЩLinkQue��*nxt������������Ҫnewһ�������ʱ��ͷ��ص�ǰδʹ�õ�objectPool���ͷŵ�ʱ����ռ���

*/

/*
ʵ��һ��������
LinkQue

����100,000��pop\push
��ʹ�ö����,28ms
ʹ�ö����,6ms
���5���Ż�
*/
template <typename T>
class LinkQue {
public:
	LinkQue()
		:_head(new QueueItem()),
		_rear(_head)
	{}
	void push(const T& val) {
		//ʹ�ö���ع���Ļ����ڴ濪�ٲ���Ҫ����ʵ��
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
		QueueItem(T data = T()) //ʹ��Ĭ�Ϲ��죬�൱����ÿ�����͵�Ĭ��ֵ��ֵ��
			:_data(data),
			_nxt(nullptr)
		{}
		T _data;
		QueueItem* _nxt;
		/*
			�����
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
	QueueItem* _head;	//ָ��ͷ�ڵ�
	QueueItem* _rear;	//ָ���β
};

template <typename T>
typename LinkQue<T>::QueueItem* LinkQue<T>::QueueItem::_objectPool = nullptr;

#endif // !__LINKQUEUE_H__
