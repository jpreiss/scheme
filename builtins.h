#pragma once 

#include "types.h"
#include <vector>


template <int N>
void require_exactly(std::vector<Object *> const &args)
{
	if (args.size() != N)
		throw std::exception("Wrong number of arguments");
}

template <int N>
void require_atleast(std::vector<Object *> const &args)
{
	if (args.size() < N)
		throw std::exception("Not enough arguments");
}

struct Lambda : public Procedure
{
	Lambda(Pair *args, Object *expression, Environment *env) :
		Procedure(env), argNames(args), expression(expression)
	{
	}
	
	Pair *argNames;
	Object *expression;

	Object *evaluate(std::vector<Object *> const &args)
	{
		Environment *newEnv = new Environment(env);
		for (size_t i = 0; i < args.size(); ++i)
		{
			Object *val = argNames->list_ref(i);
			if (val->type != &Symbol_Type)
			{
				throw std::exception("Parse error for lamba args");
			}
			Symbol *argName = (Symbol *)val;
			newEnv->set(argName->name, args[i]);
		}
		return eval(expression, newEnv);
	}
};

struct Add : public Procedure
{
	Add(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_atleast<1>(args);
		long sum = 0;
		for (auto it = args.begin(); it != args.end(); ++it)
		{
			Object *arg = *it;
			if (arg->type != &Number_Type)
			{
				throw std::exception("Arguments not addable");
			}
			Number *num = (Number *)arg;
			sum += num->value;
		}
		return new Number(sum);
	}
};

struct Subtract : public Procedure
{
	Subtract(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_atleast<1>(args);
		if (args.size() == 1)
		{
			if (!args[0]->type->is_numeric)
				throw std::exception("Can't negate non-numeric type");
			Number *num = (Number *)args[0];
			return new Number(-(num->value));
		}

		if (!args[0]->type->is_numeric)
			throw std::exception("Can't subtract from non-numeric type");
		Number *num = (Number *)args[0];

		long value = num->value;

		for (size_t i = 1; i < args.size(); ++i)
		{
			Object *arg = args[i];
			if (!arg->type->is_numeric)
			{
				throw std::exception("Can't subtract non-numeric type");
			}
			Number *num = (Number *)arg;
			value -= num->value;
		}

		return new Number(value);
	}
};

struct Multiply : public Procedure
{
	Multiply(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_atleast<1>(args);
		long product = 1;
		for (auto it = args.begin(); it != args.end(); ++it)
		{
			Object *arg = *it;
			if (arg->type != &Number_Type)
			{
				throw std::exception("Arguments not addable");
			}
			Number *num = (Number *)arg;
			product *= num->value;
		}
		return new Number(product);
	}
};

struct Divide : public Procedure
{
	Divide(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		if (!args[0]->type->is_numeric ||
			!args[1]->type->is_numeric)
		{
			throw std::exception("Arguments must be numeric");
		}
		Number *a = (Number *)args[0];
		Number *b = (Number *)args[1];
		if (b->value == 0)
			throw std::exception("Divide by zero");
		return new Number(a->value / b->value);
	}
};

struct Equal : public Procedure
{
	Equal(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_atleast<2>(args);
		Object *arg0 = args[0];
		// for now
		if (!arg0->type->is_numeric)
			throw std::exception("= only for numeric types");
		Number *a = (Number *)arg0;
		long val = a->value;

		for (size_t i = 1; i < args.size(); ++i)
		{
			Object *arg = args[i];
			if (!arg->type->is_numeric)
				throw std::exception("= only for numeric types");
			Number *b = (Number *)arg;
			if (b->value != val)
				return new Boolean(false);
		}
		return new Boolean(true);
	}
};

struct Not : public Procedure
{
	Not(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<1>(args);
		Object *arg = args[0];
		if (arg->type != &Boolean_Type)
			throw std::exception("not is only for boolean");
		Boolean *b = (Boolean *)arg;
		return new Boolean(!(b->value));
	}
};

struct And : public Procedure
{
	And(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<2>(args);
		// for now
		if (args[0]->type != &Boolean_Type ||
			args[1]->type != &Boolean_Type)
		{
			throw std::exception("Arguments must be boolean");
		}
		Boolean *a = (Boolean *)args[0];
		Boolean *b = (Boolean *)args[1];
		return new Boolean(a->value && b->value);
	}
};

struct Or : public Procedure
{
	Or(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<2>(args);
		// for now
		if (args[0]->type != &Boolean_Type ||
			args[1]->type != &Boolean_Type)
		{
			throw std::exception("Arguments must be boolean");
		}
		Boolean *a = (Boolean *)args[0];
		Boolean *b = (Boolean *)args[1];
		return new Boolean(a->value || b->value);
	}
};

struct Greater : public Procedure
{
	Greater(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<2>(args);
		// for now
		if (!args[0]->type->is_numeric ||
			!args[1]->type->is_numeric)
		{
			throw std::exception("Arguments must be numeric");
		}
		Number *a = (Number *)args[0];
		Number *b = (Number *)args[1];
		return new Boolean(a->value > b->value);
	}
};

struct Cons : public Procedure
{
	Cons(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<2>(args);
		return new Pair(args[0], args[1]);
	}
};

struct Car : public Procedure
{
	Car(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<1>(args);
		if (args[0]->type != &Pair_Type)
			throw std::exception("Argument to car must be a pair");
		Pair *p = (Pair *)args[0];
		return p->car;
	}
};

struct Cdr : public Procedure
{
	Cdr(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<1>(args);
		if (args[0]->type != &Pair_Type)
			throw std::exception("Argument to cdr must be a pair");
		Pair *p = (Pair *)args[0];
		return p->cdr;
	}
};

struct IsList : public Procedure
{
	IsList(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<1>(args);
		return new Boolean(args[0]->type == &Pair_Type);
	}
};

struct IsSymbol : public Procedure
{
	IsSymbol(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<1>(args);
		return new Boolean(args[0]->type == &Symbol_Type);
	}
};

struct IsNull : public Procedure
{
	IsNull(Environment *env) : Procedure(env) {}
	Object *evaluate(std::vector<Object *> const &args)
	{
		require_exactly<1>(args);
		return new Boolean(args[0] == nullptr);
	}
};