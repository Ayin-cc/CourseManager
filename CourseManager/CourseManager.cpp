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

// 字符串分割函数
std::vector<std::string> split(std::string str, char del) {
	std::vector<std::string> ret(63);
	int k = 0;
	bool isDel = false;
	for (char c : str) {
		if (c != del) {
			if ((k == 0 || k == 4) && c == 'c') {
				// 去除课程号中的c
				continue;
			}
			ret[k].push_back(c);
			isDel = false;
		}
		if (!isDel && c == del) {
			// 匹配到分隔符且不是连续的
			ret[k].push_back('\0');
			k++;
			isDel = true;
		}
	}
	ret.resize(k + 1);
	return ret;
}

// 课程对象转换
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
	std::vector<Node*> adjList(63);	// 课程邻接表
	int termCourses[8];	// 每个学期需要课程数
	std::ifstream *file = new std::ifstream("./course_inf.txt");

	if (!file->is_open()) {
		std::cout << "文件打开失败！" << std::endl;
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
			// 读取各个学期需要修读课程数
			firstLine = true;
			for (std::string s : sstr) {
				termCourses[term++] = std::stoi(s);
			}
		}
		else {
			// 读取各个课程信息
			Course tc = paramCourse(sstr);
			courses.push_back(tc);
			adjList[course] = new Node;
			adjList[course]->data = Course(tc);
			adjList[course++]->prev = nullptr;
		}
	}
	adjList.resize(course);

	// 建立邻接表
	for (Node* node : adjList) {
		Node* head = node;
		for (int i = 0; i < head->data.prerequisite.size(); i++) {
			node->prev = new Node;
			node->prev->data = Course(courses[head->data.prerequisite[i] - 1]);
			node->prev->prev = nullptr;
			node = node->prev;
		}
	}

	// 按学期进行拓扑排序
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

		// 统计入度并用 -1 排除非本学期的以及选过的
		std::vector<int> thisInDegree(adjList.size(), -1);
		for (int i = 0; i < adjList.size(); i++) {
			if ((adjList[i]->data.semester == sem || adjList[i]->data.semester == 0)) {
				thisInDegree[i] = inDegree[i];
			}
		}

		// 将入度为0的节点加入队列
		std::queue<Course> q;
		for (int i = 0; i < adjList.size(); i++) {
			if (thisInDegree[i] == 0) {
				if (adjList[i]->data.semester == sem) {
					// 如果是本学期的直接选上
					res.push_back(adjList[i]->data);
					continue;
				}
				q.push(adjList[i]->data);

			}
		}
		// 按照出度进行堆排序
		IndexMaxPQ<int> heap(adjList.size() + 1);
		int size = q.size();
		for (int i = 0; i < size; i++) {
			int index = q.front().id;
			heap.insert(index - 1, outDegree[index - 1]);
			q.pop();
		}

		// 依次删去入度为0的节点	
		while (!heap.isEmpty()) {
			// 达到学期要求课程数就跳出
			if (res.size() == termCourses[sem - 1]) {
				break;
			}

			Course c = courses[heap.maxIndex()];
			heap.delMax();
			res.push_back(c);
			thisInDegree[c.id - 1] = -1;
			inDegree[c.id - 1] = -1;

			// 将该节点指向的节点入度-1
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

		// 打印该学期课程
		std::cout << "学期" << sem << ":" << std::endl;
		for (Course c : res) {
			std::cout << "  |" << "\n  +--" << c.name << std::endl;
		}
		std::cout << std::endl;
	}

	delete file;
	return 0;
}