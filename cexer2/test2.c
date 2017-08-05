void print_process_parents(pid_t pid) {

  pid_t current = pid;

 while (current != 1 ) {
  
  printf("%d\n",current);
  current = get_parent_pid(current)

 }

 printf("%d (init) \n",current);
}
