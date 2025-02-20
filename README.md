# betterOS

Let's get better

### How to run ??

``make betterKernel.iso``

#### On one terminal

``qemu-system-i386 -cdrom build/betterKernel.iso``

#### On onther terminal

``vncviewer``

##### `<b>`NOTE: `</b>` Make sure `<a herf="https://www.qemu.org/">`QEMU `</a>` and `<a herf="https://tigervnc.org/">`TigerVNC `</a>` are installed. This compilation was tested on Arch Linux, may not compile on Windows.

After you type in `<b>`vncviewer `</b>`, you need to enter the address displayed on the first terminal.

### Dependencies

<ol>
    <li> G++ Compiler </li>
    <li> QEMU </li>
    <li> TigerVNC </li>
    <li> xorriso </li>
	<li> How to get scancodes ? Click <a href = "https://kbdlayout.info/kbdusx/scancodes?arrangement=ANSI104">here</a></li>
</ol>

### Tips for future reference:

<ol>
    <li> <b>__stack_chk_fail_local</b> ERROR -> Add -fno-stack-protector in GCC Flags </li>
    <li> <b>To view all qemu devices</b> -> qemu-system-x86_64 -device help </li>
</ol>
