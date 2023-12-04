#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Course {
	int id;
	std::string name;
	int hours;
	int semester;
	int prerequisite[63];
};

struct Node {
	Course data;
	Course* next;
};

// 字符串分割函数
std::vector<std::string> split(std::string str, char del) {
	std::vector<std::string> ret;
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
			k++;
		}
	}
}

// 课程对象转换
Node paramCourse(std::vector<std::string> line) {
	Node ret;
	int k = 0;
	ret.data.id = std::stoi(line[0]);
	ret.data.name = line[1];
	ret.data.hours = std::stoi(line[2]);
	ret.data.semester = std::stoi(line[3]);
	std::vector<std::string> prevs = split(line[4], ' ');
	if (prevs.size() > 0) {
		for (std::string str : prevs) {
			ret.data.prerequisite[k++] = std::stoi(str);
		}
	}
	return ret;	
}

int main() {
	Node adjList[63];	// 课程邻接表
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
		if (!firstLine) {
			// 读取各个学期需要修读课程数
			firstLine = true;
			std::vector<std::string> numsStr = split(line, '\t');
			for (std::string s : numsStr) {
				termCourses[term++] = std::stoi(s);
			}
		}
		else {
			// 读取各个课程信息
			std::vector<std::string> courseLine = split()
			adjList[course].data = 
		}
	}

	delete file;
	return 0;
}