#include <iostream>
#include<vector>
#include <string>

// max���ȶ���
template<typename T>
class IndexMaxPQ {
private:
	int MAX;	// ������ڴ�
	int N;	// �Ѵ�С
	std::vector<int> pq;	// ���������Ķ�
	std::vector<int> qp;	// pq����������, qp[pq[i]] = i
	std::vector<T> keys;	// ������Ӧ��ֵ

public:
	IndexMaxPQ(int MAX = 1024) {
		this->MAX = MAX;
		N = 0;
		pq = std::vector<int>(MAX, -1);
		qp = std::vector<int>(MAX, -1);
		keys = std::vector<T>(MAX);
	}

	// �п�
	bool isEmpty() {
		return N == 0;
	}

	// ���������ж��Ƕ��з����
	bool contains(int i) {
		if (i >= 0 && i < MAX) {
			return qp[i] != -1;
		}
		throw "Invalid index!";
	}

	// ����Ԫ��λ��
	void swap(int a, int b) {
		std::swap(pq[a], pq[b]);
		qp[pq[a]] = a;
		qp[pq[b]] = b;
	}

	// ���ֵ������
	int maxIndex() {
		if (N == 0) {
			throw "Heap is empty!";
		}
		return pq[1];
	}

	// ���ֵ
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

	// �ϸ�
	void upHeap(int k) {
		while (k > 1 && greater(k, k / 2)) {
			swap(k, k / 2);
			k = k / 2;
		}
	}

	// �³�
	void downHeap(int k) {
		while (2 * k < N) {
			int j = 2 * k;
			if (j < N && greater(j + 1, j)) j++;
			if (greater(k, j)) break;
			swap(k, j);
			k = j;
		}
	}

	// ��ȡ����������Ӧ��ֵ
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

	// ���������Ͷ�Ӧ��ֵ
	void insert(int i, T key) {
		N++;
		if (contains(i)) {
			throw "Index already exists!";
		}
		qp[i] = N;
		pq[N] = i;
		keys[i] = key;
		upHeap(N);	// ������
	}

	// ɾ�����Ԫ��
	int delMax() {
		if (N == 0) {
			throw "Heap is empty!";
		}
		int max = pq[1];
		swap(1, N--);
		downHeap(1);	//������
		qp[max] = -1;
		keys[max] = NULL;
		pq[N + 1] = -1;
		return max;
	}

	// ɾ�����ж�Ӧ������Ԫ��
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