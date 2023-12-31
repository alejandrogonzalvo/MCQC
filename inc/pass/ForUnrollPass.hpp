#pragma once

#include "pass/BasePass.hpp"
#include <algorithm>

class ForUnrollPass : public BasePass {
private:
    void unroll_statements(int start_int, int end_int, int step_int, vector<qasm3Parser::StatementContext*> statement, qasm3Parser::ForStatementContext* ctx);
    int inside_for_statement = 0;
public:
    using BasePass::BasePass;

    void enterForStatement(qasm3Parser::ForStatementContext *ctx) override;
    void exitForStatement(qasm3Parser::ForStatementContext *ctx) override;

    bool recursive_for_statement = false;
};