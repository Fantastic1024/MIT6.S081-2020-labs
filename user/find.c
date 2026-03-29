#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *
fmtname(char *path)
{
  static char buf[DIRSIZ + 1];
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
  return buf;
}

void find(char *path, char *fname)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(path, 0)) < 0)
  {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0)
  {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
  {
    printf("find: path too long\n");
  }
  strcpy(buf, path);
  p = buf + strlen(buf);
  *p++ = '/';  // 确定根目录用于寻找

  while (read(fd, &de, sizeof(de)) == sizeof(de))
  {
    if (de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;  // Modify buf to every file in current dir

    struct stat st2;  // !!!
    if (strcmp(p, ".") == 0 || strcmp(p, "..") == 0){
      continue;
    }

    if (stat(buf, &st2) < 0)  // buf will change to current file\dir in every loop
    {
      fprintf(2, "find: cannot stat %s\n", buf);
      close(fd);
      return;
    }

    if (st2.type == T_DIR){
      find(buf, fname);
    }

    if (st2.type == T_FILE){
      if (strcmp(p, fname) == 0){
        printf("%s\n", buf);
      }
    }
  }
  close(fd);
}

int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("usage: find dir filename");
    exit(2);
  }

  find(argv[1], argv[2]);
  exit(0);
}