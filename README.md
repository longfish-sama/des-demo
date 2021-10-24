# des-demo

a demo application of DES (Data Encryption Standard)

对称密钥加密算法DES的演示程序

## 使用

使用命令行输入

```
SYNOPSIS
        Untitled-1.exe -f <input file> -k <key> (-e|-d)

OPTIONS
        -f, --filepath <input file>
                    file path

        -k, --key <key>
                    key for encrypt/decrypt

        -e, --encrypt|-d, --decrypt
                    set mode
```
## 问题

仅能对文件大小为64bits整数倍的文件进行正确操作；对于不满足该条件的文件，将会忽略文件尾若干bytes以满足条件