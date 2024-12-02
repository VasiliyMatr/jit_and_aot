#ifndef TEST_FUNCS_HPP
#define TEST_FUNCS_HPP

#include <jit_aot/ir/builder.hpp>

namespace jit_aot::testing {

static const ir::IntType i32{32};

ir::Function makeTestFunc1(ir::Builder &builder) {
    ir::Function fn{i32, {i32, i32}};
    fn.setName("test_func_001");
    builder.setFunc(&fn);

    auto *v0 = fn.nthArg(0);
    auto *v1 = fn.nthArg(1);

    auto *A = builder.newBb("A", true);
    auto *B = builder.newBb("B");
    auto *C = builder.newBb("C");
    auto *D = builder.newBb("D");
    auto *E = builder.newBb("E");
    auto *F = builder.newBb("F");
    auto *G = builder.newBb("G");

    builder.setPos(A);
    builder.makeDirectUncondBranch(B);

    builder.setPos(B);
    builder.makeDirectCondBranch(C, F, v0);

    builder.setPos(C);
    builder.makeDirectUncondBranch(D);

    builder.setPos(D);
    builder.makeReturn(v0);

    builder.setPos(E);
    builder.makeDirectUncondBranch(D);

    builder.setPos(F);
    builder.makeDirectCondBranch(E, G, v1);

    builder.setPos(G);
    builder.makeDirectUncondBranch(D);

    return fn;
}

ir::Function makeTestFunc2(ir::Builder &builder) {
    ir::Function fn{i32, {i32, i32, i32, i32}};
    fn.setName("test_func_002");
    builder.setFunc(&fn);

    auto *v0 = fn.nthArg(0);
    auto *v1 = fn.nthArg(1);
    auto *v2 = fn.nthArg(2);
    auto *v3 = fn.nthArg(3);

    auto *A = builder.newBb("A", true);
    auto *B = builder.newBb("B");
    auto *C = builder.newBb("C");
    auto *D = builder.newBb("D");
    auto *E = builder.newBb("E");
    auto *F = builder.newBb("F");
    auto *G = builder.newBb("G");
    auto *H = builder.newBb("H");
    auto *I = builder.newBb("I");
    auto *J = builder.newBb("J");
    auto *K = builder.newBb("K");

    builder.setPos(A);
    builder.makeDirectUncondBranch(B);

    builder.setPos(B);
    builder.makeDirectCondBranch(C, J, v0);

    builder.setPos(C);
    builder.makeDirectUncondBranch(D);

    builder.setPos(D);
    builder.makeDirectCondBranch(C, E, v1);

    builder.setPos(E);
    builder.makeDirectUncondBranch(F);

    builder.setPos(F);
    builder.makeDirectCondBranch(E, G, v2);

    builder.setPos(G);
    builder.makeDirectCondBranch(H, I, v3);

    builder.setPos(H);
    builder.makeDirectUncondBranch(B);

    builder.setPos(I);
    builder.makeDirectUncondBranch(K);

    builder.setPos(J);
    builder.makeDirectUncondBranch(C);

    builder.setPos(K);
    builder.makeReturn(v0);

    return fn;
}

ir::Function makeTestFunc3(ir::Builder &builder) {
    ir::Function fn{i32, {i32, i32, i32, i32, i32}};
    fn.setName("test_func_003");
    builder.setFunc(&fn);

    auto *v0 = fn.nthArg(0);
    auto *v1 = fn.nthArg(1);
    auto *v2 = fn.nthArg(2);
    auto *v3 = fn.nthArg(3);
    auto *v4 = fn.nthArg(4);

    auto *A = builder.newBb("A", true);
    auto *B = builder.newBb("B");
    auto *C = builder.newBb("C");
    auto *D = builder.newBb("D");
    auto *E = builder.newBb("E");
    auto *F = builder.newBb("F");
    auto *G = builder.newBb("G");
    auto *H = builder.newBb("H");
    auto *I = builder.newBb("I");

    builder.setPos(A);
    builder.makeDirectUncondBranch(B);

    builder.setPos(B);
    builder.makeDirectCondBranch(C, E, v0);

    builder.setPos(C);
    builder.makeDirectUncondBranch(D);

    builder.setPos(D);
    builder.makeDirectUncondBranch(G);

    builder.setPos(E);
    builder.makeDirectCondBranch(F, D, v1);

    builder.setPos(F);
    builder.makeDirectCondBranch(B, H, v2);

    builder.setPos(G);
    builder.makeDirectCondBranch(C, I, v3);

    builder.setPos(H);
    builder.makeDirectCondBranch(G, I, v4);

    builder.setPos(I);
    builder.makeReturn(v0);

    return fn;
}

ir::Function makeTestFunc4(ir::Builder &builder) {
    ir::Function fn{i32, {i32}};
    fn.setName("test_func_004");
    builder.setFunc(&fn);

    auto *v0 = fn.nthArg(0);

    auto *A = builder.newBb("A", true);
    auto *B = builder.newBb("B");
    auto *C = builder.newBb("C");
    auto *D = builder.newBb("D");
    auto *E = builder.newBb("E");

    builder.setPos(A);
    builder.makeDirectUncondBranch(B);

    builder.setPos(B);
    builder.makeDirectCondBranch(C, E, v0);

    builder.setPos(C);
    builder.makeDirectUncondBranch(D);

    builder.setPos(D);
    builder.makeDirectUncondBranch(B);

    builder.setPos(E);
    builder.makeReturn(v0);

    return fn;
}

ir::Function makeTestFunc5(ir::Builder &builder) {
    ir::Function fn{i32, {i32, i32}};
    fn.setName("test_func_005");
    builder.setFunc(&fn);

    auto *v0 = fn.nthArg(0);
    auto *v1 = fn.nthArg(1);

    auto *A = builder.newBb("A", true);
    auto *B = builder.newBb("B");
    auto *C = builder.newBb("C");
    auto *D = builder.newBb("D");
    auto *E = builder.newBb("E");
    auto *F = builder.newBb("F");

    builder.setPos(A);
    builder.makeDirectUncondBranch(B);

    builder.setPos(B);
    builder.makeDirectUncondBranch(C);

    builder.setPos(C);
    builder.makeDirectCondBranch(D, F, v0);

    builder.setPos(D);
    builder.makeDirectCondBranch(E, F, v1);

    builder.setPos(E);
    builder.makeDirectUncondBranch(B);

    builder.setPos(F);
    builder.makeReturn(v0);

    return fn;
}

ir::Function makeTestFunc6(ir::Builder &builder) {
    ir::Function fn{i32, {i32, i32, i32}};
    fn.setName("test_func_006");
    builder.setFunc(&fn);

    auto *v0 = fn.nthArg(0);
    auto *v1 = fn.nthArg(1);
    auto *v2 = fn.nthArg(2);

    auto *A = builder.newBb("A", true);
    auto *B = builder.newBb("B");
    auto *C = builder.newBb("C");
    auto *D = builder.newBb("D");
    auto *E = builder.newBb("E");
    auto *F = builder.newBb("F");
    auto *G = builder.newBb("G");
    auto *H = builder.newBb("H");

    builder.setPos(A);
    builder.makeDirectUncondBranch(B);

    builder.setPos(B);
    builder.makeDirectCondBranch(C, D, v0);

    builder.setPos(C);
    builder.makeDirectCondBranch(E, H, v1);

    builder.setPos(D);
    builder.makeDirectUncondBranch(E);

    builder.setPos(E);
    builder.makeDirectUncondBranch(F);

    builder.setPos(F);
    builder.makeDirectCondBranch(B, G, v2);

    builder.setPos(G);
    builder.makeDirectUncondBranch(A);

    builder.setPos(H);
    builder.makeReturn(v0);

    return fn;
}

} // namespace jit_aot::testing

#endif // TEST_FUNCS_HPP
