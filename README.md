RawCamera-data-converter
========================

This program was made to convert raw data from the ov7670 to a png file.
Data from other cameras will also work.
To use the program you should first run it with --help or -h to see an usage guide.
This will tell you how to use the program.
Note that this is a cross platform program.
I have tested it with GNU/Linux and compiled it for windows and tested that with wine.

When you have error on compile with
===================================

`savePng.c undefined reference to `png_create_write_struct`
I think you can see the message when you compile this converter on ubuntu machine.
If you so, unzip `libpng12.tar` to `/usr/lib/` on ubuntu directory.
and use `convert` binary file to convert .raw to .png file.
