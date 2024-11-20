### Protocol Parsing

In this step we are going to do Protocol Parsing so that 
we will be able to process mutiple requests from a Client

The easiest way to achieve this is to declare how long the request
is at the beginning of the request
Like this:

+-----+------+-----+------+--------
| len | msg1 | len | msg2 | more...
+-----+------+-----+------+--------

