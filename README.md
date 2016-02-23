# usbipClient
global flag=0

while(1) {
    revc(buf);
    ++flag;
    if 0 == flag:
        create thread(0);
    else:
        continue
}
keep thread0 sleep a while
