#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>    //追加
#include <linux/uaccess.h>    //ヘッダに追加
#include <linux/io.h>　


MODULE_AUTHOR("Ryuichi Ueda & Keito Yamada");
MODULE_DESCRIPTION("driver for LED control");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.0.1");

static struct class *cls = NULL;  //追加

static dev_t dev;
static struct cdev cdv;
static volatile u32 *gpio_base = NULL;  //アドレスをマッピングするための配列をグローバルで定義

//led_writeを次のように書き換え
static ssize_t led_write(struct file* filp, const char* buf, size_t count, loff_t* pos)
{
	char c;   //読み込んだ字を入れる変数
    if(copy_from_user(&c,buf,sizeof(char)))
  	return -EFAULT; //失敗した時に正の値になるため判定を逆転
  
    if(c == 'q'){ //0と打ったら
        gpio_base[10] = 1 << 25; //受け取った場所はセンサ値のビットなので，GPIO25が機能するビットにするために10回シフトする
        gpio_base[10] = 1 << 24;
        gpio_base[10] = 1 << 23;
   	}
   	else if(c == 'r'){
        gpio_base[7] = 1 << 25;
        gpio_base[10] = 1 << 24;
        gpio_base[10] = 1 << 23;
   	}
   	else if(c == 'b'){
        gpio_base[7] = 1 << 24;
        gpio_base[10] = 1 << 23;
        gpio_base[10] = 1 << 25;
   	}	
   	else if(c == 'g'){
        gpio_base[7] = 1 << 23;
        gpio_base[10] = 1 << 24;
        gpio_base[10] = 1 << 25;
   	}
   	else if(c == 'p'){
        gpio_base[7] = 1 << 25;
        gpio_base[7] = 1 << 24;
        gpio_base[10] = 1 << 23;
   	}
   	else if(c == 'y'){
        gpio_base[7] = 1 << 23;
        gpio_base[7] = 1 << 25;
        gpio_base[10] = 1 << 24;
   	}	
   	else if(c == 'l'){
        gpio_base[7] = 1 << 23;
        gpio_base[7] = 1 << 24;
        gpio_base[10] = 1 << 25;
   	}
   	else if(c == 'w'){
        gpio_base[7] = 1 << 23;
        gpio_base[7] = 1 << 24;
        gpio_base[7] = 1 << 25;
     }
     return 1;
}



static ssize_t sushi_read(struct file* filp, char* buf, size_t count, loff_t* pos)
{
    int size = 0;
    char sushi[] = {0xF0,0x9F,0x8D,0xA3,0x0A}; //寿司の絵文字のバイナリ
    if(copy_to_user(buf+size,(const char *)sushi, sizeof(sushi))){
        printk( KERN_INFO "sushi : copy_to_user failed\n" );
    return -EFAULT;
    }
    size += sizeof(sushi);
    return size;
}

static struct file_operations led_fops = {
    .owner = THIS_MODULE,
    .write = led_write,
    .read = sushi_read //デバイスファイルが読める様になる
};

static int __init init_mod(void)
{
    int retval;
	gpio_base = ioremap_nocache(0x3f200000, 0xA0); //Pi4の場合は0xfe200000

//25ピンin/outをoutに設定
	const u32 r_led = 25;
    const u32 r_index = r_led/10;//GPFSEL2 32bitを10分割した時の2番目のグループに存在する
    const u32 r_shift = (r_led%10)*3;//15bit 15~17で制御できる
    const u32 r_mask = ~(0x7 << r_shift);//11111111111111000111111111111111 動画1:30から
    gpio_base[r_index] = (gpio_base[r_index] & r_mask) | (0x1 << r_shift);//001: output flag
    //11111111111111001111111111111111
	
//24ピンin/outをoutに設定
	const u32 b_led = 24;
    const u32 b_index = b_led/10;
    const u32 b_shift = (b_led%10)*3;
    const u32 b_mask = ~(0x7 << b_shift);
    gpio_base[b_index] = (gpio_base[b_index] & b_mask) | (0x1 << b_shift);
   
//23ピンin/outをoutに設定
	const u32 g_led = 23;
    const u32 g_index = g_led/10;
    const u32 g_shift = (g_led%10)*3;
    const u32 g_mask = ~(0x7 << g_shift);
    gpio_base[g_index] = (gpio_base[g_index] & g_mask) | (0x1 << g_shift);
     
	
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
