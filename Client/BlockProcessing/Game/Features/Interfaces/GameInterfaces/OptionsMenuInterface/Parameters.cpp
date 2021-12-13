//Author: Briac
#include "Parameters.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

#include <mutex>

static std::mutex mutex = std::mutex();
std::unordered_map<std::string, std::string> Parameters::stringParams;
std::unordered_map<std::string, int> Parameters::intParams;
std::unordered_map<std::string, float> Parameters::floatParams;
std::unordered_map<std::string, bool> Parameters::boolParams;
std::unordered_map<std::string, glm::vec2> Parameters::vec2Params;
std::unordered_map<std::string, glm::vec3> Parameters::vec3Params;
std::unordered_map<std::string, glm::vec4> Parameters::vec4Params;
std::unordered_map<std::string, std::shared_ptr<Parameters::Registrable>> Parameters::customParams;
int Parameters::instances = 0;

struct DefaultRegistrable : Parameters::Registrable{
	std::string s;
	DefaultRegistrable(const std::string& s) : s(s){

	}
	virtual ~DefaultRegistrable() {

	}

	virtual void init(const std::string& s) override{

	}
	virtual std::string toString() const override{
		return s;
	}
};

Parameters::Parameters(const std::string &className) :
		className(className) {
	std::lock_guard<std::mutex> lock(mutex);
	if (instances == 0) {
		readParams();
	}

	instances++;
}

Parameters::~Parameters() {
	instances--;

	if (instances == 0) {
		writeParams();
	}
}

//For single-line strings only!
std::string& Parameters::getString(const std::string &name, const std::string& defaultValue) const{
	auto p = stringParams.find(name.find('#') == std::string::npos ? className + "#" + name : name);
	if (p == stringParams.end()) {
		stringParams[className + "#" + name] = defaultValue;
		return stringParams[className + "#" + name];
	} else {
		return p->second;
	}
}

int& Parameters::getInt(const std::string &name, int defaultValue) const{
	auto p = intParams.find(name.find('#') == std::string::npos ? className + "#" + name : name);
	if (p == intParams.end()) {
		intParams[className + "#" + name] = defaultValue;
		return intParams[className + "#" + name];
	} else {
		return p->second;
	}
}

float& Parameters::getFloat(const std::string &name, float defaultValue) const{
	auto p = floatParams.find(name.find('#') == std::string::npos ? className + "#" + name : name);
	if (p == floatParams.end()) {
		floatParams[className + "#" + name] = defaultValue;
		return floatParams[className + "#" + name];
	} else {
		return p->second;
	}
}

bool& Parameters::getBool(const std::string &name, bool defaultValue) const{
	auto p = boolParams.find(name.find('#') == std::string::npos ? className + "#" + name : name);
	if (p == boolParams.end()) {
		boolParams[className + "#" + name] = defaultValue;
		return boolParams[className + "#" + name];
	} else {
		return p->second;
	}
}

glm::vec2& Parameters::getVec2(const std::string &name, glm::vec2 defaultValue) const{
	auto p = vec2Params.find(name.find('#') == std::string::npos ? className + "#" + name : name);
	if (p == vec2Params.end()) {
		vec2Params[className + "#" + name] = defaultValue;
		return vec2Params[className + "#" + name];
	} else {
		return p->second;
	}
}

glm::vec3& Parameters::getVec3(const std::string &name, glm::vec3 defaultValue) const{
	auto p = vec3Params.find(name.find('#') == std::string::npos ? className + "#" + name : name);
	if (p == vec3Params.end()) {
		vec3Params[className + "#" + name] = defaultValue;
		return vec3Params[className + "#" + name];
	} else {
		return p->second;
	}
}

glm::vec4& Parameters::getVec4(const std::string &name, glm::vec4 defaultValue) const{
	auto p = vec4Params.find(name.find('#') == std::string::npos ? className + "#" + name : name);
	if (p == vec4Params.end()) {
		vec4Params[className + "#" + name] = defaultValue;
		return vec4Params[className + "#" + name];
	} else {
		return p->second;
	}
}

void Parameters::putRegistrable(const std::string &name, std::shared_ptr<Parameters::Registrable> defaultValue){
	auto p = customParams.find(className + "#" + name);
	if (p != customParams.end()) {
		DefaultRegistrable* value = dynamic_cast<DefaultRegistrable*>(p->second.get());
		if(value == nullptr){
			std::cout <<"Warning: Attempted to register an already existing param on "
					<<className << "#" << name
					<<", the already existing param will be kept"
					<<std::endl;
			return;
		}
		defaultValue->init(value->s);
	}
	customParams[className + "#" + name] = defaultValue;
}

Parameters::Registrable& Parameters::get(const std::string &name) const{
	auto p = customParams.find(name.find('#') == std::string::npos ? className + "#" + name : name);
	if (p == customParams.end()) {
		customParams[className + "#" + name] = std::make_unique<DefaultRegistrable>("");
		return *customParams[className + "#" + name].get();
	} else {
		return *p->second.get();
	}
}


void Parameters::readParams() {
	std::ifstream f;
	f.open("params.txt");

	if (!f.is_open()) {
		std::cout << "Couldn't open params.txt" << std::endl;
		return;
	}
	std::cout << "Reading params.txt" << std::endl;

	std::string str;
	std::stringstream ss;

	while (std::getline(f, str)) {
		ss = std::stringstream(str);

//		std::cout << str << std::endl;

		std::string name, type;
		ss >> name >> type;

		if(type == "string"){
			std::string s;
			std::getline(ss, s);
			if(s.size() > 1){
				stringParams[name] = s.substr(1, std::string::npos);
			}else{
				stringParams[name] = "";
			}
		}else if (type == "int") {
			int v;
			ss >> v;
			intParams[name] = v;
		} else if (type == "float") {
			float v;
			ss >> v;
			floatParams[name] = v;
		} else if (type == "bool") {
			float v;
			ss >> v;
			boolParams[name] = v;
		} else if (type == "vec2") {
			float x, y;
			ss >> x >> y;
			vec2Params[name] = glm::vec2(x, y);
		} else if (type == "vec2") {
			float x, y, z;
			ss >> x >> y >> z;
			vec3Params[name] = glm::vec3(x, y, z);
		} else if (type == "vec4") {
			float x, y, z, w;
			ss >> x >> y >> z >> w;
			vec4Params[name] = glm::vec4(x, y, z, w);
		} else if (type == "custom") {
			std::string s;
			int n1 = 0, n2 = 0;
			do{
				std::string tmp;
				ss >> tmp;
				s += " " + tmp;

				n1 = (int)std::count(s.begin(), s.end(), '{');
				n2 = (int)std::count(s.begin(), s.end(), '}');

			}while(n1 != n2);
			s = s.substr(2, s.size()-4);
			customParams[name] = std::make_unique<DefaultRegistrable>(s);
		}

	}

}

void Parameters::writeParams() {
	std::ofstream f;
	f.open("params.txt");

	if (!f.is_open()) {
		std::cout << "Couldn't open params.txt" << std::endl;
		return;
	}
	std::cout << "Writting params.txt" << std::endl;

	std::vector<std::string> v;

	for (const auto& [name, value] : stringParams) {
		v.push_back(name + " " + "string" + " " + value);
	}
	for (const auto& [name, value] : intParams) {
		v.push_back(name + " " + "int" + " " + std::to_string(value));
	}
	for (const auto& [name, value] : floatParams) {
		v.push_back(name + " " + "float" + " " + std::to_string(value));
	}
	for (const auto& [name, value] : boolParams) {
		v.push_back(name + " " + "bool" + " " + std::to_string(value));
	}
	for (const auto& [name, value] : vec2Params) {
		v.push_back(name + " " + "vec2" + " " + std::to_string(value.x) + " " + std::to_string(value.y));
	}
	for (const auto& [name, value] : vec3Params) {
		v.push_back(name + " " + "vec3" + " " + std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z));
	}
	for (const auto& [name, value] : vec4Params) {
		v.push_back(name + " " + "vec4" + " " + std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + " " + std::to_string(value.w));
	}
	for (const auto& [name, value] : customParams) {
		v.push_back(name + " custom " + "{" + value->toString() + "}");
	}
	std::sort(v.begin(), v.end());

	for (const std::string &s : v) {
		f << s << std::endl;
	}
}