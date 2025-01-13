# betterOS
Let's get better

### How to run ??
```make betterKernel.iso```
#### On one terminal
```qemu-system-i386 -cdrom build/betterKernel.iso```
#### On onther terminal
```vncviewer```

##### <b>NOTE: </b> Make sure <a herf="https://www.qemu.org/">QEMU</a> and <a herf="https://tigervnc.org/">TigerVNC</a> are installed. This compilation was tested on Arch Linux, may not compile on Windows.
After you type in <b>vncviewer</b>, you need to enter the address displayed on the first terminal.

### Dependencies
<ol>
    <li> G++ Compiler </li>
    <li> QEMU </li>
    <li> TigerVNC </li>
    <li> xorriso </li>
</ol>

