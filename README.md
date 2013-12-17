ProjetSE
========

A project of embedded system done during the third year in robotics at
ENSEIRB-MATMECA.

The firmware for the Atmega8 on the Arduino board is implemented in the
`firmware` folder.

The driver running on the Raspberry pi to communicate with the other board is
implemented in the `driver` folder.

Both of them are written in C.


Tests
-----
Several tests are built by the `make` command, each one has it's own purpose.
While the latest one include the use of two processus communicating and using
assertions, older tests which were used through the developing process were
simpler and require more attention from the user.

###communication_test

The communication test consists of launching a fake firmware which will have
the same behavior than the true firmware. It will additionnally open two pipes
in /tmp/ folder. After it, the test_communication binary can be launched in
order to test the transmission. Asserts will be done, ensuring that values are
written and received as expected (from the driver side).

Tests on failsafe might be disabled by the use of the appropriate compilation
flag.

Tests is launched with :
```./fake_device &
./test_communication```


###test_driver

This test is using various binary files, each one representing a packet. The
connection input is changed to test each available command. No assertions are
used but the expected input is showed and the user can compare.

###test_protocol

This test compare the expected and calculated checksum, packet conversion and
packet validation. It has no assertion and need to be checked by an user

###test_bit_utils

This executable launch tests with no assertion, the user has to look if the
result is the same as the expected value
