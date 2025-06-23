#include <os/types.h>
#include <os/kprintf.h>
#include <os/kernel.h>
#include <debug/debug_all.h>
#include <sbi.h>

void sbi_test(void)
{
    struct sbi_ret ret;
    
    DEBUG("Starting SBI interface test");
    TEST_START("SBI Interface Test");
    
    /* 测试SBI规范版本 */
    ret = sbi_get_spec_version();
    if (ret.error == SBI_SUCCESS) {
        INFO("SBI Specification Version: %d.%d", 
                (int)(ret.value >> 24) & 0x7F, 
                (int)(ret.value & 0xFFFFFF));
    } else {
        ERROR("Failed to get SBI spec version (error: %ld)", ret.error);
    }
    
    /* 测试SBI实现ID */
    ret = sbi_get_impl_id();
    if (ret.error == SBI_SUCCESS) {
        INFO("SBI Implementation ID: %ld", ret.value);
        switch (ret.value) {
            case 0: INFO("  (Berkeley Boot Loader)"); break;
            case 1: INFO("  (OpenSBI)"); break;
            case 2: INFO("  (Xvisor)"); break;
            case 3: INFO("  (KVM)"); break;
            case 4: INFO("  (RustSBI)"); break;
            case 5: INFO("  (Diosix)"); break;
            default: WARN("  (Unknown implementation)"); break;
        }
    } else {
        ERROR("Failed to get SBI impl ID (error: %ld)", ret.error);
    }
    
    /* 测试SBI实现版本 */
    ret = sbi_get_impl_version();
    if (ret.error == SBI_SUCCESS) {
        INFO("SBI Implementation Version: 0x%lx", ret.value);
    } else {
        ERROR("Failed to get SBI impl version (error: %ld)", ret.error);
    }
    
    /* 测试扩展探测 */
    INFO("Probing SBI extensions:");
    
    const struct {
        int ext_id;
        const char *name;
    } extensions[] = {
        {SBI_EXT_BASE, "Base"},
        {SBI_EXT_TIMER, "Timer"},
        {SBI_EXT_IPI, "IPI"},
        {SBI_EXT_RFENCE, "RFENCE"},
        {SBI_EXT_HSM, "HSM"},
        {SBI_EXT_SRST, "System Reset"},
        {SBI_EXT_DBCN, "Debug Console"},
    };
    
    for (int i = 0; i < 7; i++) {
        ret = sbi_probe_extension(extensions[i].ext_id);
        if (ret.error == SBI_SUCCESS && ret.value == 1) {
            TEST_PASS("Extension %s: Available", extensions[i].name);
            DEBUG("Extension %s (ID: %d) is available", extensions[i].name, extensions[i].ext_id);
        } else {
            TEST_FAIL("Extension %s: Not available", extensions[i].name);
        }
    }
    
    TEST_END("SBI Test Complete");
}
