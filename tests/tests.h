#ifndef _TESTS_H
#define _TESTS_H

/* SBI接口测试 */
void sbi_test(void);

/* 内核恐慌功能测试 */
void test_panic_feature(void);

/* 运行所有测试 */
void run_all_tests(void);

#endif /* _TESTS_H */
