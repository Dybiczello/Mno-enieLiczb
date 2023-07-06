#include <linux/kernel.h>       
#include <linux/module.h>       
#include <linux/proc_fs.h>      
#include <linux/ioport.h>
#include <linux/uaccess.h>
#include <asm/uaccess.h>        
#include <asm/io.h>             
#include <linux/sysfs.h>

MODULE_INFO(intree, "Y");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mateusz Dybicz");
MODULE_DESCRIPTION("Simple kernel module prepared for SYKOM project");

#define MY_SYSFSDRV_BUFFER_LEN 4096

#define TIMEOUT 1000
#define SYKT_BUF_SIZE             100
#define SYKT_MEM_SIZE             16
#define SYKT_MEM_BASE_ADDR        (0x00100000)
#define SYKT_FINISHER_FAIL        (0x3333)
#define SYKT_QEMU_CTRL_ADDR       (0x00100000)
#define SYKT_GPIO_SIZE 			  (0x8000) 
#define SYKT_GPIO_ADDRESS_SPACE       (0x00100000)                           
#define SYKT_EXIT_CODE (0x7F)
#define SYKT_ID_ADDR              ((SYKT_GPIO_ADDRESS_SPACE) + 4)                       
#define SYKT_GPIO_A1_ADDR        ((SYKT_GPIO_ADDRESS_SPACE) + 0x100)  //dmda1
#define SYKT_GPIO_A2_ADDR        ((SYKT_GPIO_ADDRESS_SPACE) + 0x108)  //dmda2
#define SYKT_GPIO_W_ADDR         ((SYKT_GPIO_ADDRESS_SPACE) + 0x110)  //dmdw
#define SYKT_GPIO_L_ADDR         ((SYKT_GPIO_ADDRESS_SPACE) + 0x118)  //dmdl
#define SYKT_GPIO_B_ADDR         ((SYKT_GPIO_ADDRESS_SPACE) + 0x120)  //dmdb
#define SYKT_GPIO_COUNTER_ADDR   ((SYKT_GPIO_ADDRESS_SPACE) + 0x130)  //counter

void __iomem *baseptr;                      //FINISHER
void __iomem *A1ptr;
void __iomem *A2ptr;
void __iomem *Wptr;
void __iomem *Lptr;
void __iomem *Bptr;
void __iomem *COUNTERptr;

static struct kobject *kobj_ref;               //wskaźnik na kobject

static int dmda1_v;                    //wartość A1
static int dmda2_v;                  //wartość A2
static int dmdw_v;                     //wartość W
static int dmdl_v;                     //wartość L
static int dmdb_v;                     //wartość B
static int counter_v;                  //wartość COUNTER




//zapis i wyswietlanie A1

static ssize_t dmda1_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%x", &dmda1_v);
    writel(dmda1_v, A1ptr);
    int n = count > MY_SYSFSDRV_BUFFER_LEN ? MY_SYSFSDRV_BUFFER_LEN : count;
    return n;
}

static ssize_t dmda1_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    dmda1_v = readl(A1ptr);
    return sprintf(buf, "0x%x\n", dmda1_v);
}

//zapis i wyswietlanie A2

static ssize_t dmda2_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%x", &dmda2_v);
    writel(dmda2_v, A2ptr);
    int n = count > MY_SYSFSDRV_BUFFER_LEN ? MY_SYSFSDRV_BUFFER_LEN : count;
    return n;
}

static ssize_t dmda2_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    dmda2_v = readl(A2ptr);
    return sprintf(buf, "0x%x\n", dmda2_v);
}

//zapis i wyswietlanie W

static ssize_t dmdw_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%x", &dmdw_v);
    writel(dmdw_v, Wptr);
    int n = count > MY_SYSFSDRV_BUFFER_LEN ? MY_SYSFSDRV_BUFFER_LEN : count;
    return n;
}

static ssize_t dmdw_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    dmdw_v = readl(Wptr);
    return sprintf(buf, "0x%x\n", dmdw_v);
}

//zapis i wyswietlanie L

static ssize_t dmdl_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%x", &dmdl_v);
    writel(dmdl_v, Lptr);
    int n = count > MY_SYSFSDRV_BUFFER_LEN ? MY_SYSFSDRV_BUFFER_LEN : count;
    return n;
}

static ssize_t dmdl_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    dmdl_v = readl(Lptr);
    return sprintf(buf, "0x%x\n", dmdl_v);
}

//zapis i wyswietlanie B

static ssize_t dmdb_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%x", &dmdb_v);
    writel(dmdb_v, Bptr);
    int n = count > MY_SYSFSDRV_BUFFER_LEN ? MY_SYSFSDRV_BUFFER_LEN : count;
    return n;
}

static ssize_t dmdb_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    dmdb_v = readl(Bptr);
    return sprintf(buf, "0x%x\n", dmdb_v);
}

//zapis i wyswietlanie COUNTER

static ssize_t counter_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count) {
    sscanf(buf, "%x", &counter_v);
    writel(counter_v, COUNTERptr);
    int n = count > MY_SYSFSDRV_BUFFER_LEN ? MY_SYSFSDRV_BUFFER_LEN : count;
    return n;
}

static ssize_t counter_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf) {
    counter_v = readl(COUNTERptr);
    return sprintf(buf, "0x%x\n", counter_v);
}



static struct kobj_attribute dmda1_attr   = __ATTR_RW(dmda1);   //zapis i odczyt
static struct kobj_attribute dmda2_attr   = __ATTR_RW(dmda2);   //zapis i odczyt
static struct kobj_attribute dmdw_attr    = __ATTR_RW(dmdw);    //odczyt
static struct kobj_attribute dmdl_attr    = __ATTR_RW(dmdl);    //odczyt
static struct kobj_attribute dmdb_attr = __ATTR_RW(dmdb);    //odczyt
static struct kobj_attribute counter_attr    = __ATTR_RW(counter); //odczyt

int my_init_module(void){
    printk(KERN_INFO "Starting of the SYKT module...\n");
//    baseptr = ioremap(SYKT_MEM_BASE_ADDR, SYKT_MEM_SIZE);
	baseptr= ioremap(SYKT_GPIO_ADDRESS_SPACE, SYKT_GPIO_SIZE);
	A1ptr       = ioremap(SYKT_GPIO_A1_ADDR, 4);
	A2ptr		 = ioremap(SYKT_GPIO_A2_ADDR, 4);
	Wptr		 = ioremap(SYKT_GPIO_W_ADDR, 4);
	Lptr  		 = ioremap(SYKT_GPIO_L_ADDR, 4);
	Bptr		 = ioremap(SYKT_GPIO_B_ADDR, 4);
	COUNTERptr  = ioremap(SYKT_GPIO_COUNTER_ADDR, 4);
	
  kobj_ref = kobject_create_and_add("sykt", kernel_kobj);
	
	if (!kobj_ref) {
        return -ENOMEM;
    }     
        
    if (baseptr == NULL) {
        printk(KERN_ERR "ioremap failed\n");
        return -EIO;
    }

//obsługa wyjątkow
    if (sysfs_create_file(kobj_ref, &dmda1_attr.attr)) {
        printk(KERN_INFO"cannot create dmda1 sysfs file...\n");
    }

    if (sysfs_create_file(kobj_ref, &dmda2_attr.attr)) {
        printk(KERN_INFO"cannot ceate dmda2 sysfs file...\n");
    }

    if (sysfs_create_file(kobj_ref, &dmdw_attr.attr)) {
        printk(KERN_INFO"cannot ceate dmdw sysfs file...\n");
    }

    if (sysfs_create_file(kobj_ref, &dmdl_attr.attr)) {
        printk(KERN_INFO"cannot ceate dmdl sysfs file...\n");
    }

    if (sysfs_create_file(kobj_ref, &dmdb_attr.attr)) {
        printk(KERN_INFO"cannot ceate dmmb sysfs file...\n");
    }
	
    if (sysfs_create_file(kobj_ref, &counter_attr.attr)) {
        printk(KERN_INFO"cannot ceate counter sysfs file...\n");
    }
    return 0;
}

void my_cleanup_module(void) {
    printk(KERN_INFO "Cleanup my SYKT module.\n");

    kobject_put(kobj_ref);
	
    writel(SYKT_FINISHER_FAIL | ((SYKT_EXIT_CODE) << 16), baseptr);     //wywolanie FINISHER_PASS - konczy prace qemu
	
    sysfs_remove_file(kernel_kobj, &dmda1_attr.attr);
	sysfs_remove_file(kernel_kobj, &dmda2_attr.attr);
	sysfs_remove_file(kernel_kobj, &dmdw_attr.attr);
	sysfs_remove_file(kernel_kobj, &dmdl_attr.attr);
    sysfs_remove_file(kernel_kobj, &dmdb_attr.attr);
    sysfs_remove_file(kernel_kobj, &counter_attr.attr);

	iounmap(baseptr);
}

module_init(my_init_module);
module_exit(my_cleanup_module);