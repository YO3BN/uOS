



int ipc(void *s)
{
  return 0;
}



int ipc_send(const unsigned char *buffer, const unsigned long size, const int tid);
int ipc_recv(const unsigned char *buffer, const unsigned long size);

