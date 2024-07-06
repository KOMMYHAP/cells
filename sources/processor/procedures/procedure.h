#pragma once

class ProcedureContext;

class ProcedureBase {
public:
    virtual ~ProcedureBase() = default;
    virtual void Execute(ProcedureContext& context) = 0;
};
