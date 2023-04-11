#include <gtest/gtest.h>
#include <PolyIndex.h>

class A {};
class B: public A {};
class C: public A {};
class D: public A {};

struct W {};
struct X: W {};
struct Y: W {};
struct Z: W {};


TEST(poly, ctor) {
    PolyIndex<A, 3> I;
    EXPECT_EQ(I.empty(), true);
    EXPECT_EQ(I.count(), 0);
    EXPECT_EQ(I.has<C>(), false);
    B b;
    I.add<B>(b);
    EXPECT_EQ(I.has<B>(), true);
    I.get<B>();

    C c;
    D d;
    I.add<C>(c);
    I.add<D>(d);

    I.get<C>();
    I.get<D>();
    
    EXPECT_EQ(I.count(), 3);
    
    PolyIndex<W, 2> w;
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

