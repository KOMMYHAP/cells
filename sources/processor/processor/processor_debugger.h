#pragma once

class ProcessorContext;

class ProcessorDebugger {
public:
    virtual bool ShouldAttachDebugger(const ProcessorContext& context) const = 0;
    virtual void AttachDebugger(ProcessorContext& context) = 0;
    virtual void DetachDebugger(ProcessorContext& context) = 0;

protected:
    ~ProcessorDebugger() = default;
};