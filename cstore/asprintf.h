static int 
asprintf (char **ps, const char *fmt, ...)
{
  //   puts("my asprintf");
  va_list ap;
  va_start(ap, fmt);
  int rc = vsnprintf(*ps, 0, fmt, ap);
  va_end(ap);
  if (rc >= 0) {
    if ((*ps = (char *)malloc(rc + 2))) {
      va_start(ap, fmt);
      rc = vsnprintf(*ps, rc + 1, fmt, ap);
      va_end(ap);
    } else
      rc = -1;
  }

  return rc;
}
