#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

	int nn = 0;
	for (Node* node : adjList) {
		std::cout << ++nn << "->";
		for (int i = 0; i < node->data.prerequisite.size(); i++) {
			std::cout << node->data.prerequisite[i] << ' ';
		}
		std::cout << std::endl;
	}

	delete file;
	return 0;
}