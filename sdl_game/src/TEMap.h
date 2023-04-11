#include <any>
#include <unordered_map>
#include <exception>

//Value type erased map with key type string
class TEMap {
public:
	template<typename T>
	T get(const std::string& key) const {
		if (auto search = data.find(key); search != data.end()) {
			return std::any_cast<T>(search->second);
		}
		throw std::runtime_error("There is not key " + key + " in TEMap.");
	}
	
	template<typename T>
	void insert(const std::string& key, T value) {
		std::any teValue = value;
		data.insert({key, teValue});
	}
	
	bool contains(const std::string& key) const {
		return data.find(key) != data.end();
	}

private:
	std::unordered_map<std::string, std::any> data;	
};
