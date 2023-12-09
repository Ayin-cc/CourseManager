#include <iostream>
#include<vector>
#include <string>

// max优先队列
template<typename T>
class IndexMaxPQ {
private:
	int MAX;	// 堆最大内存
	int N;	// 堆大小
	std::vector<int> pq;	// 保存索引的堆
	std::vector<int> qp;	// pq的逆序数组, qp[pq[i]] = i
	std::vector<T> keys;	// 索引对应的值

public:
	IndexMaxPQ(int MAX = 1024) {
		this->MAX = MAX;
		N = 0;
		pq = std::vector<int>(MAX, -1);
		qp = std::vector<int>(MAX, -1);
		keys = std::vector<T>(MAX);
	}

	// 判空
	bool isEmpty() {
		return N == 0;
	}

	// 根据索引判断是堆中否包含
	bool contains(int i) {
		if (i >= 0 && i < MAX) {
			return qp[i] != -1;
		}
		throw "Invalid index!";
	}

	// 交换元素位置
	void swap(int a, int b) {
		std::swap(pq[a], pq[b]);
		qp[pq[a]] = a;
		qp[pq[b]] = b;
	}

	// 最大值的索引
	int maxIndex() {
		if (N == 0) {
			throw "Heap is empty!";
		}
		return pq[1];
	}

	// 最大值
	int maxKey() {
		if (N == 0) {
			throw "Heap is empty!";
		}
		return keys[pq[1]];
	}

	bool greater(int a, int b) {
		std::greater<T> comp;
		return comp(keys[pq[a]], keys[pq[b]]);
	}

	// 上浮
	void upHeap(int k) {
		while (k > 1 && greater(k, k / 2)) {
			swap(k, k / 2);
			k = k / 2;
		}
	}

	// 下沉
	void downHeap(int k) {
		while (2 * k < N) {
			int j = 2 * k;
			if (j < N && greater(j + 1, j)) j++;
			if (greater(k, j)) break;
			swap(k, j);
			k = j;
		}
	}

	// 获取堆中索引对应的值
	int keyOf(int i) {
		if (!contains(i)) {
			throw "Index does not exist!";
		}
		return keys[i];
	}

	void show() {
		for (int i = 1; i <= N; i++) {
			std::cout << keys[pq[i]] << ' ';
		}
		std::cout << std::endl;
	}

	// 插入索引和对应的值
	void insert(int i, T key) {
		N++;
		if (contains(i)) {
			throw "Index already exists!";
		}
		qp[i] = N;
		pq[N] = i;
		keys[i] = key;
		upHeap(N);	// 堆有序化
	}

	// 删除最大元素
	int delMax() {
		if (N == 0) {
			throw "Heap is empty!";
		}
		int max = pq[1];
		swap(1, N--);
		downHeap(1);	//堆有序化
		qp[max] = -1;
		keys[max] = NULL;
		pq[N + 1] = -1;
		return max;
	}

	// 删除堆中对应索引的元素
	void del(int i) {
		if (!contains(i)) {
			throw "Index does not exist!";
		}
		int prio = qp[i];
		swap(prio, N--);
		downHeap(prio);
		keys[i] = NULL;
		qp[i] = -1;
	}
};