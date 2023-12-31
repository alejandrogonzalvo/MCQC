#include "pass/GateDecompositionPass.hpp"

void GateDecompositionPass::enterGateStatement(qasm3Parser::GateStatementContext *ctx) {
    gates.push_back(ctx);
    inside_gate_definition = true;
}

void GateDecompositionPass::exitGateStatement(qasm3Parser::GateStatementContext *ctx) {
    inside_gate_definition = false;
    rewriter.Delete(ctx->getStart()->getTokenIndex(), ctx->getStop()->getTokenIndex());
}

void GateDecompositionPass::enterGateCallStatement(qasm3Parser::GateCallStatementContext *ctx) {
    if (inside_gate_definition) {
        return;
    }

    size_t start_index = ctx->getStart()->getTokenIndex();
    size_t stop_index = ctx->getStop()->getTokenIndex();

    for (auto gate : gates) {
        if (gate->Identifier()->getText() != ctx->Identifier()->getText()) {
            continue;
        }

        replace_gate_call(ctx, gate);
        rewriter.Delete(start_index, stop_index);
        return;
    }
}

// PRIVATE METHODS

void GateDecompositionPass::insert_statement(qasm3Parser::GateCallStatementContext* ctx, qasm3Parser::GateStatementContext* gate, qasm3Parser::StatementContext* statement) {
    auto statement_tokens = getTerminalNodes(statement);
    reverse(statement_tokens.begin(), statement_tokens.end());
    size_t stop_index = ctx->getStop()->getTokenIndex();
    auto call_qubits = ctx->gateOperandList()->gateOperand();

    for (auto terminalNode : statement_tokens) {
        string text = terminalNode->getText();
        int i = find_qubit(gate->qubits->Identifier(), text);
        if (i != call_qubits.size()) {
            rewriter.insertAfter(stop_index, call_qubits[i]->getText());
            continue;
        }
        rewriter.insertAfter(stop_index, text);
    }
}

void GateDecompositionPass::replace_gate_call(qasm3Parser::GateCallStatementContext* ctx, qasm3Parser::GateStatementContext* gate) {
    auto statements = gate->scope()->statement();
    reverse(statements.begin(), statements.end());
    for (auto statement : statements) {
        insert_statement(ctx, gate, statement);
    }
}

int GateDecompositionPass::find_qubit(const vector<tree::TerminalNode*>& qubits, const string& qubit) {
    int i = 0;
    for (auto q : qubits) {
        if (q->getText() == qubit) {
            return i;
        }
        i++;
    }
    return i;
}