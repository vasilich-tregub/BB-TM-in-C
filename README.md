# TMs of Busy Beaver BB(2, N) champions

I authored this C implementation of Busy Beaver TMs with the same algorithm that I used 
for my 
[implementation of the two-symbol TM machine in Web Assempler](https://vasilich-tregub.github.io/wa-bb/index.html)
with the only purpose of comparing their performances. Within the accuracy of my measurement 
method, the WebAssembler implementation wins.

The program 'webui-BBTM.exe' uses an http server to attach GUI to the code identical with that 
of the console application 'BB-TM-in-C.exe'. You launch 'webui-BBTM.exe' and open the web page 
index.html in your browser, navigating to the URL https://localhost:8080.  
The web page sends the TM rules to the TM simulator which runs the TM code. The page requests 
the results with the xmlhttprequest to the webui-BBTM http server and displays the response. 
The page layout is identical to that of the [BB 2-symbol TM](https://vasilich-tregub.github.io/wa-bb/index.html).

You can also run the BB-2-symbol-TM simulator included with this project while navigating to 
https://localhost:8080/wa-bb/. Do not omit the trailing slash of this URL.

Note to developers: to update the web page files of the project to be copied to the CMAKE 
current binary directory after edit, forcefully save CMakeLists.txt.

