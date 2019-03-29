### bsdiff是一个差异包比较工具,可以用来实现增量更新.

[下载地址](http://www.daemonology.net/bsdiff)

编译 Mac环境 版本：macOS 10.14.3

1. 解压下载的bsdiff-4.3.tar.gz
```
tar -zxvf bsdiff-4.3.tar.gz
```

2. 进入bsdiff-4.3目录，在终端下执行构建
```
cd bsdiff-4.3

make
```
此时报错： **-Makefile:13: *** missing separator.  Stop.**

这是makefile文件的内容的格式不对，修改Makefile文件，修改后的文件内容如下：
```
CFLAGS          +=      -O3 -lbz2

PREFIX          ?=      /usr/local
INSTALL_PROGRAM ?=      ${INSTALL} -c -s -m 555
INSTALL_MAN     ?=      ${INSTALL} -c -m 444

all:            bsdiff bspatch
bsdiff:         bsdiff.c
bspatch:        bspatch.c

install:
        ${INSTALL_PROGRAM} bsdiff bspatch ${PREFIX}/bin
        .ifndef WITHOUT_MAN
        ${INSTALL_MAN} bsdiff.1 bspatch.1 ${PREFIX}/man/man1
        .endif
```

修改方法为增加 `.ifndef WITHOUT_MAN`与`。endif`的缩进与上下行对齐

再执行make，又有报错如下：
```
cc -O3 -lbz2    bsdiff.c   -o bsdiff
cc -O3 -lbz2    bspatch.c   -o bspatch
bspatch.c:39:21: error: unknown type name 'u_char'; did you mean 'char'?
static off_t offtin(u_char *buf)
                    ^~~~~~
                    char
bspatch.c:65:8: error: expected ';' after expression
        u_char header[32],buf[8];
              ^
              ;
bspatch.c:65:2: error: use of undeclared identifier 'u_char'; did you mean
      'putchar'?
        u_char header[32],buf[8];
        ^~~~~~
        putchar
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/stdio.h:172:6: note:
      'putchar' declared here
int      putchar(int);
         ^
bspatch.c:65:9: error: use of undeclared identifier 'header'
        u_char header[32],buf[8];
               ^
bspatch.c:65:20: error: use of undeclared identifier 'buf'
        u_char header[32],buf[8];
                          ^
bspatch.c:66:2: error: use of undeclared identifier 'u_char'; did you mean
      'putchar'?
        u_char *old, *new;
        ^~~~~~
        putchar
/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk/usr/include/stdio.h:172:6: note:
      'putchar' declared here
int      putchar(int);
         ^
bspatch.c:66:10: error: use of undeclared identifier 'old'
        u_char *old, *new;
                ^
bspatch.c:66:16: error: use of undeclared identifier 'new'
        u_char *old, *new;
                      ^
bspatch.c:93:12: error: use of undeclared identifier 'header'
        if (fread(header, 1, 32, f) < 32) {
                  ^
bspatch.c:100:13: error: use of undeclared identifier 'header'
        if (memcmp(header, "BSDIFF40", 8) != 0)
                   ^
bspatch.c:104:19: error: use of undeclared identifier 'header'
        bzctrllen=offtin(header+8);
                         ^
bspatch.c:105:19: error: use of undeclared identifier 'header'
        bzdatalen=offtin(header+16);
                         ^
bspatch.c:106:17: error: use of undeclared identifier 'header'
        newsize=offtin(header+24);
                       ^
 * 2. Redistributions in binary form must reproduce the above copyright
bspatch.c:137:5: error: use of undeclared identifier 'old'
                ((old=malloc(oldsize+1))==NULL) ||
                  ^
bspatch.c:139:12: error: use of undeclared identifier 'old'
                (read(fd,old,oldsize)!=oldsize) ||
                         ^
bspatch.c:141:6: error: use of undeclared identifier 'new'
        if((new=malloc(newsize+1))==NULL) err(1,NULL);
            ^
bspatch.c:147:43: error: use of undeclared identifier 'buf'
                        lenread = BZ2_bzRead(&cbz2err, cpfbz2, buf, 8);
                                                               ^
bspatch.c:151:19: error: use of undeclared identifier 'buf'
                        ctrl[i]=offtin(buf);
                                       ^
bspatch.c:159:42: error: use of undeclared identifier 'new'
                lenread = BZ2_bzRead(&dbz2err, dpfbz2, new + newpos, ctrl[0]);
                                                       ^
fatal error: too many errors emitted, stopping now [-ferror-limit=]
20 errors generated.
make: *** [bspatch] Error 1
```

解决方法为在bspatch.c文件中加入`#include <sys/types.h>`

然后再执行make命令，此时命令执行成功，会在当前目录下生成bsdiff,bspatch文件

3. 生成差分包：
```
bsdiff oldfile newfile patchfile
```

