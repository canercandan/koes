#ifndef __TYPEDEFS_H__
# define __TYPEDEFS_H__

# include <string>

typedef std::string	Data;
typedef Data		Fact;
typedef Data		Operator;

enum	OperatorEnum {FACT = 0, RULE, AND, OR, XOR, NOT};

struct	OperatorStruct
{
  OperatorEnum	op;
  Operator	desc;
};

enum	Value {TRUE = 1, FALSE = 0, UNKNOWN = -1};

#endif // !__TYPEDEFS_H__
