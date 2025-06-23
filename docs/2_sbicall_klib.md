<!-- 主要内容：
1. 说明为何我们放弃了原本的uart实现，而完全使用sbi_call来实现kprintf
2. 说明为何我们要实现一些基础的标准库函数，主要是内存相关的
3. 说明我们是如何进行实现sbi的，包括对于sbi_call的基础介绍。可以放上一个riscv sbi规范中文文档链接：https://zhuanlan.zhihu.com/p/658161795
4. 说明我们是如何实现相关klib库函数的，为什么有的返回类型是void，有的是void *。 -->

# 2 sbi_call与klib库



