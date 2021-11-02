# Семинар №10
## Файловые аттрибуты

---

### Атрибуты файла в общих чертах

[GNU docs](https://www.gnu.org/software/libc/manual/html_node/File-Attributes.html)  
[Общий ридинг](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/stat_fcntl)
  
В файловой системе Linux с каждым файлом связана некая метаинформация, которая определяется структурой `struct stat`:
  
```c
struct stat {
   dev_t     st_dev;         /* ID of device containing file */
   ino_t     st_ino;         /* Inode number */
   mode_t    st_mode;        /* File type and mode */
   nlink_t   st_nlink;       /* Number of hard links */
   uid_t     st_uid;         /* User ID of owner */
   gid_t     st_gid;         /* Group ID of owner */
   dev_t     st_rdev;        /* Device ID (if special file) */
   off_t     st_size;        /* Total size, in bytes */
   blksize_t st_blksize;     /* Block size for filesystem I/O */
   blkcnt_t  st_blocks;      /* Number of 512B blocks allocated */

   struct timespec st_atim;  /* Time of last access */
   struct timespec st_mtim;  /* Time of last modification */
   struct timespec st_ctim;  /* Time of last status change */

   /* Backward compatibility */
   #define st_atime st_atim.tv_sec      
   #define st_mtime st_mtim.tv_sec
   #define st_ctime st_ctim.tv_sec
};
```
Метаинформацию о файле можно получить с помощью команды ```stat <filename>```:
```shell
m.v.tsion@MacBook-Air-Mikhail 00-read % stat main.c
16777231 2695583 -rw-r--r-- 1 m.v.tsion staff 0 1119 "Nov  2 15:58:32 2021" "Nov  2 15:56:49 2021" "Nov  2 15:56:49 2021" "Oct 29 17:56:32 2021" 4096 8 0x40 main.c
```
или с помощью одного из системных вызовов (см. `man 2`) из `#include <sys/stat.h>`:
  * `int stat(const char *file_name, struct stat *stat_buffer)` - получение информации о файле по его имени;
  * `int fstat(int fd, struct stat *stat_buffer)` - то же самое, но для открытого файлового дескриптора;
  * `int lstat(const char *path_name, struct stat *stat_buffer)` - аналогично stat, но в случае, если имя файла 
    указывает на символическую ссылку, то возвращается информация о самой ссылке, а не о файле, на который она ссылается 

`struct stat *stat_buffer` - буффер, куда записывается результат исполнения вызова.  
Возвращаемое значение:
```shell
RETURN VALUES
     Upon successful completion a value of 0 is returned.  Otherwise, a value of -1 is returned and errno is set to indicate the error.
```

Часто используемым полем данной структуры является поле `.st_mode`:

```shell
The status information word st_mode has the following bits:

     #define S_IFMT   0170000     /* type of file */         <--- биты соответствующие маске файла
     // биты масок ниже сравниваются с битами маски файла:
     #define    S_IFIFO  0010000  /* named pipe (fifo) */
     #define    S_IFCHR  0020000  /* character special */
     #define    S_IFDIR  0040000  /* directory */
     #define    S_IFBLK  0060000  /* block special */
     #define    S_IFREG  0100000  /* regular */
     #define    S_IFLNK  0120000  /* symbolic link */
     #define    S_IFSOCK 0140000  /* socket */
     #define    S_IFWHT  0160000  /* whiteout */
     #define    S_ISUID  0004000  /* set user id on execution */
     
     #define S_ISGID  0002000  /* set group id on execution */
     #define S_ISVTX  0001000  /* save swapped text even after use */
     #define S_IRUSR  0000400  /* read permission, owner */
     #define S_IWUSR  0000200  /* write permission, owner */
     #define S_IXUSR  0000100  /* execute/search permission, owner */

     For a list of access modes, see <sys/stat.h>, access(2) and chmod(2).

     For a list of the file flags in the st_flags field, see <sys/stat.h> and chflags(2).
```

Как тогда проверить является ли файл регулярным файлом:
```c
(stat_buffer.st_mode & S_IFMT) == S_IFREG
```
  
C другой стороны есть предопределенные макросы из `sys/stat.h`, чтобы не писать много кода с масками:
```c
S_ISREG(stat_buffer.st_mode)  <~~~> (stat_buffer.st_mode & S_IFMT) == S_IFREG
```
но не факт, что значения справа и слева равны, есть гарантия, что справа и слева будет ненулевое значение, если 
метадата соответствует регулярному файлу.

Помимо обычных (regular) файлов в рамках этой темы также будут интересовать:
  * директории (файл с относительными путями для файлов "содержащихся" в директории)
  * символическая ссылка (файл содержащий путь на файл-оригинал)

---

### Атрибуты файловых дескрипторов
[Ссылка](https://github.com/victor-yacovlev/mipt-diht-caos/tree/master/practice/stat_fcntl#%D0%B0%D1%82%D1%82%D1%80%D0%B8%D0%B1%D1%83%D1%82%D1%8B-%D1%84%D0%B0%D0%B9%D0%BB%D0%BE%D0%B2%D1%8B%D1%85-%D0%B4%D0%B5%D1%81%D0%BA%D1%80%D0%B8%D0%BF%D1%82%D0%BE%D1%80%D0%BE%D0%B2)