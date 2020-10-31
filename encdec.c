#include <linux/module.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <asm/uaccess.h>
#include <linux/random.h>
#include <linux/ctype.h>

MODULE_LICENSE("MIT?");

static char msg[1000] = {0};
static int msg_len = 0;
static short readPos = 0;

static int file_open(struct inode *, struct file *);
static int file_release(struct inode *, struct file *);
static ssize_t file_read(struct file *, char *, size_t, loff_t *);
static ssize_t file_write(struct file *, const char *, size_t, loff_t *);

static struct file_operations fo = {
	.read = file_read,
	.open = file_open,
	.write = file_write,
	.release = file_release,
};

int init_module(void) {
    int t = register_chrdev(150, "encdec", &fo);
    if (t < 0) {
        printk("Intialization of device failed");
    } else {
        printk("Intialization of device successfull");
    }
    return t;
}

void cleanup_module(void) {
	unregister_chrdev(150, "encdec");
	printk("Device unregistered successfully");
}

static int file_open(struct inode *in, struct file *fil) {
	return 0;
}

static ssize_t file_read(struct file *fil, char *buf, size_t len, loff_t *off) {
    short count = 0;
    readPos = 0;
    while (msg_len && (msg[readPos] != 0)) {
        printk("read");
        put_user(msg[readPos], buf++);
        count++;
        msg_len--;
        readPos++;
    }
    return count;
}

int input_parse(char *buf) {
    char temp[1000];
    int len, i,j;
    strcpy(temp, buf);
    len = strlen(temp);
    for (i = 0, j = 0; j < len; ++j) {
        if (isalpha(temp[j])) {
            buf[i++] = toupper(temp[j]);
        }
    }
    buf[i] = '\0';
    return i;
}

int encrypt_string(char *buf, char *key) {
    char temp[1000];
    int i, j=0;
    int len = input_parse(buf);
    int keyLen = input_parse(key);
    printk("-----Starting encryption-----");
    for (i = 0; i < len; ++i) {
        temp[i] = (char)(((int)(buf[i] - 65) + (int)(key[j] - 65))%26 + 65);
        j = (j+1)%keyLen;
    }
    temp[i] = '\0';
    strcpy(msg, temp);
    msg_len = strlen(msg);
    printk("Ciphertext:");
    printk(msg);
    return 0;
}

int decrypt_string(char *buf, char *key) {
    char temp[1000];
    int i, j=0;
    int len = input_parse(buf);
    int keyLen = input_parse(key);
    printk("-----Starting decryption-----");
    for (i = 0; i < len; ++i) {
        temp[i] = (char)(((int)(buf[i] - 65) - (int)(key[j] - 65) + 26)%26 + 65);
        j = (j+1)%keyLen;
    }
    temp[i] = '\0';
    strcpy(msg, temp);
    msg_len = strlen(msg);
    printk("Plaintext:");
    printk(msg);
    return 0;
}

static ssize_t file_write(struct file *fil, const char *buf, size_t len, loff_t *off) {
    char copy[1000];
    char temp[1000];
    char *key;
    char *operation;
    char *str1;

    memset(msg, 0, 1000);
    if (raw_copy_from_user(copy, buf, 1000) != 0)
        return -EFAULT;
    printk("Starting...");
    strcpy(temp, copy);

    str1 = copy;
    operation = strsep(&str1, ":");
    if ( str1 == NULL )
        return -EINVAL;
    printk("Encrypt/Decrypt:");
    printk(operation);

    key = strsep(&str1, ":");
    if ( str1 == NULL )
        return -EINVAL;
    printk("Key:");
    printk(key);

    printk("Input String:");
    printk(str1);
    if ( str1 == NULL )
        return -EINVAL;

    if (operation[0] == 'E') {
        encrypt_string(str1, key);
    } else if (operation[0] == 'D') {
        decrypt_string(str1, key);
    }
    printk("--------------------");
    return strlen(temp);
}

static int file_release(struct inode *in, struct file *fil) {
    printk("Input parsed succesfully...");
    return 0;
}
