//
// Created by ThomasBI on 06/03/2018.
//

#ifndef DREAMINENGINE_COMPONENTSTORE_H
#define DREAMINENGINE_COMPONENTSTORE_H

#include <vector>
#include <unordered_map>

template <class T>
class ComponentStore {

private:
	std::vector<T*> vector;
	std::unordered_map<unsigned int, unsigned int> map;

public:

	std::vector<T*>& getVector() { return vector; }
	std::unordered_map<unsigned int, unsigned int>& getMap() { return map; };

	void add(T* obj, unsigned int id) {
		vector.emplace_back(obj);
		map[id] = vector.size() - 1;
	}

	bool remove(unsigned int indice) {
		//Remove the component from the vector
		int i = map[indice];
		//vector.at(i) = vector[vector.size() - 1];
		vector[i] = vector[vector.size() - 1];
		vector.pop_back();

		//Remove the component from the map
		std::unordered_map<unsigned int, unsigned int>::iterator it2;
		for (std::unordered_map<unsigned int, unsigned int>::iterator it = map.begin(); it != map.end(); ++it) {
			int ii = it->second;
			if (ii == i) {
				it2 = it;
				//map.erase(it);
				//return true;
			}
			else if (ii > i) {
				it->second -= 1;
			}
		}
		map.erase(it2);

		return false;
	}

	T*& get(unsigned int indice) {
		return vector[map[indice]];
	};

	void kill() {
		for (auto it = vector.begin(); it != vector.end(); ++it)
		{
			delete (*it);
		}
		vector.clear();
	}

	~ComponentStore() {
		/*for (auto it = vector.begin() ; it != vector.end(); ++it)
		{
			delete (*it);
		}
		vector.clear();*/
	}
};

#endif //DREAMINENGINE_COMPONENTSTORE_H
