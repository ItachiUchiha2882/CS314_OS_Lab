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

## Part 2

Almost similar process just zip(twice.c, square.c, half.c), transfer it minix then unzip and run `gmake` then run with command line arguments for eg. 
```
./twice ./square ./half ./twice ./half 10
```

## Part 3

Make changes in main `minix` file directory, then transfer that particular file to the minix system (Don't forget to make a branch for each lab). Then make build it, reboot and see results.