# Lab 2

## Part 1

For execution process do the following.

In host machine:


- First navigate to folder where the zip file(containing .c & make file) is present.
- run following command for transferring the file to minix. (change file_name & ip address accordingly.)
```
scp 200010004_lab2_part1.zip root@10.196.7.20:/root
```

In minix system:

- Navigate to `root` (preferably root here.)
- Run following commands to extract zip file contents and then `gmake` will produce output. (which compiles with `clang` compiler, done.)
```
unzip 200010004_lab2_part1.zip 
gmake hello
```