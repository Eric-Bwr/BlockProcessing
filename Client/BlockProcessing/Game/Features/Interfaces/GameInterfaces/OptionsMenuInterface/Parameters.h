//Author: Briac
#pragma once

#include <string>
#include <sstream>
#include <unordered_map>
#include <memory>

#include "../glm/vec2.hpp"
#include "../glm/vec3.hpp"
#include "../glm/vec4.hpp"

class Parameters {
public:
	Parameters(const std::string& className);
	virtual ~Parameters();

	Parameters extend(const std::string& className){
		return Parameters(this->className + "#" + className);
	}

	std::string& getString(const std::string &name, const std::string& defaultValue = "") const;
	int& getInt(const std::string &name, int defaultValue = 0) const;
	float& getFloat(const std::string &name, float defaultValue = 0) const;
	bool& getBool(const std::string &name, bool defaultValue = 0) const;
	glm::vec2& getVec2(const std::string &name, glm::vec2 defaultValue = glm::vec2(0)) const;
	glm::vec3& getVec3(const std::string &name, glm::vec3 defaultValue = glm::vec3(0)) const;
	glm::vec4& getVec4(const std::string &name, glm::vec4 defaultValue = glm::vec4(0)) const;

	struct Registrable{
		Registrable() = default;
		virtual void init(const std::string& s) = 0;
		virtual std::string toString() const = 0;
		virtual ~Registrable() {
		}

		void read(std::stringstream& ss, bool& v) const{
			ss >> v;
		}
		void read(std::stringstream& ss, int& v) const{
			ss >> v;
		}
		void read(std::stringstream& ss, float& v) const{
			ss >> v;
		}
		void read(std::stringstream& ss, glm::vec3& v) const{
			ss >> v.x >> v.y >> v.z;
		}
		void read(std::stringstream& ss, glm::vec4& v) const{
			ss >> v.x >> v.y >> v.z >> v.w;
		}

		void write(std::stringstream& ss, bool v) const{
			ss <<v <<" ";
		}
		void write(std::stringstream& ss, int v) const{
			ss <<v <<" ";
		}
		void write(std::stringstream& ss, float v) const{
			ss <<v <<" ";
		}
		void write(std::stringstream& ss, glm::vec3 v) const{
			ss <<v.x <<" " <<v.y <<" " <<v.z <<" ";
		}
		void write(std::stringstream& ss, glm::vec4 v) const{
			ss <<v.x <<" " <<v.y <<" " <<v.z <<" " <<v.w <<" ";
		}
	};

	void putRegistrable(const std::string &name, std::shared_ptr<Parameters::Registrable> defaultValue);
	Registrable& get(const std::string & name) const;

private:
	static std::unordered_map<std::string, std::string> stringParams;
	static std::unordered_map<std::string, int> intParams;
	static std::unordered_map<std::string, float> floatParams;
	static std::unordered_map<std::string, bool> boolParams;
	static std::unordered_map<std::string, glm::vec2> vec2Params;
	static std::unordered_map<std::string, glm::vec3> vec3Params;
	static std::unordered_map<std::string, glm::vec4> vec4Params;
	static std::unordered_map<std::string, std::shared_ptr<Parameters::Registrable>> customParams;
	static int instances;

	std::string className;

	static void readParams();
	static void writeParams();
};