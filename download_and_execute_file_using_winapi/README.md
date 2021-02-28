## Application to download a file/program from a server using Windows APIs
**Objective:** Build an application that will download a file/program from a server using its download URL. After successfully downloading, execute/run the downloaded file on the system.
<br>
<br>
**Working:** The application built using one of the Windows APIs named WinHTTP Services. WinHTTP Services provides developers with an HTTP client application programming interface (API) to send requests through the HTTP protocol to other HTTP servers. The application is programmed in CPP Language and built on MS Visual Studio 2019 software. The application downloads the file by using the following steps:
<pre>
1. GET Request method, and WinHTTP Services are used to establish the connection to the file's server using its URL.
2. Create an empty file which would be the downloaded file or the output file.
3. Perform a do-while loop and simultaneously collect the required file's binary data and write it to the output file.
4. Loop ends when all the binary data collected means download is complete.
5. The output file is the required downloaded file.
6. Execute the file/program using CreateProcess API or the system command.
</pre>

##### This directory contains all input files and output files description of which given below:
<h4>
<pre>
1. download_and_execute_file_using_winapi.exe    ==> Final application that follows the objective.                 
2. sample_file.txt                               ==> Output or downloaded file.
3. src                                           ==> This folder contains all source files of the final application.
4. README.md                                     ==> Gives the brief details of all the files of this directory.
</pre>
</h4>
