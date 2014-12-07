#pragma once 

#include <deque>
#include "core.h"
#include "types.h"

std::deque<std::string> tokenize(std::string const &input)
{
	std::deque<std::string> tokens;
	std::string accum;
	
	for (auto it = input.begin(); it != input.end(); ++it)
	{
		if (*it == '(' || *it == ')')
		{
			if (!accum.empty())
			{
				tokens.push_back(accum);
				accum.clear();
			}
			tokens.push_back(std::string(1, *it));
		}
		else if (*it == ' ' || *it == '\n' || *it == '\r')
		{
			if (!accum.empty())
			{
				tokens.push_back(accum);
				accum.clear();
			}
		}
		else
		{
			accum += *it;
		}
	}
	if (!accum.empty())
	{
		tokens.push_back(accum);
		accum.clear();
	}
	
	return tokens;
}

Object *atom(std::string const &token)
{
	if (token.length() == 2 && token[0] == '#')
	{
		if (token[1] == 'f')
			return new Boolean(false);
		if (token[1] == 't')
			return new Boolean(true);
	}
	try
	{
		long l = std::stol(token);
		return new Number(l);
	}
	catch (std::exception)
	{
	}
	
	return new Symbol(token);
}

Object *read_from(std::deque<std::string> &tokens)
{
	if (tokens.empty())
	{
		throw std::runtime_error("Unexpected EOF while reading");
	}
	
	std::string token = tokens.front();
	tokens.pop_front();
	
	if (token == "(")
	{
		Pair *c = nullptr;
		Pair **walk = &c;
		while (tokens[0] != ")")
		{
			Object *obj = read_from(tokens);
			*walk = new Pair(obj, nullptr);
			walk = (Pair **)&((*walk)->cdr);
		}
		tokens.pop_front();
		return c;
	}
	if (token == ")")
	{
		throw std::runtime_error("Unexpected )");
	}
	// otherwise, atom
	return atom(token);
}

void find_and_replace(std::string &s,
                      std::string const &toReplace,
                      std::string const &replaceWith)
{
	auto find = s.find(toReplace);
	while (find < s.length())
	{
		s.replace(find, toReplace.length(), replaceWith);
		find = s.find(toReplace, find + replaceWith.length());
	}
}

Object *parse(std::string const &input)
{
	std::string pad = input;
	find_and_replace(pad, "(", " ( ");
	find_and_replace(pad, ")", " ) ");
	auto tokens = tokenize(input);
	return read_from(tokens);
}
