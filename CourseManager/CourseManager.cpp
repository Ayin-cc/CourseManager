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