#pragma once 

#include "core.h"
#include <sstream>
#include <vector>

//
// Symbol
//

std::string symbol_to_string(Object const *obj);

Type Symbol_Type =
{
	false, // is_numeric
	&symbol_to_string
};

struct Symbol : public Object
{
	Symbol(std::string const &name) : Object(&Symbol_Type), name(name) {}
	std::string name;
};

std::string symbol_to_string(Object const *obj)
{
	Symbol *sym = (Symbol *)obj;
	return sym->name;
}

//
// Pair
//

std::string pair_to_string(Object const *obj);

Type Pair_Type =
{
	false, // is_numeric
	&pair_to_string
};

struct Pair : public Object
{
	Pair(Object *car, Object *cdr) : Object(&Pair_Type), car(car), cdr(cdr) {}
	Object *car;
	Object *cdr;
	Object *list_ref(int i)
	{
		if (i == 0) return car;
		if (cdr == nullptr) throw std::exception("Out of bounds");
		if (cdr->type != &Pair_Type) throw std::exception("Not a list");
		Pair *next = (Pair *)cdr;
		return next->list_ref(i - 1);
	}
};

std::string pair_to_string(Object const *obj)
{
	Pair *pair = (Pair *)obj;
	std::string out;
	// lots of malloc, LOL
	out += '(';
	out += to_string(pair->car);
	out += ' ';
	out += to_string(pair->cdr);
	out += ')';
	return out;
}

//
// Number
//

std::string number_to_string(Object const *obj);

Type Number_Type = 
{
	true, // is_numeric
	&number_to_string
};

struct Number : public Object
{
	Number(long value) : Object(&Number_Type), value(value) {}
	long value;
};

std::string number_to_string(Object const *obj)
{
	Number *n = (Number *)obj;
	std::ostringstream ss;
	ss << n->value;
	return ss.str();
}

//
// Boolean
//

std::string boolean_to_string(Object const *obj);

Type Boolean_Type =
{
	false, // is_numeric
	&boolean_to_string,
};

struct Boolean : public Object
{
	Boolean(bool value) : Object(&Boolean_Type), value(value) {}
	bool value;
};

std::string boolean_to_string(Object const *obj)
{
	Boolean *b = (Boolean *)obj;
	if (b->value)
		return "#t";
	return "#f";
}

//
// Procedure
//

std::string procedure_to_string(Object const *obj);

Type Procedure_Type =
{
	false, // is_numeric
	&procedure_to_string,
};

struct Procedure : public Object
{
	Procedure(Environment *env) : Object(&Procedure_Type), env(env) {}
	Environment *env;
	virtual Object *evaluate(std::vector<Object *> const &args) = 0;
};

std::string procedure_to_string(Object const *obj)
{
	Procedure *proc = (Procedure *)obj;
	return "Procedure";
}