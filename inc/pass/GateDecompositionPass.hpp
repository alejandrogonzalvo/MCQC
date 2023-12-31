#pragma once

#include "pass/BasePass.hpp"

using namespace tree;

class GateDecompositionPass : public BasePass {
private:
    vector<qasm3Parser::GateStatementContext*> gates;
    bool inside_gate_definition = false;

    int find_qubit(const vector<tree::TerminalNode*>& qubits, const string& qubit);
    void insert_statement(qasm3Parser::GateCallStatementContext* ctx, qasm3Parser::GateStatementContext* gate, qasm3Parser::StatementContext* statement);
    void replace_gate_call(qasm3Parser::GateCallStatementContext* ctx, qasm3Parser::GateStatementContext* gate);
    
public:
    using BasePass::BasePass;

    void enterGateStatement(qasm3Parser::GateStatementContext *ctx) override;
    void exitGateStatement(qasm3Parser::GateStatementContext *ctx) override;
    void enterGateCallStatement(qasm3Parser::GateCallStatementContext *ctx) override;
};