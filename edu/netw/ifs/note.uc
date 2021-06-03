if you made mistake in interface config then delete all
virtual via `ifconfig eth0:X down`, correct config and
reload network service.

you cannot assign IP address to virtual network interface
that's lower than physical NIC IP address.
