# http_set_cookie_parser

This program recieves HTTP\1.1 response messages via standard input and 
parses Set-Cookie headers only. It will stop reading when the end of 
file (EOF) is reached. Within a terminal, EOF can be forced by pressing 
ctrl+d two times. 

Details:
This parser only cares about the Set-Cookie headers within HTTP\1.1 
responce messages. Furthermore, it is only concerned with the name 
and value of the Set-Cookie header. It will ignore other directives
such as Max-Age, Expires, etc. If multiple Set-Coookie headers
are recieved with the same name, the value of the most recent 
Set-Cookier header will replace the value of the oldest.

Sample input is provided below:

HTTP/1.1 404 Not Found
Date: Sun, 18 Oct 2012 10:36:20 GMT
Server: Apache/2.2.14 (Win32)
Content-Length: 230
Set-Cookie: Username=Test123
Connection: Closed
Content-Type: text/html; charset=iso-8859-1

HTTP/1.1 404 Not Found
Date: Sun, 18 Oct 2012 10:36:20 GMT
Server: Apache/2.2.14 (Win32)
Content-Length: 230
Set-Cookie: sessionId=38afes7a8
Set-Cookie: helloWorld=fes7a8; Expires=Tues, 21 Oct 2019 09:30:00 GMT
Connection: Closed
Content-Type: text/html; charset=iso-8859-1

HTTP/1.1 100 OK
Date: Sun, 18 Oct 2012 10:36:20 GMT
Server: Apache/2.2.14 (Win32)
Content-Length: 230
Set-Cookie: UN=HelloWorld
Connection: Closed
Content-Type: text/html; charset=iso-8859-1

HTTP/1.1 404 Not Found
Date: Sun, 18 Oct 2012 10:36:20 GMT
Server: Apache/2.2.14 (Win32)
Content-Length: 230
Set-Cookie: HELLO_WORLD=hello_world
Connection: Closed
Content-Type: text/html; charset=iso-8859-1

HTTP/1.1 100 OK
Date: Sun, 18 Oct 2012 10:36:20 GMT
Server: Apache/2.2.14 (Win32)
Content-Length: 230
Set-Cookie: lastName=Smith
Connection: Closed
Content-Type: text/html; charset=iso-8859-1

HTTP/1.1 100 OK
Date: Sun, 18 Oct 2012 10:36:20 GMT
Server: Apache/2.2.14 (Win32)
Content-Length: 230
Set-Cookie: SessionID=12345
Set-Cookie: SessionID=abcdef
Set-Cookie: SessionID=a1b2c3d4;Expires=Wed, 21 Oct 2015 07:28:00 GMT
Set-Cookie: SessionID=1a2b3c4d
Set-Cookie: SessionID=11aa22bb33cc; Domain=somecompany.co.uk
Set-Cookie: SessionID=qwerty12345; Max-Age:45
Connection: Closed
Content-Type: text/html; charset=iso-8859-1
