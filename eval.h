#pragma once 

#include "core.h"
#include "types.h"
#include "builtins.h"

Object *eval(Object *x, Environment *env)
{
	if (x == nullptr)
		return nullptr;

	if (x->type == &Symbol_Type)
	{
		Symbol *sym = (Symbol *)x;
		return env->lookup(sym->name);
	}
	if (x->type != &Pair_Type)
	{
		// skraight up - right now just number, but maybe bool, etc later
		return x;
	}
	Pair *cell = (Pair *)x;

	Object *first = cell->car;

	if (first->type == &Pair_Type)
	{
		// ????
		throw std::exception("I don't understand what to do here yet");
	}
	if (first->type == &Number_Type)
	{
		throw std::exception("Number isn't callable");
	}
	if (first->type == &Symbol_Type)
	{
		Symbol *sym = (Symbol *)first;
		std::string const &name = sym->name;

		if (name == "quote")
		{
			// don't try to evaluate the next element of the list.
			// at this point it might be a symbol, number, or list,
			// but it's not a procedure.
			return cell->list_ref(1);
		}	
		if (name == "if")
		{
			Object *test = cell->list_ref(1);
			Object *yes = cell->list_ref(2);
			Object *no = cell->list_ref(3);
		
			Object *test_eval = eval(test, env);
			bool is_true = true;
			if (test_eval->type == &Boolean_Type)
			{
				Boolean *b = (Boolean *)test_eval;
				is_true = b->value;
			}
			if (is_true)
			{
				return eval(yes, env);
			}
			return eval(no, env);
		}		
		if (name == "set!")
		{
			Object *name = cell->list_ref(1);
			if (name->type != &Symbol_Type)
				throw std::exception("Invalid set!");
			Symbol *setee = (Symbol *)name;
			Object *value = cell->list_ref(2);
			env->set(setee->name, value);
			return nullptr;
		}	
		if (name == "define")
		{
			Object *name = cell->list_ref(1);
			if (name->type != &Symbol_Type)
				throw std::exception("Invalid set!");
			Symbol *assignee = (Symbol *)name;
			Object *value = eval(cell->list_ref(2), env);
			env->set(assignee->name, value);
			return nullptr;
		}		
		if (name == "lambda")
		{
			Object *argNames = cell->list_ref(1);
			if (argNames->type != &Pair_Type)
				throw std::exception("Bad lambda");
			Object *expression = cell->list_ref(2);
			return new Lambda((Pair *)argNames, expression, env);
		}
		if (name == "begin")
		{
			Object *val;
			while (cell->cdr != nullptr)
			{
				val = eval(cell->list_ref(1), env);
				cell = (Pair *)cell->cdr;
			}
			return val;
		}

		// not a special form
		std::vector<Object *> args;

		// ugly... should recurse
		Pair *recurser = cell;
		Object *arg = cell->car;
		args.push_back(eval(arg, env));
		while (recurser->cdr != nullptr)
		{
			if (recurser->cdr->type != &Pair_Type)
			{
				throw std::exception("I think this means a parsing error");
			}
			recurser = (Pair *)recurser->cdr;
			Object *evald = eval(recurser->car, env);
			args.push_back(evald);
		}
		if (args[0]->type != &Procedure_Type)
		{
			throw std::exception("Not callable");
		}
		Procedure *proc = (Procedure *)args[0];
		args.erase(args.begin());
		return proc->evaluate(args);
	}

	throw std::exception("Missed something");
}