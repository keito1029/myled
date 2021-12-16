#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>    //追加

MODULE_AUTHOR("Ryuichi Ueda & Keito Yamada");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static struct class *cls = NULL;  //追加

static dev_t dev;
static struct cdev cdv;

static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
        printk(KERN_INFO "led_write is called\n");
        return 1; //読み込んだ文字数を返す（この場合はダミーの1）
}

static struct file_operations led_fops = {
        .owner = THIS_MODULE,
		.write = led_write
};


static int __init init_mod(void)
{
    int retval;
    retval =  alloc_chrdev_region(&dev, 0, 1, "myled");
    if(retval < 0){
        printk(KERN_ERR "alloc_chrdev_region failed.\n");
        return retval;
    }
        printk(KERN_INFO "%s is loaded. major:%d\n",__FILE__,MAJOR(dev));

        cdev_init(&cdv, &led_fops);
        retval = cdev_add(&cdv, dev, 1);
        if(retval < 0){
                printk(KERN_ERR "cdev_add failed. major:%d, minor:%d",MAJOR(dev),MINOR(dev));
                return retval;
        }

        cls = class_create(THIS_MODULE,"myled");
        if(IS_ERR(cls)){
                printk(KERN_ERR "class_create failed.");
                return PTR_ERR(cls);
        }
        device_create(cls, NULL, dev, NULL, "myled%d",MINOR(dev));//デバイスファイルの名前 myled0になる
        return 0;
}


static void __exit cleanup_mod(void)
{
        cdev_del(&cdv);
		device_destroy(cls, dev);
        class_destroy(cls);  //追加
        unregister_chrdev_region(dev, 1);
        printk(KERN_INFO "%s is unloaded. major:%d\n",__FILE__,MAJOR(dev));
}


module_init(init_mod);     // マクロで関数を登録
module_exit(cleanup_mod);  // 同上

//その後，devディレクトリに認識させるためにノード作成 $ sudo mknod /dev/myled0 c デバイス番号 0
// sudo rm /dev/myled0    #後始末
//$ sudo rmmod myled
