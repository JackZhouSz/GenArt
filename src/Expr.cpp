#include "ExprTools.h"
#include "MathHelpers.h"
#include "NonaryExprSubclasses.h"

#include <Math/Random.h>
#include <Util/Assert.h>
#include <algorithm>
#include <iostream>
#include <string>

interval Expr::sIval(const opInfo& opI, const interval& lv /* = interval() */, const interval& rv /* = interval() */) const
{
    return sampleIval(this, opI, lv, rv);
}

Expr* Expr::PerturbConstants(const float rc)
{
    if (left) left->PerturbConstants(rc);
    if (right) right->PerturbConstants(rc);

    return this;
}

Expr::Expr()
{
    left = right = NULL;
    nodeCount = 0;
    tokenCount = 0;
    varMask = 0u;
    // Ivl is still empty
}

Expr::~Expr()
{
    if (left) delete left;
    left = NULL;
    if (right) delete right;
    right = NULL;
}

Expr* Expr::GrabL()
{
    Expr* Tmp = left;
    left = NULL;
    nodeCount = 0; // Invalidate my records
    return Tmp;
}

Expr* Expr::GrabR()
{
    Expr* Tmp = right;
    right = NULL;
    nodeCount = 0; // Invalidate my records
    return Tmp;
}

unsigned int Expr::VarMask() const
{
    ASSERT_R(nodeCount > 0);

    return varMask;
}

int Expr::size() const
{
    // ASSERT_R(nodeCount > 0);

    int cnt = 1;
    if (left) cnt += left->size();
    if (right) cnt += right->size();
    // ASSERT_R(cnt == nodeCount);
    // nodeCount = cnt;

    return cnt; // A hack
}

int Expr::TokenCount() const
{
    // ASSERT_R(nodeCount > 0 && tokenCount > 0);

    int cnt = 1;
    if (left)
        cnt += left->TokenCount();
    else
        cnt++; // XXX Handle Var and Const
    if (right) cnt += right->TokenCount();

    return cnt;
    // return tokenCount;
}

Expr* const* Expr::FindRand(int& count) const
{
    int lc = 0, rc = 0;

    Expr* const* lp = left ? left->FindRand(lc) : NULL;
    Expr* const* rp = right ? right->FindRand(rc) : NULL;

    count = lc + rc + 1;

    if (chance(lc, lc + rc))
        if (chance(1, lc))
            return &left;
        else
            return lp;
    else if (chance(1, rc))
        return &right;
    else
        return rp;
}
