#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include "IndexMaxPQ.cpp"

struct Course {
	int id;
	std::string name;
	int hours;
	int semester;
	std::vector<int> prerequisite;
};

struct Node {
	Course data;
	Node* prev;
};

// �ַ����ָ��
std::vector<std::string> split(std::string str, char del) {
	std::vector<std::string> ret(63);
	int k = 0;
	bool isDel = false;
	for (char c : str) {
		if (c != del) {
			if ((k == 0 || k == 4) && c == 'c') {
				// ȥ���γ̺��е�c
				continue;
			}
			ret[k].push_back(c);
			isDel = false;
		}
		if (!isDel && c == del) {
			// ƥ�䵽�ָ����Ҳ���������
			ret[k].push_back('\0');
			k++;
			isDel = true;
		}
	}
	ret.resize(k + 1);
	return ret;
}

// �γ̶���ת��
Course paramCourse(std::vector<std::string> line) {
	Course ret;
	ret.id = std::stoi(line[0]);
	ret.name = line[1];
	ret.hours = std::stoi(line[2]);
	ret.semester = std::stoi(line[3]);
	std::vector<std::string> prevs;
	if (line.size() == 5) {
		prevs = std::vector<std::string>(split(line[4], ' '));
	}
	
	if (prevs.size() > 0) {
		for (std::string str : prevs) {
			if (str.empty()) {
				continue;
			}
			ret.prerequisite.push_back(std::stoi(str));
		}
	}
	return ret;	
}

int main() {
	std::vector<Course> courses;
	std::vector<Node*> adjList(63);	// �γ��ڽӱ�
	int termCourses[8];	// ÿ��ѧ����Ҫ�γ���
	std::ifstream *file = new std::ifstream("./course_inf.txt");

	if (!file->is_open()) {
		std::cout << "�ļ���ʧ�ܣ�" << std::endl;
		exit(1);
	}

	std::string line;
	bool firstLine = false;
	int term = 0;
	int course = 0;
	while (std::getline(*file, line)) {
		if (line.empty() || line[0] == '/' && line[1] == '/') {
			continue;
		}
		std::vector<std::string> sstr = split(line, '\t');
		if (!firstLine) {
			// ��ȡ����ѧ����Ҫ�޶��γ���
			firstLine = true;
			for (std::string s : sstr) {
				termCourses[term++] = std::stoi(s);
			}
		}
		else {
			// ��ȡ�����γ���Ϣ
			Course tc = paramCourse(sstr);
			courses.push_back(tc);
			adjList[course] = new Node;
			adjList[course]->data = Course(tc);
			adjList[course++]->prev = nullptr;
		}
	}
	adjList.resize(course);

	// �����ڽӱ�
	for (Node* node : adjList) {
		Node* head = node;
		for (int i = 0; i < head->data.prerequisite.size(); i++) {
			node->prev = new Node;
			node->prev->data = Course(courses[head->data.prerequisite[i] - 1]);
			node->prev->prev = nullptr;
			node = node->prev;
		}
	}

	// ��ѧ�ڽ�����������
	std::vector<int> inDegree(adjList.size(), -1);
	std::vector<int> outDegree(adjList.size(), 0);
	for (Node* node : adjList) {
		inDegree[node->data.id - 1] = node->data.prerequisite.size();
		for (int i = 0; i < node->data.prerequisite.size(); i++) {
			outDegree[node->data.prerequisite[i] - 1]++;
		}
	}
	for (int sem = 1; sem <= 8; sem++) {
		std::vector<Course> res;

		// ͳ����Ȳ��� -1 �ų��Ǳ�ѧ�ڵ��Լ�ѡ����
		std::vector<int> thisInDegree(adjList.size(), -1);
		for (int i = 0; i < adjList.size(); i++) {
			if ((adjList[i]->data.semester == sem || adjList[i]->data.semester == 0)) {
				thisInDegree[i] = inDegree[i];
			}
		}

		// �����Ϊ0�Ľڵ�������
		std::queue<Course> q;
		for (int i = 0; i < adjList.size(); i++) {
			if (thisInDegree[i] == 0) {
				if (adjList[i]->data.semester == sem) {
					// ����Ǳ�ѧ�ڵ�ֱ��ѡ��
					res.push_back(adjList[i]->data);
					continue;
				}
				q.push(adjList[i]->data);

			}
		}
		// ���ճ��Ƚ��ж�����
		IndexMaxPQ<int> heap(adjList.size() + 1);
		int size = q.size();
		for (int i = 0; i < size; i++) {
			int index = q.front().id;
			heap.insert(index - 1, outDegree[index - 1]);
			q.pop();
		}

		// ����ɾȥ���Ϊ0�Ľڵ�	
		while (!heap.isEmpty()) {
			// �ﵽѧ��Ҫ��γ���������
			if (res.size() == termCourses[sem - 1]) {
				break;
			}

			Course c = courses[heap.maxIndex()];
			heap.delMax();
			res.push_back(c);
			thisInDegree[c.id - 1] = -1;
			inDegree[c.id - 1] = -1;

			// ���ýڵ�ָ��Ľڵ����-1
			for (Node* node : adjList) {
				Node* head = node;
				Node* prev = node->prev;
				while (prev != nullptr) {
					if (prev->data.id == c.id) {
						thisInDegree[node->data.id - 1]--;
						inDegree[node->data.id - 1]--;
						head->prev = prev->prev;
						if (thisInDegree[node->data.id - 1] == 0) {
							heap.insert(node->data.id - 1, outDegree[node->data.id - 1]);
						}
						break;
					}
					head = head->prev;
					prev = prev->prev;
				}
			}
		}

		// ��ӡ��ѧ�ڿγ�
		std::cout << "ѧ��" << sem << ":" << std::endl;
		for (Course c : res) {
			std::cout << "  |" << "\n  +--" << c.name << std::endl;
		}
		std::cout << std::endl;
	}

	delete file;
	return 0;
}