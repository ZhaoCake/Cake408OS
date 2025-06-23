#include <os/types.h>
#include <os/kprintf.h>
#include <sbi.h>

void sbi_test(void)
{
    struct sbi_ret ret;
    
    kputs("\n=== SBI Interface Test ===");
    
    /* 测试SBI规范版本 */
    ret = sbi_get_spec_version();
    if (ret.error == SBI_SUCCESS) {
        kprintf("SBI Specification Version: %d.%d\n", 
                (int)(ret.value >> 24) & 0x7F, 
                (int)(ret.value & 0xFFFFFF));
    } else {
        kprintf("Failed to get SBI spec version (error: %ld)\n", ret.error);
    }
    
    /* 测试SBI实现ID */
    ret = sbi_get_impl_id();
    if (ret.error == SBI_SUCCESS) {
        kprintf("SBI Implementation ID: %ld\n", ret.value);
        switch (ret.value) {
            case 0: kprintf("  (Berkeley Boot Loader)\n"); break;
            case 1: kprintf("  (OpenSBI)\n"); break;
            case 2: kprintf("  (Xvisor)\n"); break;
            case 3: kprintf("  (KVM)\n"); break;
            case 4: kprintf("  (RustSBI)\n"); break;
            case 5: kprintf("  (Diosix)\n"); break;
            default: kprintf("  (Unknown)\n"); break;
        }
    } else {
        kprintf("Failed to get SBI impl ID (error: %ld)\n", ret.error);
    }
    
    /* 测试SBI实现版本 */
    ret = sbi_get_impl_version();
    if (ret.error == SBI_SUCCESS) {
        kprintf("SBI Implementation Version: 0x%lx\n", ret.value);
    } else {
        kprintf("Failed to get SBI impl version (error: %ld)\n", ret.error);
    }
    
    /* 测试扩展探测 */
    kputs("Probing SBI extensions:");
    
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
            kprintf("  %s: Available\n", extensions[i].name);
        } else {
            kprintf("  %s: Not available\n", extensions[i].name);
        }
    }
    
    kputs("=== SBI Test Complete ===\n");
}
