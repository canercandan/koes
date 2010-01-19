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

enum	Boolean {TRUE = 0, FALSE, UNKNOWN};

struct  BooleanStruct
{
  Boolean	bo;
  Data		desc;
};

typedef Boolean (*op_func)(Boolean, Boolean);

#endif // !__TYPEDEFS_H__
