#pragma once

#include <vector>

class UniqueIdGenerator
{
public:
	UniqueIdGenerator()
		: current_max_id(0)
	{
	}

	unsigned int get_id()
	{
		if (!free_ids.empty())
		{
			unsigned int id = free_ids.back();
			free_ids.pop_back();
			return id;
		} 
		else
		{
			return current_max_id++;
		}
	}

	void release_id(unsigned int id)
	{
		free_ids.push_back(id);
	}

private:
	unsigned int current_max_id;
	std::vector<unsigned int> free_ids;
};
