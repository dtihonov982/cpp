#ifndef RPN_H
#define RPN_H

#include "Expression.h"

namespace rpn {

void convertToRPN(Expression& expression);
Number eval(Expression& rpn);

} 


#endif
