#pragma once
#include <vector>
#include <optional>
#include <memory>
#include "../INode.h"
#include "../buildList.h"
#include "../expression/ExpressionFactory.h"
#include "../expression/FunctionCall.h"

struct IOperator : public INode {};

struct FunctionCallOperator : public FunctionCall, public IOperator
{
    using FunctionCall::FunctionCall;
};
