# mympz
整型大数运算算法。

# 编译

在编译本库之前请先安装`cmake`以及`gtest`库，因为在测试文件中需要使用`gtest`进行测试。在安装完成后，使用如下命令进行编译。

```bash
mkdir build
cd build
cmake ..
make
```

编译完成后，可以使用`make install`进行安装。

## 编译选项

目前仅支持linux下进行编译，并且仅支持x64的汇编优化。如果要禁止汇编优化采用软件模拟方式，则在`CMakeLists.txt`中修改编译选项增加`-DDISABLE_OPTIMIZE`选项。如果使用了宏`-DDEBUG`选项则开启调试日志打印功能。

## 调试输出

在`compile.h`中定义了一组*DEBUG_XXX*的调试输出，按照不同运算定了不同的宏，可以通过注释关闭或开启在调试中的运算日志打印。

## CMakeList.txt

在`src`目录下的`CMakeList.txt`的开头，可以进行开启关闭注释编译不同的版本。

```
cmake_minimum_required (VERSION 3.5)
project(mympz VERSION 1.0)

# -g allows for gdb debugging
# turn on -03 for best performance

#
# 带调试编译带汇编优化的库
#
# add_definitions(-std=c++11 -g -O0 -DDEBUG -Wunused-variable)
# add_definitions(-std=c++11 -g -O0 -DDEBUG -DDISABLE_OPTIMIZE -Wunused-variable)

#
# 编译发布版本
#
add_definitions(-std=c++11 -O3 -Wunused-variable)
```

# 使用说明

在使用时，仅需包含头文件`#include <mympz/mympz.h>`即可使用。下面分别介绍大数结构以及相关功能函数列表。随后进行链接库文件`libmympz.a`即可。

## 大数结构

```c++
  typedef struct
  {
    number_t number; ///< 保存数值的队列
    int neg;         ///< 当前数是负数
  } bignum_t;
```

`number`是一个`std::deque<unit_t>`的结构，`neg`表示正负号。

## 函数列表

|名称|说明|
|---|----|
|`init_bignum`|初始化大数结构|
|`is_negative`|大数是否为负数|
|`is_positive`|大数是否为整数|
|`is_null`|大数是否是空值|
|`is_error`|函数是否返回错误|
|`set_negative`|设置大数为负数|
|`set_positive`|设置大数为正数|
|`set_null`|设置大数为空值|
|`set_error`|设置大数为错误值|
|`bn_size`|返回大数的长度|
|`bn_resize`|重新设置大数的长度|
|`bn_ptr`|返回大数的指针|
|`bn_ptr2`|返回大数的指针+上某个索引|
|`bn_const_cast`|将静态引用转换为非静态引用|
|`num_size`|返回`number_t`的类型|
|`num_resize`|重新设定`number_t`类型的长度|
|`num_ptr`|返回`number_t`类型的指针|
|`num_ptr2`|返回`number_t`类型的指针+上某个索引|
|`num_const_cast`|将`number_t`静态引用转换为非静态引用|
|`num_ptr_const_cast`|将`number_t`静态引用指针转换为非静态引用指针|
|`clear_head_zero`|清除大数结构的前面多余的零|
|`create`|*四个版本*通过字单元；字符串；`number_t`；缓冲区的创建大数结构|
|`is_zero`|是否是零|
|`is_one`|是否是一|
|`is_odd`|是否是奇数|
|`is_abs_word`|判断大数的绝对值是某个字|
|`is_word`|判断大数的值等于某个字|
|`zero`|将大数设置为0|
|`one`|将大数设置为1|
|`get_word`|获取大数的一个字|
|`set_word`|设置大数的一个字|
|`set_bit`|设置大数的某位|
|`clear_bit`|清除大数的某位|
|`is_bit_set`|大数的某位是否被设置|
|`mask_bits`|对大数某些位取到某位|
|`bignum_bits`|大数的位数|
|`cmp`|比较|
|`ucmp`|无符号比较|
|`add`|加法|
|`uadd`|无符号加法|
|`sub`|减法|
|`usub`|无符号减法|
|`mul`|乘法|
|`sqr`|平方函数|
|`exp`|指数函数|
|`div`|除法返回商与余数|
|`idiv`|除法仅返回商|
|`lshift1`|左移动1位|
|`rshift1`|右移动1位|
|`lshift`|左移动n位|
|`rshift`|右移动n位|
|`gcd`|最大公约数|
|`nnmod`|取模数为绝对值的模|
|`mod`|模运算|
|`mod_add`|模加|
|`mod_sub`|模减|
|`mod_mul`|模乘|
|`mod_sqr`|平方后去模|
|`mod_lshift1`|左移1位然后去模|
|`mod_lshift`|左移n位然后去模|
|`mod_exp`|模幂|
|`mod_inverse`|模逆|
|`print_string`|打印大数为字符串|
|`print_buffer`|打印大数到缓冲区|
|`const_null`|*常数变量*，大数结构的空值|
|`const_zero`|*常数变量*，大数结构的零值|
|`const_one`|*常数变量*，大数结构的一值|

# 异常说明

|异常|
|---|
|除0异常|
|值超出范围|
|变量为null值|
|无效参数|
|蒙哥马利模乘法超出有效位数|
|蒙哥马利模幂的模数为偶数|
|错误的倒数|

# 测试文件

|测试文件|说明|
|-------|---|
|`test/test_addsub.cc`|对加减法进行测试|
|`test/test_create.cc`|对创建大数进行测试|
|`test/test_div.cc`|对除法进行测试|
|`test/test_exp.cc`|对指数进行测试|
|`test/test_gcd.cc`|对最大公约数进行测试|
|`test/test_mod.cc`|对模数进行测试|
|`test/test_mul.cc`|对乘法进行测试|
|`test/test_shift.cc`|对位移进行测试|
|`test/test_sqr.cc`|对平方进行测试|