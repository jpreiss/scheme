#pragma once

#include <string>
#include <map>

struct Object;

struct Type
{
	bool is_numeric;
	std::string (*to_string)(Object const *);
};

struct Object
{
	Object(Type *type) : type(type) {}
	Type *type;
};

std::string to_string(Object *obj)
{	
	if (obj == nullptr)
	{
		return "()";
	}
	return obj->type->to_string(obj);
}

//
// Environment
//

struct Environment
{
	Environment(Environment *parent) : parent(parent) {}
	void set(std::string const &s, Object *value)
	{
		items[s] = value;
	}
	Object *lookup(std::string const &s)
	{
		if (items.find(s) != items.end())
			return items[s];
		if (parent == nullptr)
		{
			std::string message = "Unbound variable: " + s;
			throw std::runtime_error(message.c_str());
		}
		return parent->lookup(s);
	}
	Environment *parent;
	std::map<std::string, Object *> items;
};

Object *eval(Object *x, Environment *env);
