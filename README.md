# jit_and_aot
For practice at JIT and AOT course

### CFG examples

![](pics/examples/fact.svg)\
*CFG for Factorial func*

![](pics/tests/func_001.svg)\
*CFG for test func #001*

![](pics/tests/func_002.svg)\
*CFG for test func #002*

![](pics/tests/func_003.svg)\
*CFG for test func #003*

![](pics/tests/func_004.svg)\
*CFG for test func #004*

![](pics/tests/func_005.svg)\
*CFG for test func #005*

![](pics/tests/func_006.svg)\
*CFG for test func #006*

### Dominator tree examples

![](pics/tests/func_001_dom.svg)\
*Dominator tree for test func #001*

![](pics/tests/func_002_dom.svg)\
*Dominator tree for test func #002*

![](pics/tests/func_003_dom.svg)\
*Dominator tree for test func #003*

### Loop tree examples

![](pics/tests/func_001_loop_tree.svg)\
*Loop tree for test func #001*

![](pics/tests/func_002_loop_tree.svg)\
*Loop tree for test func #002*

![](pics/tests/func_003_loop_tree.svg)\
*Loop tree for test func #003*

![](pics/tests/func_004_loop_tree.svg)\
*Loop tree for test func #004*

![](pics/tests/func_005_loop_tree.svg)\
*Loop tree for test func #005*

![](pics/tests/func_006_loop_tree.svg)\
*Loop tree for test func #006*

### Peephole module

See examples/src/peephole.cpp for jit_aot::peephole module usage example

build/examples/peephole output:

```
IR before peepholes:
i32 foo (i32 %0, i32 %1)
bb1:
  i32 %2 = sub i32 %0, i32 0
  i32 %3 = sub i32 42, i32 0
  i32 %4 = shl i32 %3, i32 10
  i32 %5 = shl i32 %2, i32 0
  i32 %6 = shl i32 0, i32 %1
  i32 %7 = or i32 %2, i32 %3
  i32 %8 = or i32 %7, i32 %4
  i32 %9 = or i32 %8, i32 %5
  i32 %10 = or i32 %9, i32 %6
  return i32 %10


IR after peepholes:
i32 foo (i32 %0, i32 %1)
bb1:
  i32 %7 = or i32 %0, i32 42
  i32 %8 = or i32 %7, i32 43008
  i32 %9 = or i32 %8, i32 %0
  return i32 0
```
