#include "core.h"
#include "types.h"
#include "builtins.h"
#include "eval.h"
#include "parse.h"

#include <iostream>


Environment global(nullptr);

char *initialize[] =
{
	"(define != (lambda (a b) (not (= a b))))",
	"(define < (lambda (a b) (> b a)))",
	"(define <= (lambda (a b) (not (> a b))))",
	"(define >= (lambda (a b) (not (< a b))))",
	nullptr
};

#define BUILTIN(NAME, CLASS) global.set(#NAME, new CLASS(&global));

int main()
{
	BUILTIN(+, Add);
	BUILTIN(-, Subtract);
	BUILTIN(*, Multiply);
	BUILTIN(/, Divide);
	BUILTIN(or, Or);
	BUILTIN(and, And);
	BUILTIN(not, Not);
	BUILTIN(=, Equal);
	BUILTIN(>, Greater);
	BUILTIN(cons, Cons);
	BUILTIN(car, Car);
	BUILTIN(cdr, Cdr);
	BUILTIN(list?, IsList);
	BUILTIN(symbol?, IsSymbol);
	BUILTIN(null?, IsNull);

	for (char **expr = initialize; *expr != nullptr; ++expr)
	{
		std::string line(*expr);
		eval(parse(line), &global);
	}

	std::string line;
	while (true)
	{
		try
		{
			std::getline(std::cin, line);
			Object *val = eval(parse(line), &global);
			std::cout << to_string(val) << std::endl;
		}
		catch (std::exception &e)
		{
			std::cout << e.what() << std::endl;
		}
	}

	std::string _;
	std::cin >> _;
}