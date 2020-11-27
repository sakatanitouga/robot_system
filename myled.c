#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/uaccess.h>
#include<linux/io.h>

MODULE_AUTHOR("Ryuichi Ueda and Touga Sakatani");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static dev_t dev;
static struct cdev cdv;
static volatile u32 *gpio_base = NULL;

static struct class *cls = NULL;

static ssize_t led_write(struct file *filp,const char *buf,size_t count,loff_t *pos){

        char c;

        if(copy_from_user(&c,buf,sizeof(char)))
                return -EFAULT;


        printk(KERN_INFO "[c:%0d %0x %c]\n",c,c,c);

        printk(KERN_INFO "[c:%c->%d ]\n",'7','7'-'0');
        printk(KERN_INFO "------\n");

        //led全消し
        switch (c){
                case '0':
                        gpio_base[10] = 1 << 24;
                        gpio_base[10] = 1 << 25;
                        gpio_base[10] = 1 << 26;
                        break;
                case '1':
                        gpio_base[7] = 1<< 24;
                        gpio_base[10] = 1 << 25;
                        gpio_base[10] = 1 << 26;
                        break;
                case '2':
                        gpio_base[10] = 1 << 24;
                        gpio_base[10] = 1 << 26;
                        gpio_base[7] = 1<< 25;
                        break;
                case '3':
                        gpio_base[7] = 1<< 24;
                        gpio_base[7] = 1 << 25;
                        gpio_base[10] = 1 << 26;
                        break;

                case '4':
                        gpio_base[10] = 1 << 24;
                        gpio_base[10] = 1 << 25;
                        gpio_base[7] = 1<< 26;
                        break;

                case '5':
                        gpio_base[7] = 1<< 24;
                        gpio_base[10] = 1 << 25;
                        gpio_base[7] = 1<< 26;
                        break;
                case '6':
                        gpio_base[10] = 1<< 24;
                        gpio_base[7] = 1 << 25;
                        gpio_base[7] = 1<< 26;
                        break;

                case '7':
                        gpio_base[7] = 1<< 24;
                        gpio_base[7] = 1<< 25;
                        gpio_base[7] = 1<< 26;
                        break;
        }

        return 1;

}
static ssize_t sushi_read(struct file* filp,char* buf,size_t count,loff_t* pos){
        int size  = 0;
        char sushi[] = {'s','u','s','h','i',0x0A};
        if(copy_to_user(buf+size,(const char *)sushi,sizeof(sushi))){
                printk(KERN_INFO "sushi :copy_to_user failed\n");
                return -EFAULT;
        }
        size += sizeof(sushi);
        return size;
}
static struct file_operations led_fops = {
        .owner = THIS_MODULE,
        .write = led_write,
        .read = sushi_read
};

static int __init init_mod(void){
        int retval;

        gpio_base = ioremap_nocache(0x3f200000,0xA0);

        const u32 led1 = 24;
        const u32 index1 = led1/10;
        const u32 shift1 = (led1%10)*3;
        const u32 mask1 = ~(0x7 << shift1);

        const u32 led2 = 25;
        const u32 shift2 = (led2%10)*3;
        const u32 mask2 = ~(0x7 << shift2);

        const u32 led3 = 26;
        const u32 shift3 = (led3%10)*3;
        const u32 mask3 = ~(0x7 << shift3);


        const u32 mask = mask1 & mask2 & mask3;
        //indexが同じの場合
        gpio_base[index1] = (gpio_base[index1] & mask) | (0x1 << shift1) | ( 0x1 << shift2) | (0x1 << shift3);

        retval = alloc_chrdev_region(&dev,0,1,"myled");
        printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));
        cdev_init(&cdv,&led_fops);
        retval = cdev_add(&cdv, dev,1);
        if(retval < 0){
                printk(KERN_ERR "cdev_add failed major:%d minor:%d \n",MAJOR(dev),MINOR(dev));
                return retval;
        }

        cls = class_create(THIS_MODULE,"myled");

        if(IS_ERR(cls)){
                printk(KERN_ERR "class _create failed");
                return PTR_ERR(cls);
        }
        device_create(cls,NULL,dev,NULL,"myled%d",MINOR(dev));
        return 0;
}

static void __exit cleanup_mod(void){
        cdev_del(&cdv);
        device_destroy(cls,dev);
        class_destroy(cls);
        unregister_chrdev_region(dev,1);
        printk(KERN_INFO "%s is unloaded.major:%d\n",__FILE__,MAJOR(dev));
}


module_init(init_mod);
module_exit(cleanup_mod);